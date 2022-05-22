#include "Ball.h"

#include <common/Logger.h>

using namespace Common;

Game::Ball::Ball(Engine::IRenderer::RefPtr renderer, Engine::Physics::RefPtr physics, float radius)
    : Engine::Sphere(renderer, radius)
    , m_pPhysics(physics) {

  reactphysics3d::Vector3 position(m_position.x, m_position.y, m_position.z);
  reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
  reactphysics3d::Transform transform(position, orientation);
  m_pPhysicsBody = m_pPhysics->m_pPhysicsWorld->createRigidBody(transform);

  m_pCollisionShape = m_pPhysics->m_physicsCommon.createSphereShape(radius);
  m_pCollider = m_pPhysicsBody->addCollider(m_pCollisionShape, transform);

  m_rotationMatrix = Matrix::Identity();
  m_translateMatrix = Matrix::FromTranslationVector(m_position);

  m_textureImage = "assets\\ball.png";
}

Game::Ball::~Ball() {
  m_pPhysicsBody->removeCollider(m_pCollider);
  m_pPhysics->m_physicsCommon.destroySphereShape(m_pCollisionShape);
  m_pPhysics->m_pPhysicsWorld->destroyRigidBody(m_pPhysicsBody);
}

void Game::Ball::SetPosition(Common::Float3 position) {
  GameObject::SetPosition(position);
  m_translateMatrix = Matrix::FromTranslationVector(position);

  auto transform = m_pPhysicsBody->getTransform();
  m_pPhysicsBody->setTransform(
      reactphysics3d::Transform(reactphysics3d::Vector3(m_position.x, m_position.y, m_position.z), //
                                m_pPhysicsBody->getTransform().getOrientation())                   //
  );
}

void Game::Ball::SetForceToMassCenter(float fx, float fy, float fz) {
  m_pPhysicsBody->applyWorldForceAtCenterOfMass({fx, fy, fz});
}

void Game::Ball::SetBounds(Common::Float4 rect) {
  m_bound = rect;
}

void Game::Ball::Update(const Engine::StepTimer& timer) {
  reactphysics3d::Transform currTransform = m_pPhysicsBody->getTransform();

  currTransform.getOpenGLMatrix(&(m_worldMatrix[0]));
  //// compute move vector
  // float elapsedSecond = static_cast<float>(timer.GetElapsedSeconds());
  // m_moveVector = m_velocity * elapsedSecond;

  //// handle world boundary collision
  // HandleWorldBoundaryCollision(m_position + m_moveVector);

  //// update position
  // m_position += m_moveVector;

  //// update accumulated translation matrix
  // m_translateMatrix *= Matrix::FromTranslationVector(m_moveVector);

  ////// update self-rotation matrix
  // m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationX(m_moveVector.z / m_radius));
  // m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationZ(-m_moveVector.x / m_radius));

  //// compute the word matrix by translation and rotation
  // m_worldMatrix = m_translateMatrix * m_rotationMatrix;

  // reactphysics3d::Transform transform(reactphysics3d::Vector3(m_position.x, m_position.y, m_position.z), //
  //                                     reactphysics3d::Quaternion() // ignore rotation
  //);
  // m_pPhysicsBody->setTransform(transform);
}

void Game::Ball::HandleWorldBoundaryCollision(Common::Float3 pos) {
  // collision detection
  Float3 reboundVector = Math::kZeros3f;
  Float3 limitPosition = m_position;

  bool collisionOccured = false;

  // left
  if ((pos.x - m_radius - m_bound.x) < Common::F_MIN) {
    // compute the rebound vector
    reboundVector += m_velocity - 2.0f * (m_velocity * Math::kAxisX3f) * Math::kAxisX3f;
    // reset position
    limitPosition.x = m_bound.x + m_radius;

    collisionOccured = true;
  }

  // top
  if ((pos.z - m_radius - m_bound.y) < Common::F_MIN) {
    reboundVector += m_velocity - 2.0f * (m_velocity * Math::kAxisZ3f) * Math::kAxisZ3f;
    limitPosition.z = m_bound.y + m_radius;
    collisionOccured = true;
  }

  // right
  if ((pos.x + m_radius - m_bound.z) > Common::F_MIN) {
    reboundVector += m_velocity - 2.0f * (m_velocity * -Math::kAxisX3f) * -Math::kAxisX3f;
    limitPosition.x = m_bound.z - m_radius;
    collisionOccured = true;
  }

  // bottom
  if ((pos.z + m_radius - m_bound.w) > Common::F_MIN) {
    reboundVector += m_velocity - 2.0f * (m_velocity * -Math::kAxisZ3f) * -Math::kAxisZ3f;
    limitPosition.z = m_bound.z - m_radius;
    collisionOccured = true;
  }

  if (collisionOccured) {
    m_moveVector = limitPosition - m_position;

    if (reboundVector != Math::kZeros3f) {
      m_velocity = reboundVector;
    } else {
      // if the computed new velocity is zero, we just inverse the original one
      m_velocity = -m_velocity;
    }
  }
}

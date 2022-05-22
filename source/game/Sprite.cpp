#include "Sprite.h"

#include <common/Logger.h>

using namespace Common;

Game::Sprite::Sprite(Engine::IRenderer::RefPtr renderer, Engine::Physics::RefPtr physics,
                     Engine::InputContoller::RefPtr controller, float radius)
    : Game::Ball(renderer, physics, radius)
    , m_pController(controller) {

  m_textureImage = "assets\\sprite.jpg";
}

Game::Sprite::~Sprite() {
}

// void Game::Sprite::Update(const Engine::StepTimer& timer) {
//   auto velocity = m_pController->GetVelocity() + m_velocity;
//
//   // compute move vector
//   float elapsedSecond = static_cast<float>(timer.GetElapsedSeconds());
//   m_moveVector = velocity * elapsedSecond;
//
//   // handle world boundary collision
//   HandleWorldBoundaryCollision(m_position + m_moveVector);
//
//   // update position
//   m_position += m_moveVector;
//
//   // update accumulated translation matrix
//   m_translateMatrix *= Matrix::FromTranslationVector(m_moveVector);
//
//   //// update self-rotation matrix
//   // auto quaternion = Math::Quaternion<float>(0.0f, 0.0f, 0.0f, 0.0f);
//   // auto angle = Math::LengthHelper(m_moveVector) / m_radius;
//   // if (fabsf(angle) > Common::F_MIN) {
//   //   Float3 worldAxis = Float3::CrossProduct(velocity, -Math::kAxisY3f);
//   //   Float3 axis = m_rotationMatrix * worldAxis;
//   //   quaternion = Math::Quaternion<float>::FromAngleAxis(angle, axis);
//   //   // calculate the rotation matrix
//   //   m_rotationMatrix = quaternion.ToMatrix4();
//   // }
//   m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationX(m_moveVector.z / m_radius));
//   m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationZ(-m_moveVector.x / m_radius));
//
//   // compute the word matrix by translation and rotation
//   m_worldMatrix = m_translateMatrix * m_rotationMatrix;
//
//   reactphysics3d::Transform transform(reactphysics3d::Vector3(m_position.x, m_position.y, m_position.z), //
//                                       reactphysics3d::Quaternion() // ignore rotation
//   );
//   m_pPhysicsBody->setTransform(transform);
// }

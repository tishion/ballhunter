#include "Ball.h"

#include <common/Logger.h>

using namespace Common;

Game::Ball::Ball(Engine::IRenderer::RefPtr renderer, float radius)
    : Engine::Sphere(renderer, radius) {
  // m_position.z = -20.0f;
  m_rotationMatrix = Matrix::Identity();
  m_translateMatrix = Matrix::FromTranslationVector(m_position);
}

Game::Ball::~Ball() {
}

void Game::Ball::SetBounds(Common::Float4 rect) {
  m_bound = rect;
}

void Game::Ball::Update(const Engine::StepTimer& timer) {
  // compute move vector
  float elapsedSecond = timer.GetElapsedSeconds();
  m_moveVector = m_velocity * elapsedSecond;

  // handle collision
  HandleCollision(m_position + m_moveVector);

  // update position
  m_position += m_moveVector;

  // update translation matrix
  m_translateMatrix *= Matrix::FromTranslationVector(m_moveVector);

  // update self-rotation matrix
  m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationX(m_moveVector.z / m_radius));
  m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationZ(-m_moveVector.x / m_radius));

  // compute the word matrix by translation and rotation
  m_worldMatrix = m_translateMatrix * m_rotationMatrix;
}

void Game::Ball::HandleCollision(Common::Float3 pos) {
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
      m_velocity = -m_velocity;
    }
  }
}

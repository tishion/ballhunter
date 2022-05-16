#include "Ball.h"

#include <common/Logger.h>

using namespace Common;

namespace Game {

Ball::Ball(Engine::IRenderer::RefPtr renderer, float radius)
    : Engine::Sphere(renderer, radius) {
  m_rotationMatrix = Matrix::FromTranslationVector(m_position);
  m_translateMatrix = Matrix::FromTranslationVector(m_position);
}

Ball::~Ball() {
}

void Ball::Update(const Engine::StepTimer& timer) {
  // compute move vector
  float elapsedSecond = timer.GetElapsedSeconds();
  auto moveVector = m_velocity * elapsedSecond;

  // update position
  m_position += moveVector;

  // update translation matrix
  m_translateMatrix *= Matrix::FromTranslationVector(moveVector);

  // update self-rotation matrix
  m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationX(moveVector.z / m_radius));
  m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationZ(-moveVector.x / m_radius));

  // compute the word matrix by translation and rotation
  m_worldMatrix = m_translateMatrix * m_rotationMatrix;
}
} // namespace Game

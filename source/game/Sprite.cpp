#include "Sprite.h"

#include <common/Logger.h>

using namespace Common;

Game::Sprite::Sprite(Engine::IRenderer::RefPtr renderer, Engine::InputContoller::RefPtr controller, float radius)
    : Engine::Sphere(renderer, radius)
    , m_pController(controller) {
  m_position.z = 20.0f;
  m_rotationMatrix = Matrix::FromTranslationVector(m_position);
  m_translateMatrix = Matrix::FromTranslationVector(m_position);
}

Game::Sprite::~Sprite() {
}

void Game::Sprite::Update(const Engine::StepTimer& timer) {
  auto velocity = m_pController->GetVelocity();

  LOG(INFO) << "V: " << velocity.x << ", " << velocity.y << ", " << velocity.z;

  // compute move vector
  float elapsedSecond = timer.GetElapsedSeconds();
  auto moveVector = velocity * elapsedSecond;

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

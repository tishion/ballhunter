#include "Sprite.h"

#include <common/Logger.h>

using namespace Common;

Game::Sprite::Sprite(Engine::IRenderer::RefPtr renderer, Engine::InputContoller::RefPtr controller, float radius)
    : Engine::Sphere(renderer, radius)
    , m_pController(controller) {
  m_position.z = 20.0f;
  m_rotationMatrix = Matrix::Identity();
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

  // update accumulated translation matrix
  m_translateMatrix *= Matrix::FromTranslationVector(moveVector);

  // update accumulated self-rotation matrix
  LOG(INFO) << "V: " << velocity.x << ", " << velocity.z;

  // auto angle = Math::LengthHelper(moveVector) / m_radius;
  // if (fabsf(angle) > Common::F_MIN) {
  //   Float3 worldAxis = Float3::CrossProduct(velocity, -Math::kAxisY3f);
  //   Float3 localAxis = m_rotationMatrix * worldAxis;

  //  Float3 axis = localAxis;
  //  // calculate the rotation matrix
  //  m_rotationMatrix = Quaternion::FromAngleAxis(angle, axis).ToMatrix4();
  //}

  m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationX(moveVector.z / m_radius));
  m_rotationMatrix *= Matrix::FromRotationMatrix(Matrix::RotationZ(moveVector.x / m_radius));

  // compute the word matrix by translation and rotation
  m_worldMatrix = /*m_translateMatrix **/ m_rotationMatrix;

  // hit test
}

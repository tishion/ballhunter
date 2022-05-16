#include "GameObject.h"

namespace Engine {

GameObject::GameObject(Engine::IRenderer::RefPtr renderer)
    : m_pRenderer(renderer)
    , m_position(0.0f, 0.0f, 0.0f)
    , m_velocity(0.0f, 0.0f, 0.0f) {
  m_worldMatrix = Common::Matrix::FromTranslationVector(m_position);
}

GameObject::~GameObject() {
}

bool GameObject::Initialize() {
  return true;
}

void GameObject::Uninitialize() {
}

void GameObject::Render(void* context) {
}

Common::Float3 GameObject::GetPosition() {
  return m_position;
}

void GameObject::SetPosition(Common::Float3 position) {
  m_position = position;
}

Common::Float3 GameObject::GetVelocity() {
  return m_velocity;
}

void GameObject::SetVelocity(Common::Float3 velocity) {
  m_velocity = velocity;
}

} // namespace Engine

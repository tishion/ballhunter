#include "Sphere.h"

#include "windows/SphereImpl_DirectX.h"

namespace Engine {

Sphere::Sphere(Engine::IRenderer::RefPtr renderer, float radius)
    : Engine::GameObject(renderer)
    , m_radius(radius)
    , m_pImpl(new Implementation(*this)) {
  m_position.y = radius;
}

Sphere::~Sphere() {
}

bool Sphere::Initialize() {
  if (m_pImpl->Initialize()) {
    m_pRenderer->AddRenderable(shared_from_this());
    return true;
  }
  return false;
}

void Sphere::Uninitialize() {
  m_pImpl->Uninitialize();
}

void Sphere::Render(void* context) {
  m_pImpl->Render(context);
}

void Sphere::Update(const Engine::StepTimer& timer) {
}

void Sphere::Radius(float radius) {
  m_radius = radius;
}

float Sphere::Radius() {
  return m_radius;
}

} // namespace Engine

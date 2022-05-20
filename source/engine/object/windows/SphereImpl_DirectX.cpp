#include "SphereImpl_DirectX.h"

#include <engine/renderer/windows/DXRenderingContext.h>

Engine::Sphere::Implementation::Implementation(Engine::Sphere& obj)
    : m_obj(obj) {
  m_deviceResource = static_cast<DXDeviceResources*>(m_obj.m_pRenderer->GetDeviceResource());
}

Engine::Sphere::Implementation::~Implementation() {
}

bool Engine::Sphere::Implementation::Initialize() {
  if (!m_deviceResource) {
    return false;
  }

  m_shape = DirectX::GeometricPrimitive::CreateSphere(m_deviceResource->GetD3DDeviceContext(), m_obj.m_radius * 2.0f);
  return true;
}

void Engine::Sphere::Implementation::Uninitialize() {
  m_shape.reset();
}

void Engine::Sphere::Implementation::Render(void* context) {
  auto renderingContext = static_cast<DXRenderingContext*>(context);
  if (!renderingContext) {
    return;
  }

  DirectX::SimpleMath::Matrix worldMatrix(&m_obj.m_worldMatrix[0]);
  m_shape->Draw(worldMatrix, renderingContext->m_viewMatrix, renderingContext->m_projMatrix, DirectX::Colors::BurlyWood,
                nullptr, true);
}

#include "WallImpl_DirectX.h"

#include <vector>

#include <DirectXTK/DirectXHelpers.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>

#include <engine/renderer/windows/DXRenderingContext.h>

Engine::Wall::Implementation::Implementation(Engine::Wall& obj, IRenderer::RefPtr renderer)
    : m_obj(obj)
    , m_pRenderer(renderer) {
}

Engine::Wall::Implementation::~Implementation() {
}

bool Engine::Wall::Implementation::Initialize() {
  m_deviceResource = static_cast<DXDeviceResources*>(m_pRenderer->GetDeviceResource());
  if (!m_deviceResource) {
    return false;
  }

  return true;
}

void Engine::Wall::Implementation::Uninitialize() {
}

void Engine::Wall::Implementation::Render(void* context) {
  auto deviceContext = m_deviceResource->GetD3DDeviceContext();
  if (!deviceContext) {
    return;
  }

  auto renderingContext = static_cast<DXRenderingContext*>(context);
  if (!renderingContext) {
    return;
  }
}

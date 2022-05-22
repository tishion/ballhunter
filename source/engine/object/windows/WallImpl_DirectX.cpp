#include "WallImpl_DirectX.h"

#include <vector>

#include <DirectXTK/DirectXHelpers.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/WICTextureLoader.h>

#include <engine/renderer/windows/DXRenderingContext.h>

using namespace DirectX;

Engine::Wall::Implementation::Implementation(Engine::Wall& obj, IRenderer::RefPtr renderer)
    : m_obj(obj)
    , m_pRenderer(renderer) {
  m_deviceResource = static_cast<DXDeviceResources*>(renderer->GetDeviceResource());
}

Engine::Wall::Implementation::~Implementation() {
}

bool Engine::Wall::Implementation::Initialize() {
  m_deviceResource = static_cast<DXDeviceResources*>(m_pRenderer->GetDeviceResource());
  if (!m_deviceResource) {
    return false;
  }

  m_effect = std::make_unique<BasicEffect>(m_deviceResource->GetD3DDevice());

  DX::ThrowIfFailed(CreateWICTextureFromFile(m_deviceResource->GetD3DDevice(), L"assets\\wall.jpg", nullptr,
                                             m_texture.ReleaseAndGetAddressOf()));
  m_effect->SetTextureEnabled(true);
  m_effect->SetTexture(m_texture.Get());

  DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexType>(m_deviceResource->GetD3DDevice(), m_effect.get(),
                                                            m_inputLayout.ReleaseAndGetAddressOf()));

  m_pShape = GeometricPrimitive::CreateBox(m_deviceResource->GetD3DDeviceContext(), XMFLOAT3(100, 100, 100));

  return true;
}

void Engine::Wall::Implementation::Uninitialize() {
  m_inputLayout.Reset();
  m_texture.Reset();
  m_effect.reset();
  m_pShape.reset();
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

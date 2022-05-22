#include "SphereImpl_DirectX.h"

#include <DirectXTK/DirectXHelpers.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/WICTextureLoader.h>

#include <engine/renderer/windows/DXRenderingContext.h>

using namespace DirectX;

Engine::Sphere::Implementation::Implementation(Engine::Sphere& obj)
    : m_obj(obj) {
  m_deviceResource = static_cast<DXDeviceResources*>(m_obj.m_pRenderer->GetDeviceResource());
}

Engine::Sphere::Implementation::~Implementation() {
  return;
}

bool Engine::Sphere::Implementation::Initialize() {
  if (!m_deviceResource) {
    return false;
  }

  m_shape = GeometricPrimitive::CreateSphere(m_deviceResource->GetD3DDeviceContext(), m_obj.m_radius * 2.0f);

  m_effect = std::make_unique<DirectX::BasicEffect>(m_deviceResource->GetD3DDevice());

  // load texture
  if (!m_obj.m_textureImage.empty()) {
    auto wszPath = m_obj.m_textureImage.wstring();
    CreateWICTextureFromFile(m_deviceResource->GetD3DDevice(), wszPath.c_str(), nullptr,
                             m_texture.ReleaseAndGetAddressOf());
  }

  if (m_texture) {
    m_effect->SetTextureEnabled(true);
    m_effect->SetTexture(m_texture.Get());
  }
  m_effect->SetColorAndAlpha(Colors::BurlyWood);

  // initialize the input layout
  m_shape->CreateInputLayout(m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf());

  return true;
}

void Engine::Sphere::Implementation::Uninitialize() {
  m_texture.Reset();
  m_effect.reset();
  m_shape.reset();
}

void Engine::Sphere::Implementation::Render(void* context) {
  auto deviceContext = m_deviceResource->GetD3DDeviceContext();
  if (!deviceContext) {
    return;
  }

  auto renderingContext = static_cast<DXRenderingContext*>(context);
  if (!renderingContext) {
    return;
  }

  deviceContext->OMSetBlendState(renderingContext->m_states->Opaque(), nullptr, 0xFFFFFFFF);
  deviceContext->OMSetDepthStencilState(renderingContext->m_states->DepthNone(), 0);
  deviceContext->RSSetState(renderingContext->m_states->CullNone());

  SimpleMath::Matrix worldMatrix(&(m_obj.m_worldMatrix[0]));
  m_effect->SetWorld(worldMatrix);
  m_effect->SetView(renderingContext->m_viewMatrix);
  m_effect->SetProjection(renderingContext->m_projMatrix);

  m_shape->Draw(m_effect.get(), m_inputLayout.Get());
}

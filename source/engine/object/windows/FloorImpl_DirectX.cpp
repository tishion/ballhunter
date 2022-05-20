#include "FloorImpl_DirectX.h"

#include <vector>

#include <DirectXTK/DirectXHelpers.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/WICTextureLoader.h>

#include <engine/renderer/windows/DXRenderingContext.h>

Engine::Floor::Implementation::Implementation(Engine::Floor& obj, IRenderer::RefPtr renderer)
    : m_obj(obj)
    , m_pRenderer(renderer) {
  m_deviceResource = static_cast<DXDeviceResources*>(renderer->GetDeviceResource());
}

Engine::Floor::Implementation::~Implementation() {
}

bool Engine::Floor::Implementation::Initialize() {
  if (!m_deviceResource) {
    return false;
  }

  m_effect = std::make_unique<DirectX::BasicEffect>(m_deviceResource->GetD3DDevice());

  DX::ThrowIfFailed(DirectX::CreateWICTextureFromFile(m_deviceResource->GetD3DDevice(), L"ground.jpg", nullptr,
                                                      m_texture.ReleaseAndGetAddressOf()));
  m_effect->SetTextureEnabled(true);
  m_effect->SetTexture(m_texture.Get());

  DX::ThrowIfFailed(DirectX::CreateInputLayoutFromEffect<VertexType>(m_deviceResource->GetD3DDevice(), m_effect.get(),
                                                                     m_inputLayout.ReleaseAndGetAddressOf()));

  m_pBatch = std::make_unique<QuadBatch>(m_deviceResource->GetD3DDeviceContext());

  return true;
}

void Engine::Floor::Implementation::Uninitialize() {
  m_inputLayout.Reset();
  m_texture.Reset();
  m_effect.reset();
  m_pBatch.reset();
}

void Engine::Floor::Implementation::Render(void* context) {
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

  m_effect->SetView(renderingContext->m_viewMatrix);
  m_effect->SetProjection(renderingContext->m_projMatrix);
  m_effect->Apply(deviceContext);

  auto sampler = renderingContext->m_states->LinearClamp();
  deviceContext->PSSetSamplers(0, 1, &sampler);
  deviceContext->IASetInputLayout(m_inputLayout.Get());

  m_pBatch->Begin();
  DirectX::VertexPositionTexture v1(DirectX::SimpleMath::Vector3(&m_obj.m_pos.GetColumn(0)[0]),
                                    DirectX::SimpleMath::Vector2(0, 0));
  DirectX::VertexPositionTexture v2(DirectX::SimpleMath::Vector3(&m_obj.m_pos.GetColumn(1)[0]),
                                    DirectX::SimpleMath::Vector2(1, 0));
  DirectX::VertexPositionTexture v3(DirectX::SimpleMath::Vector3(&m_obj.m_pos.GetColumn(2)[0]),
                                    DirectX::SimpleMath::Vector2(1, 1));
  DirectX::VertexPositionTexture v4(DirectX::SimpleMath::Vector3(&m_obj.m_pos.GetColumn(3)[0]),
                                    DirectX::SimpleMath::Vector2(0, 1));
  m_pBatch->DrawQuad(v1, v2, v3, v4);
  m_pBatch->End();
}

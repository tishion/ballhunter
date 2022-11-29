#include "FloorImpl_DirectX.h"

#include <vector>

#include <DirectXTK/DirectXHelpers.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/WICTextureLoader.h>

#include <engine/renderer/windows/DXRenderingContext.h>

using namespace DirectX;

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

  m_pBatch = std::make_unique<QuadBatch>(m_deviceResource->GetD3DDeviceContext());

  m_effect = std::make_unique<BasicEffect>(m_deviceResource->GetD3DDevice());

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

  DX::ThrowIfFailed(DirectX::CreateInputLayoutFromEffect<VertexType>(m_deviceResource->GetD3DDevice(), m_effect.get(),
                                                                     m_inputLayout.ReleaseAndGetAddressOf()));

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

  // Input Assembler
  deviceContext->IASetInputLayout(m_inputLayout.Get());

  // Rasterizer
  deviceContext->RSSetState(renderingContext->m_states->CullNone());

  // Pixel Shader
  auto sampler = renderingContext->m_states->PointWrap();
  deviceContext->PSSetSamplers(0, 1, &sampler);

  // Output Merge
  deviceContext->OMSetBlendState(renderingContext->m_states->Opaque(), nullptr, 0xFFFFFFFF);
  deviceContext->OMSetDepthStencilState(renderingContext->m_states->DepthNone(), 0);

  SimpleMath::Matrix worldMatrix(&(m_obj.m_worldMatrix[0]));
  m_effect->SetWorld(worldMatrix);
  m_effect->SetView(renderingContext->m_viewMatrix);
  m_effect->SetProjection(renderingContext->m_projMatrix);
  m_effect->Apply(deviceContext);

  // Draw
  m_pBatch->Begin();
  VertexPositionTexture v1(SimpleMath::Vector3(&m_obj.m_pos.GetColumn(0)[0]), SimpleMath::Vector2(0, 0));
  VertexPositionTexture v2(SimpleMath::Vector3(&m_obj.m_pos.GetColumn(1)[0]), SimpleMath::Vector2(4, 0));
  VertexPositionTexture v3(SimpleMath::Vector3(&m_obj.m_pos.GetColumn(2)[0]), SimpleMath::Vector2(4, 4));
  VertexPositionTexture v4(SimpleMath::Vector3(&m_obj.m_pos.GetColumn(3)[0]), SimpleMath::Vector2(0, 4));
  m_pBatch->DrawQuad(v1, v2, v3, v4);
  m_pBatch->End();
}

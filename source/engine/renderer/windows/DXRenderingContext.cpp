#include "DXRenderingContext.h"

DXRenderingContext::DXRenderingContext(DXDeviceResources* deviceResources) {
  // create state
  m_states = std::make_unique<DirectX::CommonStates>(deviceResources->GetD3DDevice());

  // create effect (this include vertex shader and pixel shader)
  m_effect = std::make_unique<DirectX::BasicEffect>(deviceResources->GetD3DDevice());

  // create input layout
  DX::ThrowIfFailed(DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(
      deviceResources->GetD3DDevice(), m_effect.get(), m_inputLayout.ReleaseAndGetAddressOf()));
}

DXRenderingContext::~DXRenderingContext() {
  m_states.reset();
  m_effect.reset();
  m_inputLayout.Reset();
}

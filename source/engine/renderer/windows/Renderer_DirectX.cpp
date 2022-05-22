#include "Renderer_DirectX.h"

#include <SDL_syswm.h>

#include <DirectXTK/GeometricPrimitive.h>

Renderer_DirectX::Renderer_DirectX() {
}

Renderer_DirectX::~Renderer_DirectX() {
}

void* Renderer_DirectX::GetDeviceResource() {
  return m_pDeviceResources.get();
}

bool Renderer_DirectX::Initialize(SDL_Window* window,                          //
                                  Engine::Camera::RefPtr camera,               //
                                  Engine::InputContoller::RefPtr controller,   //
                                  Engine::RenderEventListener::RefPtr listener //
) {
  m_pDeviceResources = std::make_unique<DXDeviceResources>();

  m_pCamera = camera;
  m_pController = controller;
  m_renderEventListner = listener;

  // retrieve window handle
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(window, &wmInfo);

  // retrieve window size
  int w, h = 0;
  SDL_GetWindowSize(window, &w, &h);

  // set window to the D3D device resource
  m_pDeviceResources->SetWindow(wmInfo.info.win.window, w, h);
  m_pDeviceResources->SetListener(shared_from_this());

  CreateDeviceDependentResources();
  CreateWindowSizeDependentResources();

  return true;
}

void Renderer_DirectX::Uninitialize() {
  ReleaseDeviceDependentResources();

  m_pDeviceResources.reset();
}

void Renderer_DirectX::AddRenderable(Engine::IRenderable::RefPtr renderable) {
  m_renderableList.push_back(renderable);
}

void Renderer_DirectX::UpdateWindowSize(uint32_t width, uint32_t height) {
  m_pDeviceResources->SetOutputSize(width, height);
}

void Renderer_DirectX::CreateDeviceDependentResources() {
  m_pRenderingContext = std::make_unique<DXRenderingContext>(m_pDeviceResources.get());
}

void Renderer_DirectX::ReleaseDeviceDependentResources() {
  m_pDeviceResources.reset();
}

void Renderer_DirectX::CreateWindowSizeDependentResources() {
  // set projection matrix
  auto w = m_pDeviceResources->GetOutputWidth();
  auto h = m_pDeviceResources->GetOutputHeight();
  m_pCamera->SetProjParams(Common::PI_VAL / 4, float(w) / float(h), 0.1f, 1000.f);
}

void Renderer_DirectX::Render() {
  BeginScene();

  // update IA, RS and OM context
  m_pDeviceResources->GetD3DDeviceContext()->IASetInputLayout(m_pRenderingContext->m_inputLayout.Get());
  m_pDeviceResources->GetD3DDeviceContext()->RSSetState(m_pRenderingContext->m_states->CullNone());
  m_pDeviceResources->GetD3DDeviceContext()->OMSetDepthStencilState(m_pRenderingContext->m_states->DepthNone(), 0);
  m_pDeviceResources->GetD3DDeviceContext()->OMSetBlendState(m_pRenderingContext->m_states->Opaque(), nullptr,
                                                             0xFFFFFFFF);

  // retrieve current projection matrix from to the camera
  m_pRenderingContext->m_viewMatrix = DirectX::SimpleMath::Matrix(&(m_pCamera->View()[0]));
  m_pRenderingContext->m_projMatrix = DirectX::SimpleMath::Matrix(&(m_pCamera->Projection()[0]));

  // update shader context
  m_pRenderingContext->m_effect->SetView(m_pRenderingContext->m_viewMatrix);
  m_pRenderingContext->m_effect->SetProjection(m_pRenderingContext->m_projMatrix);
  m_pRenderingContext->m_effect->SetColorAndAlpha(DirectX::Colors::White);
  m_pRenderingContext->m_effect->Apply(m_pDeviceResources->GetD3DDeviceContext());

  // rendering all renderable objects
  for (auto& renderable : m_renderableList) {
    renderable->Render(m_pRenderingContext.get());
  }

  // // For Debug
  // std::unique_ptr<DirectX::GeometricPrimitive> shape;
  // shape = DirectX::GeometricPrimitive::CreateTeapot(m_pDeviceResources->GetD3DDeviceContext(), 20.0f);
  // shape->Draw(DirectX::SimpleMath::Matrix::CreateTranslation({0, 0, 0}), viewMatrix, projMatrix,
  //             DirectX::Colors::Yellow, nullptr, true);

  EndScene();
}

void Renderer_DirectX::BeginScene() {
  // Clear the views.
  m_pDeviceResources->GetD3DDeviceContext()->ClearRenderTargetView(m_pDeviceResources->GetBackBufferRenderTargetView(),
                                                                   DirectX::Colors::CornflowerBlue);

  m_pDeviceResources->GetD3DDeviceContext()->ClearDepthStencilView(m_pDeviceResources->GetDepthStencilView(),
                                                                   D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

  ID3D11RenderTargetView* const targets[1] = {m_pDeviceResources->GetBackBufferRenderTargetView()};

  m_pDeviceResources->GetD3DDeviceContext()->OMSetRenderTargets(1, targets, m_pDeviceResources->GetDepthStencilView());

  // Set the viewport.
  CD3D11_VIEWPORT viewport(0.0f, 0.0f,                                               //
                           static_cast<float>(m_pDeviceResources->GetOutputWidth()), //
                           static_cast<float>(m_pDeviceResources->GetOutputHeight()) //
  );

  m_pDeviceResources->GetD3DDeviceContext()->RSSetViewports(1, &viewport);
}

void Renderer_DirectX::EndScene() {
  m_pDeviceResources->Present();
}

void Renderer_DirectX::OnDeviceLost() {
  ReleaseDeviceDependentResources();

  auto l = m_renderEventListner.lock();
  if (l) {
    l->OnRenderDeviceLost();
  }
}

void Renderer_DirectX::OnDeviceRestored() {
  CreateDeviceDependentResources();
  CreateWindowSizeDependentResources();

  auto l = m_renderEventListner.lock();
  if (l) {
    l->OnRenderDeviceRestored();
  }
}

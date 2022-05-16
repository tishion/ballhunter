/*
 * File: Renderer_DirectX.h
 * Project: windows
 * Created: Wednesday, 18th May 2022 11:30:58 am
 * Author: Sheen Tian
 */
#ifndef RENDERER_DIRECTX_H
#define RENDERER_DIRECTX_H

#pragma once

#include <vector>

#include <SDL.h>

#include <engine/renderer/IRenderer.h>

#include "DXDeviceResources.h"
#include "DXRenderingContext.h"

class Renderer_DirectX
    : public Engine::IRenderer
    , public DXDeviceResources::IDeviceListener
    , public std::enable_shared_from_this<Renderer_DirectX> {

public:
  Renderer_DirectX();

  ~Renderer_DirectX();

  void* GetDeviceResource() override;

  bool Initialize(SDL_Window* window,                          //
                  Engine::Camera::RefPtr camera,               //
                  Engine::InputContoller::RefPtr controller,   //
                  Engine::RenderEventListener::RefPtr listener //
                  ) override;

  void Uninitialize() override;

  void AddRenderable(Engine::IRenderable::RefPtr renderable) override;

  void UpdateWindowSize(uint32_t width, uint32_t height) override;

  void CreateDeviceDependentResources() override;

  void ReleaseDeviceDependentResources() override;

  void CreateWindowSizeDependentResources() override;

  void Render() override;

protected:
  void BeginScene();
  void EndScene();

  // Device Listener
  void OnDeviceLost() override;
  void OnDeviceRestored() override;

private:
  std::unique_ptr<DXDeviceResources> m_pDeviceResources;

  std::unique_ptr<DXRenderingContext> m_pRenderingContext;

  DirectX::SimpleMath::Matrix m_worldMatrix;

  Engine::Camera::RefPtr m_pCamera = nullptr;
  Engine::InputContoller::RefPtr m_pController = nullptr;

  Engine::RenderEventListener::WeakPtr m_renderEventListner;

  std::vector<Engine::IRenderable::RefPtr> m_renderableList;
};

#endif

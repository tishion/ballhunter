/*
 * File: IRenderer.h
 * Project: renderer
 * Created: Tuesday, 17th May 2022 3:03:23 am
 * Author: Sheen Tian Shen
 */
#ifndef IRENDERER_H
#define IRENDERER_H

#pragma once

#include <SDL.h>

#include <common/SmartPointer.h>

#include <engine/input/InputContoller.h>
#include <engine/object/Camera.h>
#include <engine/renderer/IRenderable.h>

namespace Engine {

class RenderEventListener {
  DECLARE_SMART_POINTER(RenderEventListener);

public:
  virtual ~RenderEventListener(){};

public:
  virtual void OnRenderDeviceLost() = 0;
  virtual void OnRenderDeviceRestored() = 0;
};

class IRenderer {
  DECLARE_SMART_POINTER(IRenderer);

public:
  virtual ~IRenderer(){};

  virtual void* GetDeviceResource() = 0;

  virtual bool Initialize(SDL_Window* window,                          //
                          Engine::Camera::RefPtr camera,               //
                          Engine::InputContoller::RefPtr controller,   //
                          Engine::RenderEventListener::RefPtr listener //
                          ) = 0;

  virtual void Uninitialize() = 0;

  virtual void AddRenderable(Engine::IRenderable::RefPtr renderable) = 0;

  virtual void UpdateWindowSize(uint32_t width, uint32_t height) = 0;

  virtual void CreateDeviceDependentResources() = 0;

  virtual void ReleaseDeviceDependentResources() = 0;

  virtual void CreateWindowSizeDependentResources() = 0;

  virtual void Render() = 0;
};
} // namespace Engine

#endif

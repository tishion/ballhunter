/*
 * File: WallImpl_DirectX.h
 * Project: windows
 * Created: Thursday, 19th May 2022 10:57:12 pm
 * Author: Sheen Tian Shen
 */
#ifndef WALLIMPL_DIRECTX_H
#define WALLIMPL_DIRECTX_H

#pragma once

#include <DirectXTK/SimpleMath.h>

#include <engine/renderer/windows/DXDeviceResources.h>

#include "../Wall.h"

namespace Engine {

class Wall::Implementation {
public:
  Implementation(Engine::Wall& obj, IRenderer::RefPtr renderer);

  ~Implementation();

  bool Initialize();

  void Uninitialize();

  void Render(void* context);

public:
  Engine::Wall& m_obj;
  Engine::IRenderer::RefPtr m_pRenderer;

  DXDeviceResources* m_deviceResource = nullptr;
};
} // namespace Engine

#endif

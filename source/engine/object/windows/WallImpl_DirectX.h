/*
 * File: WallImpl_DirectX.h
 * Project: windows
 * Created: Thursday, 19th May 2022 10:57:12 pm
 * Author: Sheen Tian Shen
 */
#ifndef WALLIMPL_DIRECTX_H
#define WALLIMPL_DIRECTX_H

#pragma once

#include <DirectXTK/CommonStates.h>
#include <DirectXTK/Effects.h>
#include <DirectXTK/GeometricPrimitive.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>

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

  typedef DirectX::VertexPositionTexture VertexType;

  std::unique_ptr<DirectX::GeometricPrimitive> m_pShape;
  std::unique_ptr<DirectX::BasicEffect> m_effect;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};
} // namespace Engine

#endif

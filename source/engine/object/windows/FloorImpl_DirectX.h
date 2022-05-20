/*
 * File: FloorImpl_DirectX.h
 * Project: windows
 * Created: Thursday, 19th May 2022 12:29:40 am
 * Author: Sheen Tian Shen
 */
#ifndef FLOORIMPL_DIRECTX_H
#define FLOORIMPL_DIRECTX_H

#pragma once

#include <wrl.h>
#include <wrl/client.h>

#include <d3d11.h>

#include <DirectXTK/CommonStates.h>
#include <DirectXTK/Effects.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/VertexTypes.h>

#include <engine/renderer/windows/DXDeviceResources.h>

#include "../Floor.h"

namespace Engine {

class Floor::Implementation {
public:
  Implementation(Engine::Floor& obj, IRenderer::RefPtr renderer);

  ~Implementation();

  bool Initialize();

  void Uninitialize();

  void Render(void* context);

public:
  Engine::Floor& m_obj;
  Engine::IRenderer::RefPtr m_pRenderer;

  DXDeviceResources* m_deviceResource = nullptr;

  typedef DirectX::VertexPositionTexture VertexType;
  typedef DirectX::PrimitiveBatch<VertexType> QuadBatch;

  std::unique_ptr<QuadBatch> m_pBatch;
  std::unique_ptr<DirectX::BasicEffect> m_effect;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};
} // namespace Engine

#endif

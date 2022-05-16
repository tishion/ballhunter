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

#include <DirectXTK/SimpleMath.h>

#include <engine/renderer/windows/DXDeviceResources.h>

#include "../Floor.h"

namespace Engine {

class Floor::Implementation {
public:
  Implementation(IRenderer::RefPtr renderer, int x, int z, uint32_t width, uint32_t height);

  ~Implementation();

  bool Initialize();

  void Uninitialize();

  void Render(void* context);

private:
  bool CreateBuffers();

  void ReleaseBuffers();

public:
  Engine::IRenderer::RefPtr m_pRenderer;
  DXDeviceResources* m_deviceResource = nullptr;

  Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

  DirectX::SimpleMath::Matrix m_worldMatrix;
  int m_x = 0;
  int m_z = 0;
  int m_width = 100;
  int m_height = 100;
  int m_vertexCount = 0;
  int m_indexCount = 0;
};
} // namespace Engine

#endif

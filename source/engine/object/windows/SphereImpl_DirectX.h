/*
 * File: SphereImpl_DirectX.h
 * Project: windows
 * Created: Thursday, 19th May 2022 6:27:32 pm
 * Author: Sheen Tian
 */
#ifndef SPHEREIMPL_DIRECTX_H
#define SPHEREIMPL_DIRECTX_H

#pragma once

#include <DirectXTK/CommonStates.h>
#include <DirectXTK/Effects.h>
#include <DirectXTK/GeometricPrimitive.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>

#include <engine/renderer/windows/DXDeviceResources.h>

#include "../Sphere.h"

namespace Engine {

class Sphere::Implementation {
public:
  Implementation(Engine::Sphere& obj);

  ~Implementation();

  bool Initialize();

  void Uninitialize();

  void Render(void* context);

public:
  Engine::Sphere& m_obj;

  DXDeviceResources* m_deviceResource = nullptr;

  std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
  std::unique_ptr<DirectX::BasicEffect> m_effect;

  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};
} // namespace Engine

#endif

﻿/*
 * File: SphereImpl_DirectX.h
 * Project: windows
 * Created: Thursday, 19th May 2022 6:27:32 pm
 * Author: Sheen Tian
 */
#ifndef SPHEREIMPL_DIRECTX_H
#define SPHEREIMPL_DIRECTX_H

#pragma once

#include <DirectXTK/GeometricPrimitive.h>
#include <DirectXTK/SimpleMath.h>

#include <engine/renderer/windows/DXDeviceResources.h>

#include "../Sphere.h"

namespace Engine {

class Sphere::Implementation {
public:
  Implementation(Engine::Sphere& host);

  ~Implementation();

  bool Initialize();

  void Uninitialize();

  void Render(void* context);

public:
  Engine::Sphere& m_obj;

  DXDeviceResources* m_deviceResource = nullptr;
  std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
};
} // namespace Engine

#endif

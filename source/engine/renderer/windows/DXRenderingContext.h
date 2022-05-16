/*
 * File: DXRenderingContext.h
 * Project: windows
 * Created: Thursday, 19th May 2022 9:26:54 pm
 * Author: Sheen Tian Shen
 */
#ifndef DXRENDERINGCONTEXT_H
#define DXRENDERINGCONTEXT_H

#pragma once

#include <memory>

#include <wrl/client.h>

#include <DirectXTK/CommonStates.h>
#include <DirectXTK/DirectXHelpers.h>
#include <DirectXTK/Effects.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>

#include "DXDeviceResources.h"

class DXRenderingContext {
public:
  DXRenderingContext(DXDeviceResources* deviceResources);

  ~DXRenderingContext();

  DirectX::SimpleMath::Matrix m_viewMatrix;

  DirectX::SimpleMath::Matrix m_projMatrix;

  std::unique_ptr<DirectX::CommonStates> m_states;

  std::unique_ptr<DirectX::BasicEffect> m_effect;

  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

#endif

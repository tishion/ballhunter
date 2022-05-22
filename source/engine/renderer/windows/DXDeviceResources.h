/*
 * File: DXDeviceResource.h
 * Project: windows
 * Created: Tuesday, 17th May 2022 10:45:53 am
 * Author: Sheen Tian
 */
#ifndef D3DDEVICERESOURCE_H
#define D3DDEVICERESOURCE_H

#pragma once

#include <winsdkver.h>

#include <sdkddkver.h>

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#include <atlstr.h>

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl.h>
#include <wrl/client.h>

#include <DirectXColors.h>
#include <DirectXMath.h>
#include <d2d1_3.h>
#include <d3d11_3.h>
#include <dwrite_3.h>
#include <dxgi1_2.h>
#include <wincodec.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <common/SmartPointer.h>

namespace DX {
inline void ThrowIfFailed(HRESULT hr) {
  if (FAILED(hr)) {
    // Set a breakpoint on this line to catch DirectX API errors
    throw std::exception();
  }
}
} // namespace DX

class DXDeviceResources {
public:
  class IDeviceListener {
    DECLARE_SMART_POINTER(DXDeviceResources::IDeviceListener);

  public:
    virtual void OnDeviceLost() = 0;
    virtual void OnDeviceRestored() = 0;
  };

public:
  DXDeviceResources();
  ~DXDeviceResources();

  void SetListener(DXDeviceResources::IDeviceListener::RefPtr listener);

  void SetWindow(HWND hWnd, UINT width, UINT height);

  void SetOutputSize(UINT width, UINT height);

  void HandleDeviceLost();

  void Trim();

  void Present();

  UINT GetOutputWidth() const {
    return m_outputWidth;
  }

  UINT GetOutputHeight() const {
    return m_outputHeight;
  }

  // D3D Accessors.
  ID3D11Device3* GetD3DDevice() const {
    return m_d3dDevice.Get();
  }

  ID3D11DeviceContext3* GetD3DDeviceContext() const {
    return m_d3dContext.Get();
  }

  IDXGISwapChain1* GetSwapChain() const {
    return m_swapChain.Get();
  }

  D3D_FEATURE_LEVEL GetDeviceFeatureLevel() const {
    return m_d3dFeatureLevel;
  }

  ID3D11RenderTargetView* GetBackBufferRenderTargetView() const {
    return m_d3dRenderTargetView.Get();
  }

  ID3D11DepthStencilView* GetDepthStencilView() const {
    return m_d3dDepthStencilView.Get();
  }

  D3D11_VIEWPORT GetScreenViewport() const {
    return m_screenViewport;
  }

  // D2D Accessors.
  ID2D1Factory3* GetD2DFactory() const {
    return m_d2dFactory.Get();
  }

  ID2D1Device2* GetD2DDevice() const {
    return m_d2dDevice.Get();
  }

  ID2D1DeviceContext2* GetD2DDeviceContext() const {
    return m_d2dContext.Get();
  }

  ID2D1Bitmap1* GetD2DTargetBitmap() const {
    return m_d2dTargetBitmap.Get();
  }

  ID2D1Bitmap1* GetD2DTargetBitmapRight() const {
    return m_d2dTargetBitmapRight.Get();
  }

  IDWriteFactory3* GetDWriteFactory() const {
    return m_dwriteFactory.Get();
  }

protected:
  void CreateDeviceIndependentResources();
  void CreateDeviceResources();
  void CreateWindowSizeDependentResources();

private:
  // Device resources.
  HWND m_window;
  UINT m_outputWidth;
  UINT m_outputHeight;

  // Cached device properties.
  D3D_FEATURE_LEVEL m_d3dFeatureLevel;

  // Direct3D objects.
  Microsoft::WRL::ComPtr<ID3D11Device3> m_d3dDevice;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext3> m_d3dContext;
  Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;

  // Direct3D rendering objects. Required for 3D.
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_d3dDepthStencilView;
  D3D11_VIEWPORT m_screenViewport;

  // Direct2D drawing components.
  Microsoft::WRL::ComPtr<ID2D1Factory3> m_d2dFactory;
  Microsoft::WRL::ComPtr<ID2D1Device2> m_d2dDevice;
  Microsoft::WRL::ComPtr<ID2D1DeviceContext2> m_d2dContext;
  Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dTargetBitmap;
  Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dTargetBitmapRight;

  // DirectWrite drawing components.
  Microsoft::WRL::ComPtr<IDWriteFactory3> m_dwriteFactory;

  //
  IDeviceListener::WeakPtr m_deviceListener;
};

#endif

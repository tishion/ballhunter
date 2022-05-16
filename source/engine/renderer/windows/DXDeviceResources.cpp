#include "DXDeviceResources.h"

using namespace Microsoft::WRL;

DXDeviceResources::DXDeviceResources()
    : m_screenViewport()
    , m_d3dFeatureLevel(D3D_FEATURE_LEVEL_9_1) {
  CreateDeviceIndependentResources();
  CreateDeviceResources();
}

DXDeviceResources::~DXDeviceResources() {
}

void DXDeviceResources::SetListener(IDeviceListener::RefPtr listener) {
  m_deviceListener = listener;
}

void DXDeviceResources::SetWindow(HWND hWnd, UINT width, UINT height) {
  m_window = hWnd;
  m_outputHeight = height;
  m_outputWidth = width;
  CreateWindowSizeDependentResources();
}

void DXDeviceResources::SetOutputSize(UINT width, UINT height) {
  m_outputHeight = height;
  m_outputWidth = width;

  CreateWindowSizeDependentResources();
}

void DXDeviceResources::HandleDeviceLost() {
  m_swapChain = nullptr;

  auto listener = m_deviceListener.lock();
  if (listener != nullptr) {
    listener->OnDeviceLost();
  }

  // Make sure the rendering state has been released.
  m_d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
  m_d3dDepthStencilView = nullptr;
  m_d3dRenderTargetView = nullptr;

  m_d2dContext->SetTarget(nullptr);
  m_d2dTargetBitmap = nullptr;
  m_d2dTargetBitmapRight = nullptr;
  m_d2dContext = nullptr;
  m_d2dDevice = nullptr;

  m_d3dContext->Flush();

  CreateDeviceResources();
  CreateWindowSizeDependentResources();

  if (listener != nullptr) {
    listener->OnDeviceRestored();
  }
}

void DXDeviceResources::Trim() {
  ComPtr<IDXGIDevice3> dxgiDevice;
  m_d3dDevice.As(&dxgiDevice);

  dxgiDevice->Trim();
}

void DXDeviceResources::Present() {
  HRESULT hr = m_swapChain->Present(1, 0);

  m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());
  m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());

  if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
    HandleDeviceLost();
  } else {
    DX::ThrowIfFailed(hr);
  }
}

void DXDeviceResources::CreateDeviceIndependentResources() {
  // Initialize Direct2D resources.
  D2D1_FACTORY_OPTIONS options;
  ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
  // If the project is in a debug build, enable Direct2D debugging via SDK Layers.
  options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

  // Initialize the Direct2D Factory.
  DX::ThrowIfFailed(
      D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory2), &options, &m_d2dFactory));

  // Initialize the DirectWrite Factory.
  DX::ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2), &m_dwriteFactory));
}

void DXDeviceResources::CreateDeviceResources() {
  // This flag adds support for surfaces with a different color channel ordering
  // than the API default. It is required for compatibility with Direct2D.
  UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

  // This array defines the set of DirectX hardware feature levels this app will support.
  // Note the ordering should be preserved.
  // Don't forget to declare your application's minimum required feature level in its
  // description.  All applications are assumed to support 9.1 unless otherwise stated.
  D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
                                       D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3,  D3D_FEATURE_LEVEL_9_2,
                                       D3D_FEATURE_LEVEL_9_1};

  // Create the Direct3D 11 API device object and a corresponding context.
  ComPtr<ID3D11Device> device;
  ComPtr<ID3D11DeviceContext> context;

  HRESULT hr = D3D11CreateDevice(nullptr,                  // Specify nullptr to use the default adapter.
                                 D3D_DRIVER_TYPE_HARDWARE, // Create a device using the hardware graphics driver.
                                 0,                        // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
                                 creationFlags,            // Set debug and Direct2D compatibility flags.
                                 featureLevels,            // List of feature levels this app can support.
                                 ARRAYSIZE(featureLevels), // Size of the list above.
                                 D3D11_SDK_VERSION,  // Always set this to D3D11_SDK_VERSION for Windows Runtime apps.
                                 &device,            // Returns the Direct3D device created.
                                 &m_d3dFeatureLevel, // Returns feature level of device created.
                                 &context            // Returns the device immediate context.
  );

  if (FAILED(hr)) {
    // If the initialization fails, fall back to the WARP device.
    // For more information on WARP, see:
    // http://go.microsoft.com/fwlink/?LinkId=286690
    DX::ThrowIfFailed(D3D11CreateDevice(nullptr,
                                        D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
                                        0, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
                                        &device, &m_d3dFeatureLevel, &context));
  }

  // Store pointers to the Direct3D 11.1 API device and immediate context.
  DX::ThrowIfFailed(device.As(&m_d3dDevice));

  DX::ThrowIfFailed(context.As(&m_d3dContext));

  // Create the Direct2D device object and a corresponding context.
  ComPtr<IDXGIDevice3> dxgiDevice;
  DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

  DX::ThrowIfFailed(m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice));

  DX::ThrowIfFailed(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext));
}

void DXDeviceResources::CreateWindowSizeDependentResources() {
  // Clear the previous window size specific context.
  ID3D11RenderTargetView* nullViews[] = {nullptr};
  m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
  m_d3dRenderTargetView = nullptr;
  m_d2dContext->SetTarget(nullptr);
  m_d2dTargetBitmap = nullptr;
  m_d2dTargetBitmapRight = nullptr;
  m_d3dDepthStencilView = nullptr;
  m_d3dContext->Flush();

  if (m_swapChain != nullptr) {
    // If the swap chain already exists, resize it.
    HRESULT hr = m_swapChain->ResizeBuffers(2, // Double-buffered swap chain.
                                            m_outputWidth, m_outputHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
      // If the device was removed for any reason, a new device and swap chain will need to be
      // created.
      HandleDeviceLost();

      // Everything is set up now. Do not continue execution of this method. HandleDeviceLost will
      // reenter this method and correctly set up the new device.
      return;
    } else {
      DX::ThrowIfFailed(hr);
    }
  } else {
    // Otherwise, create a new one using the same adapter as the existing Direct3D device.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};

    swapChainDesc.Width = m_outputWidth;                         // Match the size of the window.
    swapChainDesc.Height = m_outputHeight;                       //
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // This is the most common swap chain format.
    swapChainDesc.Stereo = FALSE;                                //
    swapChainDesc.SampleDesc.Count = 1;                          // Don't use multi-sampling.
    swapChainDesc.SampleDesc.Quality = 0;                        //
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //
    swapChainDesc.BufferCount = 2;                               // Use double-buffering to minimize latency.
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
    swapChainDesc.Flags = 0;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

    // This sequence obtains the DXGI factory that was used to create the Direct3D device above.
    ComPtr<IDXGIDevice3> dxgiDevice;
    DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

    ComPtr<IDXGIAdapter> dxgiAdapter;
    DX::ThrowIfFailed(dxgiDevice->GetAdapter(&dxgiAdapter));

    ComPtr<IDXGIFactory3> dxgiFactory;
    DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
    fsSwapChainDesc.Windowed = TRUE;

    DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(m_d3dDevice.Get(), m_window, &swapChainDesc, &fsSwapChainDesc,
                                                          nullptr, m_swapChain.ReleaseAndGetAddressOf()));

    // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
    // ensures that the application will only render after each VSync, minimizing power consumption.
    DX::ThrowIfFailed(dxgiDevice->SetMaximumFrameLatency(1));
  }

  // Create a render target view of the swap chain back buffer.
  ComPtr<ID3D11Texture2D> backBuffer;
  DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));

  DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_d3dRenderTargetView));

  // Create a depth stencil view for use with 3D rendering if needed.
  CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, m_outputWidth, m_outputHeight,
                                         1, // This depth stencil view has only one texture.
                                         1, // Use a single mipmap level.
                                         D3D11_BIND_DEPTH_STENCIL);

  ComPtr<ID3D11Texture2D> depthStencil;
  DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil));

  CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
  DX::ThrowIfFailed(
      m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, &m_d3dDepthStencilView));

  // Set the 3D rendering viewport to target the entire window.
  m_screenViewport = CD3D11_VIEWPORT(0.0f, 0.0f,                        //
                                     static_cast<FLOAT>(m_outputWidth), //
                                     static_cast<FLOAT>(m_outputHeight) //
  );

  m_d3dContext->RSSetViewports(1, &m_screenViewport);

  // Create a Direct2D target bitmap associated with the
  // swap chain back buffer and set it as the current target.
  D2D1_BITMAP_PROPERTIES1 bitmapProperties =
      D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                              D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

  ComPtr<IDXGIResource1> dxgiBackBuffer;
  DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)));

  ComPtr<IDXGISurface2> dxgiSurface;
  DX::ThrowIfFailed(dxgiBackBuffer->CreateSubresourceSurface(0, &dxgiSurface));

  DX::ThrowIfFailed(
      m_d2dContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProperties, &m_d2dTargetBitmap));

  m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());

  // Gray scale text anti-aliasing is recommended for all Windows Store apps.
  m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

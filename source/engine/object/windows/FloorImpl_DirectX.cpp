#include "FloorImpl_DirectX.h"

#include <vector>

#include <DirectXTK/DirectXHelpers.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/VertexTypes.h>

#include <engine/renderer/windows/DXRenderingContext.h>

Engine::Floor::Implementation::Implementation(IRenderer::RefPtr renderer, int x, int z, uint32_t width, uint32_t height)
    : m_pRenderer(renderer)
    , m_x(x)
    , m_z(z)
    , m_width(width)
    , m_height(height) {
  m_worldMatrix.Translation(DirectX::SimpleMath::Vector3::Zero);
}

Engine::Floor::Implementation::~Implementation() {
}

bool Engine::Floor::Implementation::Initialize() {
  m_deviceResource = static_cast<DXDeviceResources*>(m_pRenderer->GetDeviceResource());
  if (!m_deviceResource) {
    return false;
  }

  return CreateBuffers();
}

void Engine::Floor::Implementation::Uninitialize() {
  ReleaseBuffers();
}

void Engine::Floor::Implementation::Render(void* context) {
  auto deviceContext = m_deviceResource->GetD3DDeviceContext();
  if (!deviceContext) {
    return;
  }

  auto renderingContext = static_cast<DXRenderingContext*>(context);
  if (!renderingContext) {
    return;
  }

  renderingContext->m_effect->SetWorld(m_worldMatrix);
  renderingContext->m_effect->SetColorAndAlpha(DirectX::Colors::Gray);
  renderingContext->m_effect->Apply(deviceContext);

  unsigned int stride = sizeof(DirectX::VertexPositionColor);
  unsigned int offset = 0;

  deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
  deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
  deviceContext->DrawIndexed(m_indexCount, 0, 0);
}

bool Engine::Floor::Implementation::CreateBuffers() {
  if (!m_deviceResource) {
    return false;
  }

  auto device = m_deviceResource->GetD3DDevice();
  if (!device) {
    return false;
  }

  m_vertexCount = (m_width - 1) * (m_height - 1) * 8;
  m_indexCount = m_vertexCount;

  int index = 0;
  float positionX = 0;
  float positionZ = 0;

  std::vector<DirectX::VertexPositionColor> vertices(m_vertexCount);
  std::vector<UINT32> indices(m_indexCount);

  // Load the vertex and index arrays with the terrain data.
  for (int z = m_z; z < (m_z + m_height - 1); z++) {
    for (int x = m_x; x < (m_x + m_width - 1); x++) {
      // LINE 1
      // Upper left.
      positionX = (float)x;
      positionZ = (float)(z + 1);

      vertices[index].position = DirectX::SimpleMath::Vector3(positionX, 0.0f, positionZ);
      vertices[index].color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
      indices[index] = index;
      index++;

      // Upper right.
      positionX = (float)(x + 1);
      positionZ = (float)(z + 1);

      vertices[index].position = DirectX::SimpleMath::Vector3(positionX, 0.0f, positionZ);
      vertices[index].color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
      indices[index] = index;
      index++;

      // LINE 2
      // Upper right.
      positionX = (float)(x + 1);
      positionZ = (float)(z + 1);

      vertices[index].position = DirectX::SimpleMath::Vector3(positionX, 0.0f, positionZ);
      vertices[index].color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
      indices[index] = index;
      index++;

      // Bottom right.
      positionX = (float)(x + 1);
      positionZ = (float)z;

      vertices[index].position = DirectX::SimpleMath::Vector3(positionX, 0.0f, positionZ);
      vertices[index].color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
      indices[index] = index;
      index++;

      // LINE 3
      // Bottom right.
      positionX = (float)(x + 1);
      positionZ = (float)z;

      vertices[index].position = DirectX::SimpleMath::Vector3(positionX, 0.0f, positionZ);
      vertices[index].color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
      indices[index] = index;
      index++;

      // Bottom left.
      positionX = (float)x;
      positionZ = (float)z;

      vertices[index].position = DirectX::SimpleMath::Vector3(positionX, 0.0f, positionZ);
      vertices[index].color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
      indices[index] = index;
      index++;

      // LINE 4
      // Bottom left.
      positionX = (float)x;
      positionZ = (float)z;

      vertices[index].position = DirectX::SimpleMath::Vector3(positionX, 0.0f, positionZ);
      vertices[index].color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
      indices[index] = index;
      index++;

      // Upper left.
      positionX = (float)x;
      positionZ = (float)(z + 1);

      vertices[index].position = DirectX::SimpleMath::Vector3(positionX, 0.0f, positionZ);
      vertices[index].color = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
      indices[index] = index;
      index++;
    }
  }

  HRESULT result = S_FALSE;

  // Set up the description of the static vertex buffer.
  D3D11_BUFFER_DESC vertexBufferDesc;
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(DirectX::VertexPositionColor) * m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the vertex data.
  D3D11_SUBRESOURCE_DATA vertexData;
  vertexData.pSysMem = vertices.data();
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  // Now create the vertex buffer.
  Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
  result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
  if (FAILED(result)) {
    return false;
  }

  // Set up the description of the static index buffer.
  D3D11_BUFFER_DESC indexBufferDesc;
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the index data.
  D3D11_SUBRESOURCE_DATA indexData;
  indexData.pSysMem = indices.data();
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  // Create the index buffer.
  Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
  result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
  if (FAILED(result)) {
    return false;
  }

  m_vertexBuffer = vertexBuffer;
  m_indexBuffer = indexBuffer;

  return true;
}

void Engine::Floor::Implementation::ReleaseBuffers() {
  m_indexBuffer.Reset();
  m_vertexBuffer.Reset();
}

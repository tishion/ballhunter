#include "Floor.h"

#include "windows/FloorImpl_DirectX.h"

namespace Engine {

Floor::Floor(IRenderer::RefPtr renderer, int x, int z, uint32_t width, uint32_t height)
    : m_pImpl(new Implementation(renderer, x, z, width, height)) {
}

Floor::~Floor() {
}

bool Floor::Initialize() {
  if (m_pImpl->Initialize()) {
    m_pImpl->m_pRenderer->AddRenderable(shared_from_this());
    return true;
  }
  return false;
}

void Floor::Uninitialize() {
  m_pImpl->Uninitialize();
}

void Floor::Render(void* context) {
  m_pImpl->Render(context);
}

} // namespace Engine

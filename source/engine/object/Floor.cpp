#include "Floor.h"

#include "windows/FloorImpl_DirectX.h"

namespace Engine {

Floor::Floor(IRenderer::RefPtr renderer, Common::Float4x4 pos)
    : Engine::GameObject(renderer)
    , m_pImpl(new Implementation(*this, renderer))
    , m_pos(pos) {
  m_textureImage = "assets\\ground.jpg";
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

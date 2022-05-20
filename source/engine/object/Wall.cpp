#include "Wall.h"

#include "windows/WallImpl_DirectX.h"

Engine::Wall::Wall(IRenderer::RefPtr renderer)
    : m_pImpl(new Implementation(*this, renderer)) {
}

Engine::Wall::~Wall() {
}

bool Engine::Wall::Initialize() {
  if (m_pImpl->Initialize()) {
    m_pImpl->m_pRenderer->AddRenderable(shared_from_this());
    return true;
  }
  return false;
}

void Engine::Wall::Uninitialize() {
  m_pImpl->Uninitialize();
}

void Engine::Wall::Render(void* context) {
  m_pImpl->Render(context);
}

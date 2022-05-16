#include "WallImpl_DirectX.h"

Engine::Wall::Implementation::Implementation(IRenderer::RefPtr renderer)
    : m_pRenderer(renderer) {
}

Engine::Wall::Implementation::~Implementation() {
}

bool Engine::Wall::Implementation::Initialize() {
  return true;
}

void Engine::Wall::Implementation::Uninitialize() {
}

void Engine::Wall::Implementation::Render(void* context) {
}

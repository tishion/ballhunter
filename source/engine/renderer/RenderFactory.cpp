#include "RenderFactory.h"

#include "windows/Renderer_DirectX.h"

namespace Engine { namespace RendererFactory {

IRenderer::RefPtr CreateRenderer() {
  return std::make_shared<Renderer_DirectX>();
}
}} // namespace Engine::RendererFactory

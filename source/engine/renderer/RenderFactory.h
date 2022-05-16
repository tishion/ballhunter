/*
 * File: RenderFactory.h
 * Project: renderer
 * Created: Tuesday, 17th May 2022 2:58:36 pm
 * Author: Sheen Tian
 */
#ifndef RENDERERFACTORY_H
#define RENDERERFACTORY_H

#pragma once

#include <engine/renderer/IRenderer.h>

namespace Engine { namespace RendererFactory {
//
IRenderer::RefPtr CreateRenderer();
}; } // namespace Engine::RendererFactory
#endif

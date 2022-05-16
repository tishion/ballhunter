/*
 * File: Wall.h
 * Project: object
 * Created: Thursday, 19th May 2022 10:53:12 pm
 * Author: Sheen Tian Shen
 */
#ifndef WALL_H
#define WALL_H

#pragma once

#include <common/BasicTypes.h>

#include <engine/renderer/IRenderable.h>
#include <engine/renderer/IRenderer.h>

namespace Engine {

class Wall : public IRenderable {
  DECLARE_SMART_POINTER(Wall);

public:
  Wall(IRenderer::RefPtr renderer);

  ~Wall();

  bool Initialize() override;

  void Uninitialize() override;

  void Render(void* context) override;

private:
  class Implementation;
  std::unique_ptr<Implementation> m_pImpl;

private:
};

} // namespace Engine
#endif

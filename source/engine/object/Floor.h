﻿/*
 * File: Floor.h
 * Project: object
 * Created: Wednesday, 18th May 2022 11:40:37 pm
 * Author: Sheen Tian Shen
 */
#ifndef FLOOR_H
#define FLOOR_H

#pragma once

#include <common/BasicTypes.h>

#include <engine/renderer/IRenderable.h>
#include <engine/renderer/IRenderer.h>

namespace Engine {

class Floor : public IRenderable {
  DECLARE_SMART_POINTER(Floor);

public:
  Floor(IRenderer::RefPtr renderer, int x, int z, uint32_t width, uint32_t height);

  ~Floor();

  bool Initialize() override;

  void Uninitialize() override;

  void Render(void* context) override;

private:
  class Implementation;
  std::unique_ptr<Implementation> m_pImpl;
};

} // namespace Engine

#endif
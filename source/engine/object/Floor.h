/*
 * File: Floor.h
 * Project: object
 * Created: Wednesday, 18th May 2022 11:40:37 pm
 * Author: Sheen Tian Shen
 */
#ifndef FLOOR_H
#define FLOOR_H

#pragma once

#include <filesystem>

#include <common/BasicTypes.h>

#include <engine/object/GameObject.h>

namespace Engine {

class Floor : public GameObject {
  DECLARE_SMART_POINTER(Floor);

public:
  Floor(IRenderer::RefPtr renderer, Common::Float4x4 pos);

  ~Floor();

  bool Initialize() override;

  void Uninitialize() override;

  void Render(void* context) override;

private:
  class Implementation;
  std::unique_ptr<Implementation> m_pImpl;

  Common::Float4x4 m_pos;

  std::filesystem::path m_textureImage;
};

} // namespace Engine

#endif

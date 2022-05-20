/*
 * File: Sprite.h
 * Project: game
 * Created: Friday, 20th May 2022 11:50:15 pm
 * Author: Sheen Tian Shen
 */
#ifndef SPRITE_H
#define SPRITE_H

#pragma once

#include <common/SmartPointer.h>

#include <engine/input/InputContoller.h>
#include <engine/object/Sphere.h>

namespace Game {
class Sprite : public Engine::Sphere {
  DECLARE_SMART_POINTER(Sprite);

public:
  Sprite(Engine::IRenderer::RefPtr renderer, Engine::InputContoller::RefPtr controller, float radius);

  ~Sprite();

  void Update(const Engine::StepTimer& timer) override;

private:
  Engine::InputContoller::RefPtr m_pController;

  // accumulated translation matrix
  Common::Matrix m_translateMatrix;

  // accumulated rotation matrix
  Common::Matrix m_rotationMatrix;
};
} // namespace Game
#endif

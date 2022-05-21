/*
 * File: Ball.h
 * Project: game
 * Created: Friday, 20th May 2022 11:25:56 am
 * Author: Sheen Tian
 */
#ifndef BALL_H
#define BALL_H

#pragma once

#include <common/SmartPointer.h>

#include <engine/object/Sphere.h>

namespace Game {
class Ball : public Engine::Sphere {
  DECLARE_SMART_POINTER(Ball);

public:
  Ball(Engine::IRenderer::RefPtr renderer, float radius);

  ~Ball();

  void SetBounds(Common::Float4 rect);

  void Update(const Engine::StepTimer& timer) override;

protected:
  void HandleCollision(Common::Float3 pos);

private:
  // accumulated translation matrix
  Common::Matrix m_translateMatrix;

  // accumulated rotation matrix
  Common::Matrix m_rotationMatrix;

  Common::Float4 m_bound;

  Common::Float3 m_moveVector;
};
} // namespace Game

#endif

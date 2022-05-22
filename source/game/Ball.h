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
#include <engine/physics/Physics.h>

namespace Game {
class Ball : public Engine::Sphere {
  DECLARE_SMART_POINTER(Ball);

public:
  Ball(Engine::IRenderer::RefPtr renderer, Engine::Physics::RefPtr physics, float radius);

  ~Ball();

  void SetPosition(Common::Float3 position);

  void SetBounds(Common::Float4 rect);

  void Update(const Engine::StepTimer& timer) override;

protected:
  // this could be moved to the physics engine
  void HandleWorldBoundaryCollision(Common::Float3 pos);

protected:
  // physics parameters
  Engine::Physics::RefPtr m_pPhysics;
  reactphysics3d::CollisionBody* m_pCollisionBody;
  reactphysics3d::SphereShape* m_pCollisionShape;
  reactphysics3d::Collider* m_pCollider;

  // accumulated translation matrix
  Common::Matrix m_translateMatrix;

  // accumulated rotation matrix
  Common::Matrix m_rotationMatrix;

  // world bound
  Common::Float4 m_bound;

  // move vector
  Common::Float3 m_moveVector;
};
} // namespace Game

#endif

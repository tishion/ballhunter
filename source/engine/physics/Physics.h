/*
 * File: Physics.h
 * Project: physics
 * Created: Sunday, 22nd May 2022 3:50:25 pm
 * Author: Sheen Tian Shen
 */
#ifndef PHYSICS_H
#define PHYSICS_H

#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include <common/SmartPointer.h>

namespace Engine {

class Physics {
  DECLARE_SMART_POINTER(Physics);

public:
  Physics();
  ~Physics();

  void update(float step);

public:
  reactphysics3d::PhysicsCommon m_physicsCommon;
  reactphysics3d::PhysicsWorld* m_pPhysicsWorld;
};

} // namespace Engine

#endif

#ifndef GROUND_H
#define GROUND_H

#pragma once

#include <common/SmartPointer.h>

#include <engine/object/Floor.h>
#include <engine/physics/Physics.h>

namespace Game {

class Ground : public Engine::Floor {
  DECLARE_SMART_POINTER(Ground);

public:
  Ground(Engine::IRenderer::RefPtr renderer, Engine::Physics::RefPtr physics, Common::Float4x4 pos);

  ~Ground();

private:
  // physics parameters
  Engine::Physics::RefPtr m_pPhysics;
  reactphysics3d::CollisionBody* m_pCollisionBody;
  reactphysics3d::HeightFieldShape* m_pCollisionShape;
  reactphysics3d::Collider* m_pCollider;

  std::vector<int> heightValues;
};

} // namespace Game

#endif

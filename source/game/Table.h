#ifndef TABLE_H
#define TABLE_H

#pragma once

#include <common/SmartPointer.h>

#include <engine/object/Floor.h>
#include <engine/physics/Physics.h>

namespace Game {

class Table : public Engine::Floor {
  DECLARE_SMART_POINTER(Table);

public:
  Table(Engine::IRenderer::RefPtr renderer, Engine::Physics::RefPtr physics, Common::Float4x4 pos);

  ~Table();

private:
  // physics parameters
  Engine::Physics::RefPtr m_pPhysics;
  reactphysics3d::RigidBody* m_pSurfaceBody;
  reactphysics3d::HeightFieldShape* m_pCollisionShape;
  reactphysics3d::Collider* m_pCollider;

  reactphysics3d::BoxShape* m_pVerticalEdgeBoxShape;
  reactphysics3d::BoxShape* m_pHorizontalEdgeBoxShape;

  reactphysics3d::RigidBody* m_pLeftEdgeBody;
  reactphysics3d::Collider* m_pLeftCollider;

  reactphysics3d::RigidBody* m_pTopEdgeBody;
  reactphysics3d::Collider* m_pTopCollider;

  reactphysics3d::RigidBody* m_pRightEdgeBody;
  reactphysics3d::Collider* m_pRightCollider;

  reactphysics3d::RigidBody* m_pBottomEdgeBody;
  reactphysics3d::Collider* m_pBottomCollider;

  std::vector<int> heightValues;
};

} // namespace Game

#endif

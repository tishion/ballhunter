#include "Ground.h"

Game::Ground::Ground(Engine::IRenderer::RefPtr renderer, Engine::Physics::RefPtr physics, Common::Float4x4 pos)
    : Engine::Floor(renderer, pos)
    , m_pPhysics(physics) {

  auto transfrom = reactphysics3d::Transform::identity();
  m_pCollisionBody = m_pPhysics->m_pPhysicsWorld->createCollisionBody(transfrom);

  int cols = static_cast<int>(pos.GetColumn(1).x - pos.GetColumn(0).x);
  int rows = static_cast<int>(pos.GetColumn(3).z - pos.GetColumn(0).z);
  heightValues.resize(cols * rows);
  m_pCollisionShape = m_pPhysics->m_physicsCommon.createHeightFieldShape(
      cols, rows, 0, 0, heightValues.data(), reactphysics3d::HeightFieldShape::HeightDataType::HEIGHT_INT_TYPE);
  m_pCollider = m_pCollisionBody->addCollider(m_pCollisionShape, transfrom);

  m_pCollisionBody->setIsActive(false);
}

Game::Ground::~Ground() {
  m_pCollisionBody->removeCollider(m_pCollider);
  m_pPhysics->m_physicsCommon.destroyHeightFieldShape(m_pCollisionShape);
  m_pPhysics->m_pPhysicsWorld->destroyCollisionBody(m_pCollisionBody);
}

#include "Table.h"

Game::Table::Table(Engine::IRenderer::RefPtr renderer, Engine::Physics::RefPtr physics, Common::Float4x4 pos)
    : Engine::Floor(renderer, pos)
    , m_pPhysics(physics) {

  // create table surface
  auto transform = reactphysics3d::Transform::identity();
  m_pSurfaceBody = m_pPhysics->m_pPhysicsWorld->createRigidBody(transform);
  m_pSurfaceBody->setType(reactphysics3d::BodyType::STATIC);

  int cols = static_cast<int>(pos.GetColumn(1).x - pos.GetColumn(0).x);
  int rows = static_cast<int>(pos.GetColumn(3).z - pos.GetColumn(0).z);
  heightValues.resize(cols * rows);
  m_pCollisionShape = m_pPhysics->m_physicsCommon.createHeightFieldShape(
      cols, rows, 0, 0, heightValues.data(), reactphysics3d::HeightFieldShape::HeightDataType::HEIGHT_INT_TYPE);
  m_pCollider = m_pSurfaceBody->addCollider(m_pCollisionShape, transform);

  // create table edge, we use 4 box bodies
  m_pVerticalEdgeBoxShape = m_pPhysics->m_physicsCommon.createBoxShape(reactphysics3d::Vector3(1.0f, 500.0f, rows));

  // left
  transform = reactphysics3d::Transform(reactphysics3d::Vector3(pos.GetColumn(0).x - 1.0f, 0.0f, 0.0f),
                                        reactphysics3d::Quaternion::identity());
  m_pLeftEdgeBody = m_pPhysics->m_pPhysicsWorld->createRigidBody(transform);
  m_pLeftEdgeBody->setType(reactphysics3d::BodyType::STATIC);
  m_pLeftCollider = m_pLeftEdgeBody->addCollider(m_pVerticalEdgeBoxShape, transform);

  //// right
  // transform = reactphysics3d::Transform(reactphysics3d::Vector3(pos.GetColumn(2).x, 500.0f, 0.0f),
  //                                       reactphysics3d::Quaternion::identity());
  // m_pRightEdgeBody = m_pPhysics->m_pPhysicsWorld->createRigidBody(transform);
  // m_pRightEdgeBody->setType(reactphysics3d::BodyType::STATIC);
  // m_pRightCollider = m_pRightEdgeBody->addCollider(m_pVerticalEdgeBoxShape, transform);

  m_pHorizontalEdgeBoxShape = m_pPhysics->m_physicsCommon.createBoxShape(reactphysics3d::Vector3(cols, 500.0f, 1.0f));

  //// top
  // transform = reactphysics3d::Transform(reactphysics3d::Vector3(0.0f, 10.0f, pos.GetColumn(0).z),
  //                                       reactphysics3d::Quaternion::identity());
  // m_pTopEdgeBody = m_pPhysics->m_pPhysicsWorld->createRigidBody(transform);
  // m_pTopEdgeBody->setType(reactphysics3d::BodyType::STATIC);
  // m_pTopCollider = m_pTopEdgeBody->addCollider(m_pHorizontalEdgeBoxShape, transform);

  // bottom
  transform = reactphysics3d::Transform(reactphysics3d::Vector3(0.0f, 500.0f, pos.GetColumn(2).z),
                                        reactphysics3d::Quaternion::identity());
  m_pBottomEdgeBody = m_pPhysics->m_pPhysicsWorld->createRigidBody(transform);
  m_pBottomEdgeBody->setType(reactphysics3d::BodyType::STATIC);
  m_pBottomCollider = m_pBottomEdgeBody->addCollider(m_pHorizontalEdgeBoxShape, transform);
}

Game::Table::~Table() {
  m_pSurfaceBody->removeCollider(m_pCollider);
  m_pPhysics->m_physicsCommon.destroyHeightFieldShape(m_pCollisionShape);
  m_pPhysics->m_pPhysicsWorld->destroyRigidBody(m_pSurfaceBody);
}

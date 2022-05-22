#include "Physics.h"

using namespace reactphysics3d;

Engine::Physics::Physics() {
  // create physics engine
  PhysicsWorld::WorldSettings settings;
  settings.defaultVelocitySolverNbIterations = 20;
  settings.isSleepingEnabled = false;
  settings.gravity = Vector3(0.0f, -9.81f, 0.0f);
  m_pPhysicsWorld = m_physicsCommon.createPhysicsWorld(settings);
}

Engine::Physics::~Physics() {
  if (m_pPhysicsWorld) {
    m_physicsCommon.destroyPhysicsWorld(m_pPhysicsWorld);
    m_pPhysicsWorld = nullptr;
  }
}

void Engine::Physics::update(float step) {
  m_pPhysicsWorld->update(step);
}

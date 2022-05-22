#include "GameMain.h"

#include <common/Logger.h>

#include <engine/renderer/RenderFactory.h>

namespace Game {

GameMain::GameMain()
    : m_timer()
    , m_pRenderer(Engine::RendererFactory::CreateRenderer())
    , m_pPhysics(std::make_shared<Engine::Physics>())
    , m_pCamera(std::make_shared<Engine::Camera>())
    , m_pController(std::make_shared<Engine::InputContoller>()) {
  worldLeft = -50.0f;
  worldRight = 50.0f;
  worldTop = -50.0f;
  worldBottom = 50.0f;
}

GameMain::~GameMain() {
}

bool GameMain::Initialize(SDL_Window* window) {
  m_pPhysics->m_pPhysicsWorld->setEventListener(this);

  if (!m_pRenderer->Initialize(window, m_pCamera, m_pController, shared_from_this())) {
    LOG(ERROR) << "Failed to initialize renderer.";
    return false;
  }

  // set camera matrix, 0x & +y & +z
  m_pCamera->SetViewParams(Common::Float3(0.0f, 50.0f, 100.0f), // Eye point in world coordinates.
                           Common::Float3(0.0f, 0.0f, 0.0f),    // Look at point in world coordinates.
                           Common::Float3(0.0f, 1.0f, 0.0f)     // The Up vector for the camera.
  );

  //// set camera matrix, 0x & 0y & 0+z
  // m_pCamera->SetViewParams(Common::Float3(0.0f, 0.0f, 100.0f), // Eye point in world coordinates.
  //                          Common::Float3(0.0f, 0.0f, 0.0f),   // Look at point in world coordinates.
  //                          Common::Float3(0.0f, 1.0f, 0.0f)    // The Up vector for the camera.
  //);

  //// set camera matrix, 0x & +y & 0z
  // m_pCamera->SetViewParams(Common::Float3(0.0f, 150.0f, 0.0f), // Eye point in world coordinates.
  //                          Common::Float3(0.0f, 0.0f, 0.0f),   // Look at point in world coordinates.
  //                          Common::Float3(0.0f, 0.0f, -1.0f)   // The Up vector for the camera.
  //);

  // initialize the floor (world)
  Common::Float4x4 floorVertices(Common::Float4(worldLeft, 0.0f, worldTop, 0.0f),     //
                                 Common::Float4(worldRight, 0.0f, worldTop, 0.0f),    //
                                 Common::Float4(worldRight, 0.0f, worldBottom, 0.0f), //
                                 Common::Float4(worldLeft, 0.0f, worldBottom, 0.0f)   //
  );
  m_pGround = std::make_shared<Game::Ground>(m_pRenderer, m_pPhysics, floorVertices);
  m_pGround->Initialize();

  // initialize the sprite
  m_pSprite = std::make_shared<Game::Sprite>(m_pRenderer, m_pPhysics, m_pController, 5.0f);
  m_pSprite->Initialize();
  m_pSprite->SetBounds(Common::Float4(worldLeft, worldTop, worldRight, worldBottom));

  // initialize the ball
  m_pBallA = std::make_shared<Game::Ball>(m_pRenderer, m_pPhysics, 5.0f);
  m_pBallA->Initialize();
  m_pBallA->SetBounds(Common::Float4(worldLeft, worldTop, worldRight, worldBottom));

  m_pBallB = std::make_shared<Game::Ball>(m_pRenderer, m_pPhysics, 5.0f);
  m_pBallB->Initialize();
  m_pBallB->SetBounds(Common::Float4(worldLeft, worldTop, worldRight, worldBottom));

  m_radiusSpriteAndBall = (m_pBallA->Radius() + m_pSprite->Radius());
  m_radiusSpriteAndBall *= m_radiusSpriteAndBall;

  m_radiusBallAndBall = m_pBallA->Radius() + m_pBallB->Radius();
  m_radiusBallAndBall *= m_radiusBallAndBall;

  RestartGame();

  return true;
}

void GameMain::Uninitialize() {
  if (m_pBallB) {
    m_pBallB->Uninitialize();
    m_pBallB.reset();
  }

  if (m_pBallA) {
    m_pBallA->Uninitialize();
    m_pBallA.reset();
  }

  if (m_pSprite) {
    m_pSprite->Uninitialize();
    m_pSprite.reset();
  }

  if (m_pGround) {
    m_pGround->Uninitialize();
    m_pGround.reset();
  }

  if (m_pRenderer) {
    m_pRenderer->Uninitialize();
    m_pRenderer.reset();
  }
}

void GameMain::WindowSizeChanged(uint32_t width, uint32_t height) {
  m_pRenderer->UpdateWindowSize(width, height);
  m_pRenderer->CreateWindowSizeDependentResources();
}

void GameMain::Process(const SDL_Event& e) {
  m_pController->Update(e);
}

void GameMain::Update() {
  m_timer.Tick([&]() {
    // update game logic
    UpdateGame();
  });

  m_pRenderer->Render();
}

void GameMain::RestartGame() {
  m_pSprite->SetPosition(Common::Float3(30.0f, 0.0f, 30.0f));

  float x = 0;
  float z = 0;
  ::srand(time(nullptr));
  m_pBallA->SetPosition(Common::Float3(-40.0f, 0.0f, -40.0f));
  x = Math::RandomInRange<float>(50.0f, 100.0f);
  z = Math::RandomInRange<float>(50.0f, 100.0f);
  m_pBallA->SetVelocity(Common::Float3(x, 0.0f, z));

  ::srand(time(nullptr));
  m_pBallB->SetPosition(Common::Float3(40.0f, 0.0f, -40.0f));
  x = Math::RandomInRange<float>(50.0f, 100.0f);
  z = Math::RandomInRange<float>(50.0f, 100.0f);
  m_pBallB->SetVelocity(Common::Float3(x, 0.0f, z));
}

void GameMain::UpdateGame() {
  m_pPhysics->update(m_timer.GetElapsedSeconds());

  m_pSprite->Update(m_timer);
  m_pBallA->Update(m_timer);
  m_pBallB->Update(m_timer);

  HandleBallBallCollision();
}

bool GameMain::HandleBallBallCollision() {
  // detect the collision between player and ball
  auto posA = m_pBallA->GetPosition();
  auto posB = m_pBallB->GetPosition();
  auto deltaX = posA.x - posB.x;
  auto deltaZ = posA.z - posB.z;

  if (deltaX * deltaX + deltaZ * deltaZ <= m_radiusBallAndBall) {
    // swap the velocities of the ball and sprite
    auto vA = m_pBallA->GetVelocity();
    auto vB = m_pBallB->GetVelocity();
    m_pBallA->SetVelocity(vB);
    m_pBallB->SetVelocity(vA);
    return true;
  }

  return false;
}

void GameMain::onContact(const CollisionCallback::CallbackData& callbackData) {
  LOG(INFO) << "Collision occurred";
  // callbackData.getContactPair();
}

void GameMain::OnRenderDeviceLost() {
}

void GameMain::OnRenderDeviceRestored() {
}
} // namespace Game

#include "GameMain.h"

#include <common/Logger.h>

#include <engine/renderer/RenderFactory.h>

namespace Game {

GameMain::GameMain()
    : m_timer()
    , m_pCamera(std::make_shared<Engine::Camera>())
    , m_pRenderer(Engine::RendererFactory::CreateRenderer())
    , m_pController(std::make_shared<Engine::InputContoller>()) {
  worldLeft = -50.0f;
  worldRight = 50.0f;
  worldTop = -50.0f;
  worldBottom = 50.0f;
}

GameMain::~GameMain() {
}

bool GameMain::Initialize(SDL_Window* window) {
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

  Common::Float4x4 floorVertices(Common::Float4(worldLeft, 0.0f, worldTop, 0.0f),     //
                                 Common::Float4(worldRight, 0.0f, worldTop, 0.0f),    //
                                 Common::Float4(worldRight, 0.0f, worldBottom, 0.0f), //
                                 Common::Float4(worldLeft, 0.0f, worldBottom, 0.0f)   //
  );
  m_pFloor = std::make_shared<Engine::Floor>(m_pRenderer, floorVertices);
  m_pFloor->Initialize();

  m_pSprite = std::make_shared<Game::Sprite>(m_pRenderer, m_pController, 5.0f);
  m_pSprite->Initialize();

  m_pBall = std::make_shared<Game::Ball>(m_pRenderer, 5.0f);
  m_pBall->Initialize();
  m_pBall->SetBounds(Common::Float4(worldLeft, worldTop, worldRight, worldBottom));
  m_pBall->SetVelocity(Common::Float3(123.45f, 0.0f, 67.89f));

  return true;
}

void GameMain::Uninitialize() {
  if (m_pBall) {
    m_pBall->Uninitialize();
    m_pBall.reset();
  }

  if (m_pSprite) {
    m_pSprite->Uninitialize();
    m_pSprite.reset();
  }

  if (m_pFloor) {
    m_pFloor->Uninitialize();
    m_pFloor.reset();
  }

  m_pRenderer->Uninitialize();
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

void GameMain::UpdateGame() {
  m_pSprite->Update(m_timer);

  m_pBall->Update(m_timer);
}

void GameMain::OnRenderDeviceLost() {
}

void GameMain::OnRenderDeviceRestored() {
}
} // namespace Game

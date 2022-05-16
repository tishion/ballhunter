#include "GameMain.h"

#include <common/Logger.h>

#include <engine/renderer/RenderFactory.h>

namespace Game {

GameMain::GameMain()
    : m_timer()
    , m_pCamera(std::make_shared<Engine::Camera>())
    , m_pRenderer(Engine::RendererFactory::CreateRenderer())
    , m_pController(std::make_shared<Engine::InputContoller>()) {
}

GameMain::~GameMain() {
}

bool GameMain::Initialize(SDL_Window* window) {
  if (!m_pRenderer->Initialize(window, m_pCamera, m_pController, shared_from_this())) {
    LOG(ERROR) << "Failed to initialize renderer.";
    return false;
  }

  // set camera matrix, 0x & +y & +z
  m_pCamera->SetViewParams(Common::Float3(0.0f, 50.0f, 50.0f), // Eye point in world coordinates.
                           Common::Float3(0.0f, 0.0f, 0.0f),   // Look at point in world coordinates.
                           Common::Float3(0.0f, 1.0f, 0.0f)    // The Up vector for the camera.
  );

  //// set camera matrix, 0x & 0y & 0+z
  // m_pCamera->SetViewParams(Common::Float3(0.0f, 0.0f, 50.0f), // Eye point in world coordinates.
  //                          Common::Float3(0.0f, 0.0f, 0.0f),  // Look at point in world coordinates.
  //                          Common::Float3(0.0f, 1.0f, 0.0f)   // The Up vector for the camera.
  //);

  //// set camera matrix, 0x & +y & 0z
  // m_pCamera->SetViewParams(Common::Float3(0.0f, 150.0f, 0.0f), // Eye point in world coordinates.
  //                          Common::Float3(0.0f, 0.0f, 0.0f),   // Look at point in world coordinates.
  //                          Common::Float3(0.0f, 0.0f, -1.0f)   // The Up vector for the camera.
  //);

  m_pFloor = std::make_shared<Engine::Floor>(m_pRenderer, -50, -50, 101, 101);
  m_pFloor->Initialize();

  m_pBall = std::make_shared<Game::Ball>(m_pRenderer, 2.0f);
  m_pBall->Initialize();
  m_pBall->SetVelocity(Common::Float3(5.0f, 0.0f, 5.0f));

  return true;
}

void GameMain::Uninitialize() {
  if (m_pBall) {
    m_pBall->Uninitialize();
    m_pBall.reset();
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
  m_pBall->Update(m_timer);
}

void GameMain::OnRenderDeviceLost() {
}

void GameMain::OnRenderDeviceRestored() {
}
} // namespace Game

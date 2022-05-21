#include "Application.h"

#include <SDL.h>

#include <common/Logger.h>

Application::Application()
    : m_window(nullptr) {
  // initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    LOG(ERROR) << "SDL_Init Error: " << SDL_GetError();
  }
}

Application::~Application() {
  if (m_game) {
    m_game.reset();
  }

  if (m_window) {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }

  SDL_Quit();
}

int Application::Run() {

  // create main window
  m_window = SDL_CreateWindow("BallHunter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 760,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (m_window == nullptr) {
    LOG(ERROR) << "SDL_CreateWindow Error: " << SDL_GetError();
  }

  // create game instance
  m_game = std::make_shared<Game::GameMain>();
  if (m_game == nullptr) {
    LOG(ERROR) << "Failed to create game instance.";
  }

  // initialize the game instance
  if (!m_game->Initialize(m_window)) {
    LOG(ERROR) << "Failed to initialize game instance.";
  }

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    // poll event
    while (SDL_PollEvent(&e)) {
      quit = ProcessEvent(e);

      // update input
      m_game->Process(e);
    }

    // update game status
    m_game->Update();
  }

  // clean up game
  m_game->Uninitialize();

  return 0;
}

bool Application::ProcessEvent(const SDL_Event& e) {
  switch (e.type) {
  case SDL_QUIT:
    return true;
    break;
  case SDL_KEYDOWN:
    if (e.key.keysym.sym == SDLK_ESCAPE)
      return true;
    break;
  case SDL_WINDOWEVENT: {
    switch (e.window.event) {
    case SDL_WINDOWEVENT_RESIZED:
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      m_game->WindowSizeChanged(e.window.data1, e.window.data2);
      break;
    default:
      break;
    }
  } break;
  default:
    break;
  }

  return false;
}

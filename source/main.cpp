/*
 * File: main.cpp
 * Project: source
 * Created: Monday, 16th May 2022 9:07:20 pm
 * Author: Sheen Tian Shen
 */

#include <SDL.h>

#include "common/Logger.h"

#include "Application.h"

INITIALIZE_EASYLOGGINGPP;

int main(int argc, char* argv[]) {
  // init logging library
  Common::InitLog(argc, argv);

  // create application and run
  Application app;
  return app.Run();

  // SDL_SetMainReady();
  // if (SDL_Init(SDL_INIT_VIDEO) != 0) {
  //  LOG(ERROR) << "SDL_Init Error: " << SDL_GetError();
  //  return false;
  //}

  // SDL_Window* window =
  //    SDL_CreateWindow("BallHunter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,
  //                     SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  // if (window == nullptr) {
  //  LOG(ERROR) << "SDL_CreateWindow Error: " << SDL_GetError();
  //  return false;
  //}

  // SDL_Event e;
  // bool quit = false;
  // while (!quit) {
  //  // Event Polling
  //  while (SDL_PollEvent(&e)) {
  //  }
  //}

  // return 0;

  // Engine::RefPtr engine = Engine::GetInstance();
  // if (!engine->Initialize()) {
  //  return -1;
  //}

  // Game::RefPtr game = Game::GetInstance();

  // int ret = engine->Run(game);

  // engine->Uninitialize();

  // return ret;
}

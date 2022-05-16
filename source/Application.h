/*
 * File: Application.h
 * Project: source
 * Created: Wednesday, 18th May 2022 10:12:48 am
 * Author: Sheen Tian
 */
#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once

#include <SDL.h>

#include <game/GameMain.h>

class Application {
public:
  Application();
  ~Application();

  int Run();

protected:
  bool ProcessEvent(const SDL_Event& e);

private:
  SDL_Window* m_window;
  Game::GameMain::RefPtr m_game;
};

#endif

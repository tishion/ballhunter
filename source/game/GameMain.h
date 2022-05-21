/*
 * File: GameMain.h
 * Project: source
 * Created: Monday, 16th May 2022 11:39:06 pm
 * Author: Sheen Tian Shen
 */
#ifndef GAMEMAIN_H
#define GAMEMAIN_H

#pragma once

#include <vector>

#include <common/SmartPointer.h>

#include <engine/input/InputContoller.h>
#include <engine/object/Camera.h>
#include <engine/object/Floor.h>
#include <engine/renderer/IRenderer.h>
#include <engine/utils/StepTimer.h>

#include <game/Ball.h>
#include <game/Sprite.h>

namespace Game {

class GameMain
    : public Engine::RenderEventListener
    , public std::enable_shared_from_this<GameMain> {
  DECLARE_SMART_POINTER(GameMain);

public:
  GameMain();

  ~GameMain();

  bool Initialize(SDL_Window* window);

  void Uninitialize();

  void WindowSizeChanged(uint32_t width, uint32_t height);

  void Process(const SDL_Event& e);

  void Update();

protected:
  void UpdateGame();

  void OnRenderDeviceLost() override;

  void OnRenderDeviceRestored() override;

private:
  Engine::StepTimer m_timer;

  Engine::Camera::RefPtr m_pCamera;
  Engine::IRenderer::RefPtr m_pRenderer;
  Engine::InputContoller::RefPtr m_pController;

  float worldLeft;
  float worldRight;
  float worldTop;
  float worldBottom;

  Engine::Floor::RefPtr m_pFloor;
  Game::Ball::RefPtr m_pBall;
  Game::Sprite::RefPtr m_pSprite;
};

} // namespace Game
#endif

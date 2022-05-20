/*
 * File: InputContoller.h
 * Project: input
 * Created: Thursday, 19th May 2022 2:25:15 pm
 * Author: Sheen Tian
 */
#ifndef INPUTCONTOLLER_H
#define INPUTCONTOLLER_H

#pragma once

#include <SDL_events.h>

#include <common/BasicTypes.h>
#include <common/SmartPointer.h>

namespace Engine {

class InputContoller {
  DECLARE_SMART_POINTER(InputContoller);

public:
  InputContoller();

  ~InputContoller();

  Common::Float3 GetVelocity();

  void Update(const SDL_Event& e);

protected:
  void PollingInputEvent(const SDL_Event& e);
  void OnKeyDown(SDL_Keycode key);
  void OnKeyUp(SDL_Keycode key);

  void ClearState();

private:
  Common::Float3 m_moveCommand;

  Common::Float3 m_velocity;

  // Input states for Keyboard.
  bool m_forward = false;
  bool m_back = false;
  bool m_left = false;
  bool m_right = false;
};
} // namespace Engine

#endif

#include "InputContoller.h"

const float MovementGain = 2.0f;

Engine::InputContoller::InputContoller() {
}

Engine::InputContoller::~InputContoller() {
}

void Engine::InputContoller::Update(const SDL_Event& e) {
  PollingInputEvent(e);

  if (m_forward) {
    m_moveCommand.y += 1.0f;
  }
  if (m_back) {
    m_moveCommand.y -= 1.0f;
  }
  if (m_left) {
    m_moveCommand.x += 1.0f;
  }
  if (m_right) {
    m_moveCommand.x -= 1.0f;
  }
  if (m_up) {
    m_moveCommand.z += 1.0f;
  }
  if (m_down) {
    m_moveCommand.z -= 1.0f;
  }

  // Make sure that 45deg cases are not faster.
  if (fabsf(m_moveCommand.x) > 0.1f || fabsf(m_moveCommand.y) > 0.1f || fabsf(m_moveCommand.z) > 0.1f) {
    m_moveCommand.Normalize();
  }

  // Rotate command to align with our direction (world coordinates).
  Common::Float3 wCommand;
  wCommand.x = m_moveCommand.x * cosf(m_yaw) - m_moveCommand.y * sinf(m_yaw);
  wCommand.y = m_moveCommand.x * sinf(m_yaw) + m_moveCommand.y * cosf(m_yaw);
  wCommand.z = m_moveCommand.z;

  // Scale for sensitivity adjustment, the velocity is based on the command. Y is up.
  m_velocity.x = -wCommand.x * MovementGain;
  m_velocity.z = wCommand.y * MovementGain;
  m_velocity.y = wCommand.z * MovementGain;

  // Clear movement input accumulator for use during next frame.
  m_moveCommand = Common::Float3(0.0f, 0.0f, 0.0f);
}

void Engine::InputContoller::PollingInputEvent(const SDL_Event& e) {
  switch (e.type) {
  case SDL_KEYDOWN:
    OnKeyDown(e.key.keysym.sym);
    break;
  case SDL_KEYUP:
    OnKeyUp(e.key.keysym.sym);
    break;
  default:
    break;
  }
}

void Engine::InputContoller::OnKeyDown(SDL_Keycode key) {
  switch (key) {
  case SDLK_LEFT:
  case SDLK_a:
    m_left = true;
    break;
  case SDLK_RIGHT:
  case SDLK_d:
    m_right = true;
    break;
  case SDLK_UP:
  case SDLK_w:
    m_forward = true;
    break;
  case SDLK_DOWN:
  case SDLK_s:
    m_back = true;
    break;
  default:
    break;
  }
}

void Engine::InputContoller::OnKeyUp(SDL_Keycode key) {
  switch (key) {
  case SDLK_LEFT:
  case SDLK_a:
    m_left = false;
    break;
  case SDLK_RIGHT:
  case SDLK_d:
    m_right = false;
    break;
  case SDLK_UP:
  case SDLK_w:
    m_forward = false;
    break;
  case SDLK_DOWN:
  case SDLK_s:
    m_back = false;
    break;
  default:
    break;
  }
}

void Engine::InputContoller::ClearState() {
  m_forward = false;
  m_back = false;
  m_left = false;
  m_right = false;
}

/*
 * File: GameObject.h
 * Project: object
 * Created: Tuesday, 17th May 2022 9:04:15 pm
 * Author: Sheen Tian Shen
 */
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#pragma once

#include <common/BasicTypes.h>

#include <engine/renderer/IRenderable.h>
#include <engine/renderer/IRenderer.h>
#include <engine/utils/StepTimer.h>

namespace Engine {

class GameObject : public IRenderable {
public:
  GameObject(Engine::IRenderer::RefPtr renderer);

  ~GameObject();

  bool Initialize() override;

  void Uninitialize() override;

  virtual void Render(void* context) override;

  virtual void Update(const Engine::StepTimer& timer){};

  void SetPosition(Common::Float3 position);

  Common::Float3 GetPosition();

  void SetVelocity(Common::Float3 velocity);

  Common::Float3 GetVelocity();

protected:
  Engine::IRenderer::RefPtr m_pRenderer;

  Common::Float3 m_position;
  Common::Float3 m_velocity;

  Common::Matrix m_worldMatrix;
};

} // namespace Engine
#endif

/*
 * File: Sphere.h
 * Project: object
 * Created: Tuesday, 17th May 2022 9:04:24 pm
 * Author: Sheen Tian Shen
 */
#ifndef SPHERE_H
#define SPHERE_H

#pragma once

#include <engine/object/GameObject.h>

namespace Engine {

class Sphere : public GameObject {
  DECLARE_SMART_POINTER(Sphere);

public:
  Sphere(Engine::IRenderer::RefPtr renderer, float radius);

  ~Sphere();

  bool Initialize() override;

  void Uninitialize() override;

  void Render(void* context) override;

  void Update(const Engine::StepTimer& timer) override;

  void Radius(float radius);

  float Radius();

protected:
  float m_radius;

private:
  class Implementation;
  std::unique_ptr<Implementation> m_pImpl;
};

} // namespace Engine
#endif

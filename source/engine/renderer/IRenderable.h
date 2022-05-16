/*
 * File: IRenderable.h
 * Project: renderer
 * Created: Tuesday, 17th May 2022 12:50:18 am
 * Author: Sheen Tian Shen
 */
#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#pragma once

#include <common/SmartPointer.h>

namespace Engine {

class IRenderable : public std::enable_shared_from_this<IRenderable> {
  DECLARE_SMART_POINTER(IRenderable);

public:
  virtual ~IRenderable(){};

  virtual bool Initialize() = 0;

  virtual void Uninitialize() = 0;

  virtual void Render(void* renderingContext) = 0;
};

} // namespace Engine

#endif

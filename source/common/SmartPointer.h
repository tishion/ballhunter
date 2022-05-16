#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#pragma once

#include <memory>

#define DECLARE_SMART_POINTER(c)                                                                                       \
public:                                                                                                                \
  typedef std::shared_ptr<c> RefPtr;                                                                                   \
  typedef std::weak_ptr<c> WeakPtr

#endif

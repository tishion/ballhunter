/*
 * File: Camera.h
 * Project: object
 * Created: Tuesday, 17th May 2022 9:04:34 pm
 * Author: Sheen Tian Shen
 */
#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#include <common/BasicTypes.h>
#include <common/SmartPointer.h>

namespace Engine {

class Camera {
  DECLARE_SMART_POINTER(Camera);

public:
  Camera();

  void SetViewParams(Common::Float3 eye, Common::Float3 lookAt, Common::Float3 up);

  void SetProjParams(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

  void LookDirection(Common::Float3 lookDirection);

  void Eye(Common::Float3 position);

  Common::Matrix View();

  Common::Matrix Projection();

  // Common::Matrix World();

  Common::Float3 Eye();

  Common::Float3 LookAt();

  Common::Float3 Up();

  float NearClipPlane();

  float FarClipPlane();

  float Pitch();

  float Yaw();

private:
  Common::Matrix m_viewMatrix;
  Common::Matrix m_projectionMatrix;

  // Common::Matrix m_inverseView;

  Common::Float3 m_eye;
  Common::Float3 m_lookAt;
  Common::Float3 m_up;

  float m_cameraYawAngle;
  float m_cameraPitchAngle;

  float m_fieldOfView;
  float m_aspectRatio;
  float m_nearPlane;
  float m_farPlane;
};
} // namespace Engine

#endif

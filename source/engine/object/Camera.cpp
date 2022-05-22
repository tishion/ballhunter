#include "Camera.h"

using namespace Common;

namespace Engine {

Camera::Camera() {
  // Setup the view matrix.
  SetViewParams(Float3(0.0f, 0.0f, 0.0f), // Default eye position.
                Float3(0.0f, 0.0f, 1.0f), // Default look at position.
                Float3(0.0f, 1.0f, 0.0f)  // Default up vector.
  );

  // Setup the projection matrix.
  SetProjParams(static_cast<float>(M_PI) / 4, 1.0f, 1.0f, 1000.0f);
}

void Camera::SetViewParams(Float3 eye, Float3 lookAt, Float3 up) {
  m_eye = eye;
  m_lookAt = lookAt;
  m_up = up;

  // Calculate the view matrix.
  m_viewMatrix = Matrix::LookAt(m_lookAt, m_eye, m_up, 1.0f);
  // m_inverseView = m_viewMatrix.Inverse();

  //// The axis basis vectors and camera position are stored inside the
  //// position matrix in the 4 rows of the camera's world matrix.
  //// To figure out the yaw/pitch of the camera, we just need the Z basis vector.
  // Float4 zBasis = m_inverseView.GetColumn(2);

  // m_cameraYawAngle = atan2f(zBasis.x, zBasis.z);

  // float len = sqrtf(zBasis.z * zBasis.z + zBasis.x * zBasis.x);
  // m_cameraPitchAngle = atan2f(zBasis.y, len);
}

void Camera::SetProjParams(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
  // Set attributes for the projection matrix.
  m_fieldOfView = fieldOfView;
  m_aspectRatio = aspectRatio;
  m_nearPlane = nearPlane;
  m_farPlane = farPlane;

  m_projectionMatrix = Float4x4::Perspective(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
}

void Camera::LookDirection(Float3 lookDirection) {
  Float3 lookAt;
  lookAt.x = m_eye.x + lookDirection.x;
  lookAt.y = m_eye.y + lookDirection.y;
  lookAt.z = m_eye.z + lookDirection.z;

  SetViewParams(m_eye, lookAt, m_up);
}

void Camera::Eye(Float3 eye) {
  SetViewParams(eye, m_lookAt, m_up);
}

Matrix Camera::View() {
  return m_viewMatrix;
}

Matrix Camera::Projection() {
  return m_projectionMatrix;
}

// Matrix Camera::World() {
//  return m_inverseView;
//}

Float3 Camera::Eye() {
  return m_eye;
}

Float3 Camera::LookAt() {
  return m_lookAt;
}

Float3 Camera::Up() {
  return m_up;
}

float Camera::NearClipPlane() {
  return m_nearPlane;
}

float Camera::FarClipPlane() {
  return m_farPlane;
}

float Camera::Pitch() {
  return m_cameraPitchAngle;
}

float Camera::Yaw() {
  return m_cameraYawAngle;
}
} // namespace Engine

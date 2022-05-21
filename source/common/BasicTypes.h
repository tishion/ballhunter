/*
 * File: BasicTypes.h
 * Project: common
 * Created: Tuesday, 17th May 2022 9:08:05 pm
 * Author: Sheen Tian Shen
 */
#ifndef BASICTYPES_H
#define BASICTYPES_H

#pragma once

#include <cstdint>

#include <mathfu/constants.h>
#include <mathfu/matrix.h>
#include <mathfu/quaternion.h>
#include <mathfu/vector.h>

namespace Math = mathfu;

namespace Common {
typedef mathfu::Vector<float, 2> Float2;
typedef mathfu::Vector<float, 3> Float3;
typedef mathfu::Vector<float, 4> Float4;
typedef mathfu::Matrix<float, 4, 4> Float4x4;
typedef mathfu::Matrix<float, 4, 4> Matrix;
typedef mathfu::Quaternion<float> Quaternion;

/* pi */
const float PI_VAL = 3.14159265358979323846f;

const float F_MIN = 1e-7f;

} // namespace Common

#endif

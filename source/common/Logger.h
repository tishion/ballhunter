/*
 * File: Logger.h
 * Project: common
 * Created: Tuesday, 17th May 2022 1:21:17 am
 * Author: Sheen Tian Shen
 */
#ifndef LOGGER_H
#define LOGGER_H

#pragma once

#include <easylogging++/easylogging++.h>

namespace Common {

/// <summary>
///
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
void InitLog(int argc, char** argv);

}; // namespace Common

#endif

/*
 * File: main.cpp
 * Project: source
 * Created: Monday, 16th May 2022 9:07:20 pm
 * Author: Sheen Tian Shen
 */

#include "common/Logger.h"

#include "Application.h"

INITIALIZE_EASYLOGGINGPP;

int main(int argc, char* argv[]) {
  // init logging library
  Common::InitLog(argc, argv);

  // create application and run
  Application app;
  return app.Run();
}

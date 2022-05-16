#include "Logger.h"

void Common::InitLog(int argc, char** argv) {
  START_EASYLOGGINGPP(argc, argv);

  el::Configurations defaultConf;
  defaultConf.setToDefault();
  // Values are always std::string
  defaultConf.set(el::Level::Info, el::ConfigurationType::Format, "%datetime %level %msg");
  // default logger uses default configurations
  el::Loggers::reconfigureLogger("default", defaultConf);
}

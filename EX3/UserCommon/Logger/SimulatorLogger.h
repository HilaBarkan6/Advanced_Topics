#pragma once
#include "../../UserCommon/Logger/Logger.h"

inline UserCommon_209399021_208239152::Logger& getSimulatorLogger() {
    static bool initialized = false;
    static UserCommon_209399021_208239152::Logger logger;
    if (!initialized) {
        logger.setLogFile("simulator.log");
        initialized = true;
    }
    return logger;
}

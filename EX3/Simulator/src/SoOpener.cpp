#include "SoOpener.h"

SoOpener::SoOpener(std::string so_path) {
    handle = dlopen(so_path.c_str(), RTLD_LAZY);
    if (!handle) {
        throw std::runtime_error("Failed to open: " + so_path);
    }
}

SoOpener::~SoOpener() {
    try {
        if (handle) {
            dlclose(handle);
            handle = nullptr; // Set to nullptr to avoid double close
        }
    }
    catch (...){}
}
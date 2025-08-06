#ifndef SO_OPENER_H
#define SO_OPENER_H

#include <string>
#include <dlfcn.h>
#include <stdexcept>

class SoOpener {
    private:
        void* handle;
        
    public:
        SoOpener(std::string so_path); 
        ~SoOpener();
        SoOpener(const SoOpener&) = delete;
        SoOpener& operator=(const SoOpener&) = delete;
        SoOpener(SoOpener&&) = delete;
        SoOpener& operator=(SoOpener&&) = delete;
};

#endif
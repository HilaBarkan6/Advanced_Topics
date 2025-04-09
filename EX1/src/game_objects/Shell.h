#ifndef SHELL_H
#define SHELL_H
#include "GameObject.h"

class Shell: public GameObject{
    private:
        int location_x;
        int location_y;

    public:
        Shell(int x, int y) ;
        int getLocationX() const { return location_x; }
        int getLocationY() const { return location_y; }
        void setLocation(int x, int y) {}
        
};

#endif
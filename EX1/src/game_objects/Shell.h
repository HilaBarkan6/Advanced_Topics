#ifndef SHELL_H
#define SHELL_H
#include "GameObject.h"
#include "CanonDirection.h"


class Shell: public GameObject{
    private:
        int location_x;
        int location_y;
        CanonDirection flying_direction;

    public:
        Shell() : GameObject(), location_x(-1), location_y(-1), flying_direction(CanonDirection::None) {};
        int getLocationX() const;
        int getLocationY() const;
        void setLocation(int x, int y);
        void setFlyingDirection(CanonDirection direction);
        
};

#endif
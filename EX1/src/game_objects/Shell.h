#ifndef SHELL_H
#define SHELL_H
#include "GameObject.h"
#include "CanonDirection.h"


class Shell: public GameObject{
    private:
        int location_x;
        int location_y;
        std::pair<int, int> next_location;
        CanonDirection flying_direction;

    public:
        Shell(int x, int y, CanonDirection dir);
        int getLocationX() const;
        int getLocationY() const;
        void setLocation(int x, int y);
        void setFlyingDirection(CanonDirection direction);
        CanonDirection getFlyingDirection() const ;
        std::pair<int, int> getNextLocation() const;
        void setNextLocation(int x, int y);
   
};

#endif
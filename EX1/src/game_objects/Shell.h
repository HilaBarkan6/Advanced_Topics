#ifndef SHELL_H
#define SHELL_H
#include "GameObject.h"
#include "CanonDirection.h"
#include <vector>
#include <utility> // for std::pair


class Shell: public GameObject{
    private:
        std::pair<int, int> location;
        std::pair<int, int> next_location;
        std::pair<int, int> previous_location;
        CanonDirection flying_direction;
        int tank_id;

    public:
        Shell(std::pair<int, int> loc, CanonDirection dir, int tank_id);
        std::pair<int, int> getLocation() const;
        void setLocation(std::pair<int, int> location);
        void setFlyingDirection(CanonDirection direction);
        CanonDirection getFlyingDirection() const ;
        std::pair<int, int> getNextLocation() const;
        void setNextLocation(std::pair<int, int> next_location);
        int getTankID() const;
        void setPrevLocation(std::pair<int, int> cur_location);
        std::pair<int,int> getPrevLocation() const;
   
};

#endif
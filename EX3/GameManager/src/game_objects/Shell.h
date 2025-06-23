#ifndef SHELL_H
#define SHELL_H
#include "GameObject.h"
#include "../../../UserCommon/CanonDirection.h"
#include <vector>
#include <utility> // for std::pair


class Shell: public GameObject {
    private:
        // locations of the shells are needed to check collision, since shells move faster than tanks.
        std::pair<int, int> location;
        std::pair<int, int> next_location;
        std::pair<int, int> previous_location;

        // shells flying direction, same as the tank's canon when shooted.
        CanonDirection flying_direction;

    public:
        Shell(std::pair<int, int> loc, CanonDirection dir);

        std::pair<int, int> getLocation() const;
        void setLocation(std::pair<int, int> location);

        std::pair<int, int> getNextLocation() const;
        void setNextLocation(std::pair<int, int> next_location);

        std::pair<int,int> getPrevLocation() const;
        void setPrevLocation(std::pair<int, int> cur_location);

        CanonDirection getFlyingDirection() const ;
        void setFlyingDirection(CanonDirection direction); 
        
};

#endif
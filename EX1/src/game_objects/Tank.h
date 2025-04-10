#ifndef TANK_H
#define TANK_H
#include "GameObject.h"
#include "Shell.h"
#include <vector>



class Tank : public GameObject
{
    private:
        int location_x;
        int location_y;
        CanonDirection canon_direction;
        Shell  unused_shells[16];
        std::vector<Shell> flying_shells;
        std::vector<Shell> dead_shells;
        int last_shoot_turn_index;
        int last_backwards_turn_index;
        bool alive;

    public:
        Tank(int x, int y, CanonDirection direction);
        int getLocationX() const;
        int getLocationY() const;
        void setLocation(int x, int y);
        void setLastShootTurnIndex(int index);
        int getLastShootTurnIndex() const;
        void setLastBackwardsTurnIndex(int index);
        int getLastBackwardsTurnIndex() const;
        CanonDirection getCanonDirection() const;
        void setCanonDirection(CanonDirection direction);
        void setAlive();
        bool getAlive();

};



#endif
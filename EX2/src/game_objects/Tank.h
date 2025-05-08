#ifndef TANK_H
#define TANK_H
#include "GameObject.h"
#include "Shell.h"
#include <vector>
#include "../common/TankAlgorithm.h"


class Tank : public GameObject
{
    private:
        int id;
        bool alive;
        int location_x;
        int location_y;
        CanonDirection canon_direction;
        int unused_shells_count;
        // currently flying shells shooted by this tank
        // TODO - should be unique ptr to shell
        std::vector<Shell *> flying_shells;
        std::unique_ptr<TankAlgorithm> tank_algorithm;

    public:
        Tank(int x, int y, CanonDirection direction, int id, int shells_count, std::unique_ptr<TankAlgorithm> tank_algorithm);
        Tank(){};
        Tank (const Tank&) = delete;
        ~Tank();

        int getId() const;

        bool getAlive();
        void setAlive();

        int getLocationX() const;
        int getLocationY() const;
        void setLocation(int x, int y);
  
        CanonDirection getCanonDirection() const;
        void setCanonDirection(CanonDirection direction);
        
        int getUnusedShellsCount() const;
        void setUnusedShellsCount(int count) ;

        // TODO - should work with unique ptr
        const std::vector<Shell *>& getFlyingShells() const;
        void addFlyingShell(Shell * shell);
        void deleteShell(Shell * shell);

        TankAlgorithm& getTankAlgorithm() const;
};

#endif
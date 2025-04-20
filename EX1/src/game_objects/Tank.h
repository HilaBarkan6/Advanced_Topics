#ifndef TANK_H
#define TANK_H
#include "GameObject.h"
#include "Shell.h"
#include <vector>



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
        std::vector<Shell *> flying_shells;
        
    public:
        Tank(int x, int y, CanonDirection direction, int id);
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

        const std::vector<Shell *>& getFlyingShells() const;
        void addFlyingShell(Shell * shell);
        void deleteShell(Shell * shell);
};

#endif
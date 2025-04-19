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
        std::vector<Shell *> flying_shells;
        int unused_shells_count;
       
        bool alive;

    public:
        Tank(int x, int y, CanonDirection direction);
        Tank (const Tank&) = delete;
        Tank& operator= (const Tank&) = delete;

        ~Tank() override;
        int getLocationX() const;
        int getLocationY() const;
        void setLocation(int x, int y);
  

        CanonDirection getCanonDirection() const;
        void setCanonDirection(CanonDirection direction);
        void setAlive();
        bool getAlive();

        int getUnusedShellsCount() const;
        void setUnusedShellsCount(int count) ;

        std::vector<Shell *>& getFlyingShells();

        void deleteShell(Shell * shell);


       

};



#endif
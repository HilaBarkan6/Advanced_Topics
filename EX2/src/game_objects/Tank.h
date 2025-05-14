#ifndef TANK_H
#define TANK_H
#include "GameObject.h"
#include "Shell.h"
#include <vector>
#include <memory>
#include "../common/TankAlgorithm.h"


class Tank : public GameObject
{
    private:
        int player_id;
        int tank_index;
        bool alive;
        int location_x;
        int location_y;
        CanonDirection canon_direction;
        int unused_shells_count;

        std::unique_ptr<TankAlgorithm> tank_algorithm;

    public:
        Tank(int x, int y, CanonDirection direction, int player_id, int tank_index, int shells_count, std::unique_ptr<TankAlgorithm> tank_algorithm);
        Tank(){};

        Tank(Tank&& other) noexcept
            : GameObject(std::move(other)), 
            player_id(other.player_id),
            tank_index(other.tank_index),
            alive(other.alive),
            location_x(other.location_x),
            location_y(other.location_y),
            canon_direction(other.canon_direction),
            unused_shells_count(other.unused_shells_count),
            tank_algorithm(std::move(other.tank_algorithm)) {}

        Tank& operator=(Tank&& other) noexcept {
            if (this != &other) {
                GameObject::operator=(std::move(other)); 
                player_id = other.player_id;
                tank_index = other.tank_index;
                alive = other.alive;
                location_x = other.location_x;
                location_y = other.location_y;
                canon_direction = other.canon_direction;
                unused_shells_count = other.unused_shells_count;
                tank_algorithm = std::move(other.tank_algorithm);
            }
            return *this;
        }

        Tank (const Tank&) = delete;
        Tank& operator=(const Tank&) = delete;
        
        ~Tank(){};

        int getPlayerId() const;
        int getTankIndex() const;

        bool getAlive() const;
        void setAlive();

        int getLocationX() const;
        int getLocationY() const;
        void setLocation(int x, int y);
  
        CanonDirection getCanonDirection() const;
        void setCanonDirection(CanonDirection direction);
        
        int getUnusedShellsCount() const;
        void setUnusedShellsCount(int count) ;

        TankAlgorithm& getTankAlgorithm() const;
};

#endif
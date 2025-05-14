#ifndef SIMPLE_PLAYER_FACTORY_H
#define SIMPLE_PLAYER_FACTORY_H
#include "../common/PlayerFactory.h"

class SimplePlayerFactory : public PlayerFactory {
    public:
        virtual unique_ptr<Player> create(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells ) const override;
};

#endif
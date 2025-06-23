#ifndef SIMPLE_PLAYER_FACTORY_H
#define SIMPLE_PLAYER_FACTORY_H
#include "../common/PlayerFactory.h"
#include "SimplePlayer.h"
#include "RadiusPlayer.h"
#include "../configuration/Config.h"

class SimplePlayerFactory : public PlayerFactory {
    private:
        static constexpr int DEFAULT_RADIUS = 3;
        static constexpr const char* DEFAULT_PLAYER_1_TYPE = "radius";
        static constexpr const char* DEFAULT_PLAYER_2_TYPE = "simple";

        std::string player_1_type; // Type of player 1, can be "simple" or "radius"
        std::string player_2_type; // Type of player 2, can be "simple" or "radius"
        int radius_value;

    public:
        virtual ~SimplePlayerFactory() = default;
        SimplePlayerFactory();
        virtual unique_ptr<Player> create(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells ) const override;
};

#endif
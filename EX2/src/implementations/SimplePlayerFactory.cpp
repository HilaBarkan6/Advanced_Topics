#include "SimplePlayerFactory.h"

// SimplePlayerFactory::SimplePlayerFactory() {}
// SimplePlayerFactory::~SimplePlayerFactory() {}  

// SimplePlayerFactory::SimplePlayerFactory(const Config& config)
//     : player_type(config.get("player_type", DEFAULT_PLAYER_TYPE)),
//       radius_value(config.getInt("radius_value", DEFAULT_RADIUS)) {}

unique_ptr<Player> SimplePlayerFactory::create(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells ) const {
    return std::make_unique<RadiusPlayer>(player_index, x, y, max_steps, num_shells, 3); // magic number 3 for radius, add to config file later
}

// std::unique_ptr<Player> SimplePlayerFactory::create(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) const {
//     if (player_type == "radius") {
//         return std::make_unique<RadiusPlayer>(player_index, x, y, max_steps, num_shells, radius_value);
//     } 
    
//     else {
//         return std::make_unique<SimplePlayer>(player_index, x, y, max_steps, num_shells);
//     }
}
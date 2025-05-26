#include "SimplePlayerFactory.h"
#include "SimplePlayer.h"
#include "RadiusPlayer.h"

// SimplePlayerFactory::SimplePlayerFactory() {}
// SimplePlayerFactory::~SimplePlayerFactory() {}  
unique_ptr<Player> SimplePlayerFactory::create(int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells ) const {
    return std::make_unique<RadiusPlayer>(player_index, x, y, max_steps, num_shells, 2);
}
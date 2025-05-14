#include "SatelliteViewImp.h"

char SatelliteViewImp::getObjectAt(size_t x, size_t y) const {
    if (x >= rows || y >= columns) {
        return '&';
    }
    if (called_tank_location.first == static_cast<int>(x) && called_tank_location.second == static_cast<int>(y)) {
        return '%';
    }
    return satellite_view[x][y];
    
    
}

void SatelliteViewImp::setCalledLocation(std::pair<int, int> location) {
    called_tank_location = location;
}

void SatelliteViewImp::setSatelliteView(const std::vector<std::vector<char>>& view) {
    satellite_view = view;
}

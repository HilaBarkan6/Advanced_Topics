#include "SatelliteViewImp.h"

char SatelliteViewImp::getObjectAt(size_t x, size_t y) const {
    if (x >= rows || y >= columns) {
        return '&';
    }
    return satellite_view[x][y];
      
}



void SatelliteViewImp::setSatelliteView(const std::vector<std::vector<char>>& view) {
    satellite_view = view;
}

void SatelliteViewImp::setCharAtLocation(const std::pair<int, int>& location, const char&& char_to_put) {
    satellite_view[location.first][location.second] = char_to_put;
}
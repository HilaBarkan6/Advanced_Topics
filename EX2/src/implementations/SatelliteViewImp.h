#ifndef SATELLITE_VIEW_IMP_H
#define SATELLITE_VIEW_IMP_H    
#include "../common/SatelliteView.h"
#include "../Board.h"
#include "../game_objects/Shell.h"
#include "../game_objects/Tank.h"

class SatelliteViewImp : public SatelliteView {
    // TODO - this class should only keep copies and not refrences to the real thing so it could reflect to players the state before this turn and not in the middle of it.
    public:
        SatelliteViewImp(size_t rows, size_t columns) 
            : rows(rows), columns(columns), called_tank_location(std::make_pair(-1,-1)) {};
        char getObjectAt(size_t x, size_t y) const override;
        void setCalledLocation(std::pair<int, int> location);
        void setSatelliteView(const std::vector<std::vector<char>>& view);

    private:
        size_t rows;
        size_t columns;
        std::pair<int, int> called_tank_location;
        std::vector<std::vector<char>> satellite_view;

};

#endif
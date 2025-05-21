#ifndef SATELLITE_VIEW_IMP_H
#define SATELLITE_VIEW_IMP_H    
#include "../common/SatelliteView.h"
#include "../Board.h"
#include "../game_objects/Shell.h"
#include "../game_objects/Tank.h"

class SatelliteViewImp : public SatelliteView {
    // TODO - this class should only keep copies and not refrences to the real thing so it could reflect to players the state before this turn and not in the middle of it.
    public:
        SatelliteViewImp() {};
        char getObjectAt(size_t x, size_t y) const override;
        void setSatelliteView(const std::vector<std::vector<char>>& view);
        void setCharAtLocation(const std::pair<int, int>& location, const char&& char_to_put);
        void setRowsAndColumns(size_t rows, size_t columns);

    private:
        size_t rows;
        size_t columns;
        std::vector<std::vector<char>> satellite_view;

};

#endif
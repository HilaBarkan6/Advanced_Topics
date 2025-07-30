#ifndef SATELLITE_VIEW_IMP_H
#define SATELLITE_VIEW_IMP_H    
#include "../../common/SatelliteView.h"
#include "Board.h"
#include "game_objects/Shell.h"
#include "game_objects/Tank.h"
#include "../../UserCommon/configuration/Config.h"

class SatelliteViewImp : public SatelliteView {
    private:
        static constexpr const char OUT_OF_BOUNDS_SIGN = '&'; 

        size_t rows;
        size_t columns;
        std::vector<std::vector<char>> satellite_view;
        char out_of_bounds_sign;

    public:
        SatelliteViewImp();

        // Constructor to initialize from a base-class SatelliteView
        SatelliteViewImp(const SatelliteView& original);

        // Rule of 5
        ~SatelliteViewImp() override = default;
        SatelliteViewImp& operator=(const SatelliteViewImp& other) = delete;
        SatelliteViewImp(SatelliteViewImp&& other) = delete;
        SatelliteViewImp& operator=(SatelliteViewImp&& other) = delete;

        char getObjectAt(size_t x, size_t y) const override;
        void setSatelliteView(const std::vector<std::vector<char>>& view);
        void setCharAtLocation(const std::pair<int, int>& location, const char& char_to_put); // Sets a character at the specified location in the satellite view
        void setRowsAndColumns(size_t rows, size_t columns); 
};

#endif
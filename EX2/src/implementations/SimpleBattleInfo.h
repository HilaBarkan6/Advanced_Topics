#ifndef SIMPLEBATTLEINFO_H
#define SIMPLEBATTLEINFO_H
#include "../common/BattleInfo.h"
#include <vector>

class SimpleBattleInfo : public BattleInfo {
    private:
        std::vector<std::pair<int, int>> tanks1_locations;
        std::vector<std::pair<int, int>> tanks2_locations;
        std::pair<int, int> called_tank_location;
        std::vector<std::pair<int, int>> shells_locations;
        std::vector<std::pair<int, int>> mines_locations;
        std::vector<std::pair<int, int>> walls_locations;

    public:
        SimpleBattleInfo() {};
        const std::vector<std::pair<int, int>>& getTanks1Locations() const;
        const std::vector<std::pair<int, int>>& getTanks2Locations() const;
        const std::pair<int, int>& getCalledTankLocation() const;
        const std::vector<std::pair<int, int>>& getShellsLocations() const;
        const std::vector<std::pair<int, int>>& getMinesLocations() const; 
        const std::vector<std::pair<int, int>>& getWallsLocations() const; 

        void setTanks1Locations(const std::vector<std::pair<int, int>>& tanks);
        void setTanks2Locations(const std::vector<std::pair<int, int>>& tanks);
        void setCalledTankLocation(const std::pair<int, int>& location);
        void setShellsLocations(const std::vector<std::pair<int, int>>& shells);  
        void setMinesLocations(const std::vector<std::pair<int, int>>& mines);
        void setWallsLocations(const std::vector<std::pair<int, int>>& walls);
};

#endif
#include "SimplePlayer.h"
#include <iostream>
#include <vector>
#include <utility> // for std::pair

void::SimplePlayer::updateTankWithBattleInfo(TankAlgorithm& tank, SatelliteView& satellite_view) {
    // create some battle info and pass it to the tank algorithm
    // use satellite view function getObjectAt to get all objects on the board and update battleinfo according to that
    std::vector<std::pair<int, int>> tanks1_locations;
    std::vector<std::pair<int, int>> tanks2_locations;
    std::pair<int, int> called_tank_location;
    std::vector<std::pair<int, int>> shells_locations;
    std::vector<std::pair<int, int>> mines_locations;
    std::vector<std::pair<int, int>> walls_locations;
    // Iterate over the board using satellite_view and populate battle_info
    for (int x = 0; x < height; ++x) {
        for (int y = 0; y < width; ++y) {
            char object_sign = satellite_view.getObjectAt(x, y);
            switch(object_sign) {
                case '1': // Tank1
                    tanks1_locations.emplace_back(std::make_pair(x, y));
                    break;
                case '2': // Tank2
                    tanks2_locations.emplace_back(std::make_pair(x, y));
                    break;
                case '%': // Called tank
                    called_tank_location = std::make_pair(x, y);
                    break;
                case '*': // Shell
                    shells_locations.emplace_back(std::make_pair(x, y));
                    break;
                case '@': // Mine
                    mines_locations.emplace_back(std::make_pair(x, y));
                    break;
                case '#': // Wall
                    walls_locations.emplace_back(std::make_pair(x, y));
                    break;
                default:
                    break; // Ignore other objects
            }
        }
    }
    battle_info.setTanks1Locations(tanks1_locations);
    battle_info.setTanks2Locations(tanks2_locations);
    battle_info.setCalledTankLocation(called_tank_location);
    battle_info.setShellsLocations(shells_locations);   
    battle_info.setMinesLocations(mines_locations);
    battle_info.setWallsLocations(walls_locations);

    // Update the tank algorithm with the populated battle info 
    tank.updateBattleInfo(battle_info);
    std::cout << "call to updateTankWithBattleInfo "<< std::endl;
}
#ifndef SATELLITE_VIEW_IMP_H
#define SATELLITE_VIEW_IMP_H    
#include "../common/SatelliteView.h"
#include "../Board.h"
#include "../game_objects/Shell.h"
#include "../game_objects/Tank.h"

class SatelliteViewImp : public SatelliteView {
    // TODO - this class should only keep copies and not refrences to the real thing so it could reflect to players the state before this turn and not in the middle of it.
    public:
        SatelliteViewImp(size_t rows, size_t columns, const Board & game_board, const std::vector<Shell> & game_flying_shells, const std::vector<Tank> & game_all_tanks) 
            : rows(rows), columns(columns), called_player_id(-1), called_tank_index(-1), game_board(game_board), game_flying_shells(game_flying_shells), game_all_tanks(game_all_tanks) {};
        char getObjectAt(size_t x, size_t y) const override;
        void setCalledPlayerId(int player_id);
        void setCalledTankIndex(int tank_index);

    private:
        size_t rows;
        size_t columns;
        int called_player_id;
        int called_tank_index;
        const Board game_board;
        const std::vector<Shell> & game_flying_shells;
        const std::vector<Tank> & game_all_tanks;

};

#endif
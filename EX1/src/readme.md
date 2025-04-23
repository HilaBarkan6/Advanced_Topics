This is our version of the tank game of Advanced Topics in Programming course.
Yovel Hatan - 209399021
Hila Barkan - 208239152

Overview - 
    In the project we implemented 2 types of players.
    One **Defensive** - Shoot if possible but otherwise escaping and try to avoid the enemy and shells.
    Second **Offensive** - Preforms BFS to chase the opponent's tank, preforms only the first action in the best path for a position in which it can shoot the opponent.

Input file format - 
    First row is expected to be **height** (number of rows).
    Second row is expected to be **width** (number of columns).
    Then the board itself as explained in the assignment. (see example in input/input1.txt)

Output file - 
    Will be created in output directory.
    The content is full documentation of the actions preformed by the players and game states.

Implement more types of Players - 
    New players should inherit from abstract "Player" class and implement "getAction" function.
    To use them, include them in the main file, create instance and send them to GameManager as unique_ptr.

Notes - 
    We assume board and players were designed to create a reasonable game that should be finished.
    If the game designed to not finish, we allow it. For example - if both players stays in place and just rotating, the game will run forever.
    Game can be finished only by the conditions described in the assignment.
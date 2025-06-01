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

Input and Output examples - 
    Many are provided in "tests" directory, see more in "testing approach" in the HLD document.
    Selected 3 boards are also in "input" directory with matching game flow in "output" directory
    In these 3 boards, player 1 is defensive and player 2 is offensive. In inputs_a and input_c player 2 wins, in input_b player 1 wins.
    Also notes these are the same boards as tests/inputs_9, tests/inputs_11, tests/inputs_12 so for more details see the "testing approach" in the HLD document.

Notes - 
    We assume board and players were designed to create a reasonable game that should be finished.
    If the game designed to not finish, we allow it. For example - if both players stays in place and just rotating, the game will run forever.
    Game can be finished only by the conditions described in the assignment.

Whats Next-
SimplePlayer - will send to its tanks SimpleBattleInfo - tanks will act offensive with no cordination.
We need to implement a version on Offensive tank algorithm (similar to the BFS from EX1)

SmartPlayer - will divide the board to areas for each tank and will sent each tank only the relevant battleinfo to it's area
each tank will act offensive in their area.
We need to implemet - the player, tank algoritm, maybe battle info but not sure

Also need to update PlayerFactory and TankAlgorithmFactory to create the correct object for each player
Algorithms should think about what to keep from battle info and how often to request a new battle info

Whats Next 2 -
Tests on input file edge cases and errors
If one of the player doesn't have any tanks at all, he loses. make sure to do so instead of throwing error (this is the behevior now) - done
If both of them don't have tanks at all so tie - done
When Game ends - if both players are out of shells and 40 turns passed need to change the line we write to the output file - done
Add a test when action is written with ignored and killed in the same turn - done, test j, needed small change in rotateTankAlgorithm to cause the stuipd tank to move forward and get ignored.
Make sure no new and delete - done, only place with "new" was in main.cpp so changed that to make_unique, not sure if std::move was needed there but this works
Clean the code - remove duplications, short function, rule of 5, all common mistakes, magic numbers
Move read_board from GameManager to Board
Logging for bonus and maybe configuration file
Mabye more tests
Check what going on in the forum
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
**If one of the player doesn't have any tanks at all, he loses. make sure to do so instead of throwing error (this is the behevior now) - done**
**If both of them don't have tanks at all so tie - done**
**When Game ends - if both players are out of shells and 40 turns passed need to change the line we write to the output file - done**
**Add a test when action is written with ignored and killed in the same turn - done**
**Make sure no new and delete - done**
**Move read_board from GameManager to Board - done**
**configuration file - done**
**remove duplications - done**
**short function - done**
**magic numbers - done (need to go over it again together)**
**Logging for bonus - done**
**Clean the code - rule of 5 - done**
**GTest for bonus - done**
**all common mistakes - done**
**We need to make sure that in every case of polymorphism, we declare a virtual destructor in the base class - done**
**according to lecture 8, the only place where we can make down cast is in BattleInfo unless there is something from our implementation - done**
**check if we made const in order to protect smart pointers value and if we did, check if the const is inside the <> brackets - done**
**Tests on input file edge cases and errors - done**
**Mabye more tests - done**
**write a bonous file - done**

Check what going on in the forum - each one of us
update readme - Hila
make sure it works on nova server - Yovel

_______________________
NEW README
_______________________

This is our version for EX2 of the tank game of "Advanced Topics in Programming" course.
Yovel Hatan - 209399021
Hila Barkan - 208239152

Overview - 
    In this project, we implemented the required classes and adapted the game to support multiple tanks for each player.
    Additionally, we added some bonus features: logging, configuration, and tests.
    The main code is located in the /src directory. For bonus features, see the file bonus.txt.

Implementatins - 
    SatelliteView - We implemented SimpleSatelliteView, which maintains a matrix of characters received from the GameManager and uses it to implement the getObjectAt function.

    BattleInfo - We implemented SimpleBattleInfo, which stores location vectors for: walls, mines, shells, tanks of player 1, tanks of player 2, and the position of the calling tank (i.e., the tank requesting the battle info).

    Players - We implemented two types of players:
        SimplePlayer: When the GameManager calls updateTankWithBattleInfo(), this player creates a SimpleBattleInfo containing **all** data from the SatelliteView, and then passes it to the corresponding tank algorithm via updateBattleInfo().

        RadiusPlayer: Also implements updateTankWithBattleInfo(), but only provides data within a specified radius from the requesting tank. This avoids sending irrelevant data from distant parts of the board, simplifying communication and improving efficiency.

    Both players share some common functionality, so they inherit from the PlayerImp class.

    TankAlgorithm - We implemented two tank algorithms:
        BFSTankAlgorithm: Every three turns, it requests battle info, then uses it to locate the nearest enemy and runs a BFS algorithm to determine the best next two moves to shoot at that enemy. It then executes these moves in the next two turns before requesting info again.

        RotatingTankAlgorithm: A simple algorithm used mainly for testing. It only rotates in place and typically loses unless the opponent makes critical mistakes.

    Factories - 
        SimpleTankAlgorithmFactory: Creates a BFSTankAlgorithm for player 1 and a RotatingTankAlgorithm for player 2.
        SimplePlayerFactory: Creates a player instance based on the type specified in the configuration file. If no type is specified, the default is SimplePlayer.

How to Run - 
    The GameManager should receive the two factories via its constructor.
    In main.cpp, we create them using make_unique.
    To use different factories, simply modify line 24 in main.cpp—and that's it!

Input and Output examples -
    We provide three input and output files in the /input and /output directories.
    See bonus.txt for additional test inputs.
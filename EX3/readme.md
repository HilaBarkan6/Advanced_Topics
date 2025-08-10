This is our version for EX2 of the tank game of "Advanced Topics in Programming" course.
Yovel Hatan - 209399021
Hila Barkan - 208239152

**Overview -**
    In this project, we implemented the required classes and adapted the game to support multiple tanks for each player.
    Additionally, we added some bonus features: logging, configuration, and tests.
    The main code is located in the /src directory. For bonus features, see the file bonus.txt.

**Implementatins -**
    SatelliteView - We implemented SimpleSatelliteView, which maintains a matrix of characters received from the GameManager and uses it to implement the getObjectAt function.

    BattleInfo - We implemented SimpleBattleInfo, which stores location vectors for: walls, mines, shells, tanks of player 1, tanks of player 2, and the position of the calling tank (i.e., the tank requesting the battle info).

    Players - We implemented two types of players:
        SimplePlayer: When the GameManager calls updateTankWithBattleInfo(), this player creates a SimpleBattleInfo containing **all** data from the SatelliteView, and then passes it to the corresponding tank algorithm via updateBattleInfo().

        RadiusPlayer: Also implements updateTankWithBattleInfo(), but only provides data within a specified radius from the requesting tank. This avoids sending irrelevant data from distant parts of the board, simplifying communication and improving efficiency.

    Both players share some common functionality, so they inherit from the PlayerImp class.

    TankAlgorithm - We implemented two tank algorithms:
        BFSTankAlgorithm: Every three turns, it requests battle info, then uses it to locate the nearest enemy and runs a BFS algorithm to determine the best next two moves to shoot at that enemy. It then executes these moves in the next two turns before requesting info again.

        SimpleTankAlgorithm : Every second turn, it requests batlle info, then it uses it to locate the nearest enemy. If it has a clear shooting line it shoots at the enemy, if not, if moving forward is a good idea (meaning no wall, mines, etc) it moves forward and if not then it rotates so maybe next time moving will be possible.

        RotatingTankAlgorithm: A simple algorithm used only for testing. It only rotates in place and typically loses unless the opponent makes critical mistakes.
    
    BFSTankAlgorithm and SimpleTankAlgorithm share some common functionality, so they inherit from the TankAlgorithmImp class.

    Factories - 
        SimpleTankAlgorithmFactory: Creates a BFSTankAlgorithm for player 1 and a SimpleTankAlgorithm for player 2.
        SimplePlayerFactory: Creates a player instance based on the type specified in the configuration file. 
                             The current configuration is RadiusPlayer for player1 and SimplePlayer for player2.
                             This setup creates intresting games, since player1 tank's are smart and use BFS, but they only see data within a small radius, and player2 tank's have a much simpler algorithm, but they see the whole board.

**How to Run -**
    The GameManager should receive the two factories via its constructor.
    In main.cpp, we create them using make_unique.
    To use different factories, simply modify line 24 in main.cpp—and that's it!

**Input and Output examples -**
    We provide three input and output files in the /input and /output directories.
    See bonus.txt for additional test inputs.


**Building Instructions**
    To compile the project, make sure you have a C++20-compatible compiler (e.g., g++) and the GoogleTest library installed.
    - Build the main game executable:
        make
        This will produce the main executable: tank_game.exe
    - Build and run the tests:
        make test
        ./test_runner.exe
    - Build the main game executable and the test executable:
        make all
    - Clean build files:
        make clean

**Notes:**
    - For backward movement:
                        - We consider the current turn in which we call backward as a turn we wait.
                          For example, if tank requests backward in turn 1, it can move in turn 3 (because turns 1 and 2 are used for waiting).
                        - In the turn when backward should be applied (in the last example is turn 3), if the tank requests      
                          MoveForward or GetBattleInfo, the backward movement will not be applied and the the one of such actions will be apllied. Otherwise the action will be ignored and the  backward movement will  be applied (As disccused in the forum). 
                          For example: tank asks for backward in turn 1, and in turn 3 asks for rotate action, the rotation will not be applied since in turn 3 the backward movement will be applied.
                          Another example: tank asks for backward in turn 1 and in turn 3 asks for forward movement, the movement will be applied in turn 3 and the backword movement won't.


What to do next time - 
Simulator runners should send the game manager satellite view and not matrix of chars
Game manager factory should return a real game manager and not abstract
Our game manager should implement the abstract game manager properly - this changes tanks and board creation
game manager and algorithm should use the macro to load themselves to the regitrars
Make sure output files are written to the correct place
decide what to do with the logger
Make our algorithms good


What's next:
- build Simulator with the general makefile and see it works run with the command line - yovel **done**
- Make sure default configuration works - Hila **done** deleted the config file localy and everything worked well
- Error handaling - bad parameters -Hila **done**
- Make sure each function is not too long - Yovel **done**
- Make sure there are no magic numbers - Yovel **done**
- Fix "hello_map" - Hila **done**
- Run comparative mode with same algorithms pointing to the same file - Yovel **done**
- threads **done**
- Comparative section 4 - Hila **done**
- Make sure in competition mode, mathces are generated properly - Yovel **done**
- Competitive section 2 - Yovel **done**
- Competitive section 5 - Hila **done**
- Additional output and error handaling - make sure everything works as wrriten there - Hila **done** 
- student.txt and readme - Hila
- Make sure all dll's closed at finish - Yovel **done**
- Fix congureation file path in main - Yovel **done**
- Delete commented functions **done**
- Look on TODO things and CR things - Yovel
- Delete files we don't need (for example simple player, or maybe remove it to another folder) **done**
- Use system.clock instead of time **done**
- Make sure comments are correct - both
- Go over forum to make sure we didn't miss anything - both
- Clean code again - Yovel
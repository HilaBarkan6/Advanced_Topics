This is our version for EX3 of the tank game of "Advanced Topics in Programming" course.
Yovel Hatan - 209399021
Hila Barkan - 208239152

**Overview -**
    This project consists of three main components: Game Manager, Algorithm, and Simulator.
    Additionally, there is a UserCommon project that provides logging and configuration utilities, and Implementation of SatelliteView (relevant for both GameManager and Algorithm).
    The main source code for each component is located in its respective src/ directory.
    See bonus.txt for requested bonus for some features seen in the forum.

**Implementatins -**
    Algorithm-
    Implemented SimpleBattleInfo, which stores location vectors for: walls, mines, shells, tanks of player 1, tanks of player 2, and the position of the requesting tank (i.e., the tank that requested the battle info).

    Implemented Player (Player_209339021_208239152):
    When the Game Manager calls updateTankWithBattleInfo(), this player creates a SimpleBattleInfo object containing all data from the SatelliteView, then passes it to the corresponding tank algorithm via updateBattleInfo().

    Implemented TankAlgorithm (TankAlgorithm_209339021_208239152):
    Every three turns, it requests battle info, locates the nearest enemy, and runs a BFS algorithm to determine the best next two moves to shoot at that enemy. It then executes these moves over the next two turns before requesting info again.
    While calculating, it tracks moving shells and, when enough data is available to determine their direction, avoids them while choosing the shortest path to attack the enemy without collision.

    The Player and TankAlgorithm inherit from the base classes PlayerImp and TankAlgorithmImp, which provide general reusable functionality for other player or tank algorithm implementations.

    GameManager - 
    Implemented according to the assignment specification.

    Simulator - 
    Implemented according to the assignment specification.


    UserCommon - 
    Implemented SatelliteViewImp, which maintains a character matrix received from the Game Manager and uses it to implement the getObjectAt() function.
    Implemented logging and configuration file handling.
    

**Building Instructions**
    To build the entire project, use the Makefile in the main directory.
    To build an individual component, use the Makefile located in that component’s directory.

**How to Run -**
    Build the Simulator project and run it with the arguments specified in the assignment.


**Notes:**
    - For backward movement:
                        - We consider the current turn in which we call backward as a turn we wait.
                          For example, if tank requests backward in turn 1, it can move in turn 3 (because turns 1 and 2 are used for waiting).
                        - In the turn when backward should be applied (in the last example is turn 3), if the tank requests      
                          MoveForward or GetBattleInfo, the backward movement will not be applied and the the one of such actions will be apllied. Otherwise the action will be ignored and the  backward movement will  be applied (As disccused in the forum). 
                          For example: tank asks for backward in turn 1, and in turn 3 asks for rotate action, the rotation will not be applied since in turn 3 the backward movement will be applied.
                          Another example: tank asks for backward in turn 1 and in turn 3 asks for forward movement, the movement will be applied in turn 3 and the backword movement won't.




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
- Look on TODO things and CR things - Yovel **done**
- Delete files we don't need (for example simple player, or maybe remove it to another folder) **done**
- Use system.clock instead of time **done**
- Make sure comments are correct - both
- Go over forum to make sure we didn't miss anything - both
- Clean code again - Yovel **done**
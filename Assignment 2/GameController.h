#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:

    void check_for_points(BlockFall& game);
    void print_grid(BlockFall& game, bool end, bool clearing);
    void use_gravity(BlockFall& game);
    void drop(BlockFall& game);
    void rotate(BlockFall& game, bool to_right);
    void move(BlockFall& game, bool to_right);
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
};


#endif //PA2_GAMECONTROLLER_H

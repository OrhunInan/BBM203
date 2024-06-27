#include "BlockFall.h"
#include "GameController.h"
#include <iostream>

int main(int argc, char **argv) {

    // Create a BlockFall instance
    string gravity_mode("GRAVITY_OFF");
    BlockFall game("../sampleIO/1_big_grid_gravity_switch/grid.dat",
                   "../sampleIO/1_big_grid_gravity_switch/blocks.dat",
                   false,
                   "../sampleIO/leaderboard.txt",
                   "alperen");

    // Create a GameController instance
    GameController controller;

    // Play
    controller.play(game,"../sampleIO/1_big_grid_gravity_switch/commands.dat");
    return 0;
}
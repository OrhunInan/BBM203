#include "BlockFall.h"
#include <iostream>
#include <fstream>


BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on,
                     const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(gravity_mode_on),
                     leaderboard_file_name(leaderboard_file_name), player_name(player_name), rows(0), cols(0) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {

    std::ifstream file(input_file);
    std::string line;

    std::vector<std::vector<std::vector<bool>>> shapes;
    std::vector<std::vector<bool>> block_shape;
    std::vector<bool> block_line;

    while (getline(file, line)) {

        if (line.empty()) continue;

        for (char c : line) {

            switch (c) {

                case '1':

                    block_line.push_back(true);
                    continue;

                case '0':

                    block_line.push_back(false);
                    continue;


                case ']':

                    block_shape.push_back(block_line);
                    block_line.clear();
                    shapes.push_back(block_shape);
                    block_shape.clear();
                    continue;

                default:
                    continue;
            }
        }

        if (!block_line.empty()) {

            block_shape.push_back(block_line);
            block_line.clear();
        }
    }

    power_up = shapes[shapes.size() - 1];
    shapes.pop_back();

    Block* current_block = nullptr;

    for (vector<vector<bool>> &shape : shapes) {

        if (current_block == nullptr) current_block = initial_block = new Block{};
        else {
            current_block->next_block = new Block;
            current_block = current_block->next_block;
        }

        current_block->shape = shape;
        current_block->add_rotations();
    }

    file.close();
    active_block = initial_block;
    active_rotation = active_block;
}

void BlockFall::initialize_grid(const string &input_file) {
    std::ifstream file(input_file);
    std::string line;
    vector<int> grid_vector_line;

    while (std::getline(file, line)) {

        for (char c : line) {

            switch (c) {

                case '1':

                    grid_vector_line.push_back(1);
                    continue;

                case '0':

                    grid_vector_line.push_back(0);
                    continue;

                default:

                    continue;
            }
        }

        grid.push_back(grid_vector_line);
        grid_vector_line.clear();
    }

    rows = static_cast<int>(grid.size());
    cols = static_cast<int>(grid[0].size());
}

BlockFall::~BlockFall() {
    delete initial_block;
}
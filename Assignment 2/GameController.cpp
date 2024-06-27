#include <fstream>
#include "GameController.h"

void clear(BlockFall &game) {

    for (vector<int> const &line : game.grid) {

        for (int point : line) {

            game.current_score += point;
        }
    }

    game.current_score += 1000;
    game.grid.resize(game.rows, vector<int>(game.cols, 0));
}

bool if_and_only_if(bool statement_one, bool statement_two) {

    return (statement_one and statement_two) or (!statement_one and !statement_two);
}

bool contains(vector<int> &v, int element) {

    for (bool e : v) {

        if (e == element) return true;
    }

    return false;
}

bool check_collisions(BlockFall& game, long x, long y) {

    if (x < 0 or
        y < 0 or
        x + game.active_rotation->shape.size() > game.rows or
        y + game.active_rotation->shape[0].size() > game.cols or
        game.active_rotation == nullptr) return true;

    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {

        for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {

            if (game.grid[x + i][y + j] == 1 &&
                    game.active_rotation->shape[i][j]) return true;
        }
    }

    return false;
}

void GameController::check_for_points(BlockFall& game) {


    bool did_power_up(true);

    for (int i = 0; i <= game.grid.size() - game.power_up.size(); ++i) {

        for (int j = 0; j <= game.grid[0].size() - game.power_up[0].size(); ++j) {

            for (int k = 0; k < game.power_up.size(); ++k) {

                for (int l = 0; l < game.power_up[0].size(); ++l) {

                    if (!if_and_only_if(game.grid[i+k][j+l], game.power_up[k][l])) did_power_up = false;
                }
            }
        }
    }

    if (did_power_up) {

        print_grid(game, false, true);
        clear(game);
        return;
    }

    bool did_clear(false);
    for (int i = 0 ; i < game.grid.size() ; ++i) {

        if (contains(game.grid[i], 1)) {

            if (!contains(game.grid[i], 0)) {

                if(!did_clear) {

                    print_grid(game, false, true);
                    did_clear = true;
                }

                game.current_score += 10;
                game.grid.erase(game.grid.begin()+i);
                game.grid.insert(game.grid.begin(), vector<int>(game.grid[0].size(), 0));
            }
        }
    }
}

void GameController::print_grid(BlockFall& game, bool end, bool clearing) {

    if (clearing) {
        cout <<"Before clearing:\n";
    }
    else {
        if (game.leaderboard.head_leaderboard_entry == nullptr or
            game.leaderboard.head_leaderboard_entry->score <= game.current_score) {

            cout << "High Score: " << game.current_score << std::endl;
        } else {

            cout << "Score: " << game.current_score << std::endl << "High Score: " <<
                 game.leaderboard.head_leaderboard_entry->score << std::endl;
        }
    }

    std::vector<std::vector<int>> grid_copy = game.grid;

    if (!(game.active_rotation == nullptr or end or clearing)) {
        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {

            for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {

                if (game.active_rotation->shape[i][j])
                    grid_copy[game.current_block_position[0] + i][game.current_block_position[1] + j] = 1;
            }
        }
    }

    for (vector<int> const line : grid_copy) {


        for (int const point : line) {

            if (point == 1) cout << "⬜";
            else cout << "⬛";
        }
        cout << std::endl;
    }
    cout << std::endl;
    cout << std::endl;
}

void GameController::use_gravity(BlockFall& game){

    bool gravity_possible(true);
    while (gravity_possible){
        gravity_possible = false;

        for (int i = game.grid.size() - 1; i > 0; --i) {

            for (int j = 0; j < game.grid[i].size(); ++j) {

                if (game.grid[i][j] == 0 and game.grid[i - 1][j] == 1) {

                    game.grid[i][j] = 1;
                    game.grid[i - 1][j] = 0;
                    gravity_possible = true;
                }
            }
        }
    }
}

void GameController::drop(BlockFall& game) {

        while(!check_collisions(game, ++game.current_block_position[0], game.current_block_position[1])) {
        }
        game.current_block_position[0]--;


        int point_count(0);
        for (int i = 0; i < game.active_rotation->shape.size(); ++i) {

            for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {


                if(game.active_rotation->shape[i][j]) {

                    game.grid[game.current_block_position[0] + i][game.current_block_position[1] + j] = 1 ;
                    point_count++;
                }
            }
        }

        game.current_score += game.current_block_position[0] * point_count;

        game.current_block_position[0] = 0;
        game.current_block_position[1] = 0;

        game.active_block = game.active_block->next_block;
        game.active_rotation = game.active_block;
    }

void GameController::rotate(BlockFall& game, bool to_right) {

    if(to_right) {

        game.active_rotation = game.active_rotation->right_rotation;
        if (check_collisions(game, game.current_block_position[0], game.current_block_position[1]))
            game.active_rotation = game.active_rotation->left_rotation;
    }
    else {

        game.active_rotation = game.active_rotation->left_rotation;
        if (check_collisions(game, game.current_block_position[0], game.current_block_position[1]))
            game.active_rotation = game.active_rotation->right_rotation;
    }
}

void GameController::move(BlockFall& game, bool to_right) {

    if (to_right) {

        if(check_collisions(game, game.current_block_position[0],++game.current_block_position[1]))
            game.current_block_position[1]--;
    }
    else {

        if(check_collisions(game, game.current_block_position[0],--game.current_block_position[1]))
            game.current_block_position[1]++;
    }
}

bool GameController::play(BlockFall& game, const string& commands_file){

    std::ifstream file(commands_file);
    std::string line;

    while(file >> line) {


        // if (line == "PRINT_GRID") print_grid(game, false, false);
        if (line == "ROTATE_RIGHT") rotate(game, true);
        else if(line == "ROTATE_LEFT") rotate(game, false);
        else if (line == "MOVE_RIGHT") move(game, true);
        else if (line == "MOVE_LEFT") move(game, false);
        else if (line == "GRAVITY_SWITCH") game.gravity_mode_on = !game.gravity_mode_on;
        else if ((line == "DROP")) drop(game);

        if (game.gravity_mode_on) use_gravity(game);
        check_for_points(game);
        if (check_collisions(game, 0, 0)) {

        };
    }

    file.close();

    return true;
}




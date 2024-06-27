#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>
#include <iostream>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next_leaderboard_entry block to appear in the game

    bool operator==(const Block& other) const {
        return this->shape == other.shape;
    }

    bool operator!=(const Block& other) const {
        return !this->operator==(other);
    }

    ~Block(){

        if (right_rotation != nullptr && left_rotation != nullptr && right_rotation != this) {

            if (right_rotation->right_rotation == this) {

                right_rotation->right_rotation = nullptr;
                right_rotation->left_rotation = nullptr;
                delete right_rotation;
            }
            else {

                right_rotation->right_rotation->right_rotation = nullptr;
                right_rotation->right_rotation->left_rotation = nullptr;
                delete right_rotation->right_rotation;

                right_rotation->right_rotation = nullptr;
                right_rotation->left_rotation = nullptr;
                delete right_rotation;

                left_rotation->left_rotation = nullptr;
                left_rotation->right_rotation = nullptr;
                delete left_rotation;

            }
        }

        delete next_block;
    }

    void print(){ //todo: delete this
        for (vector<bool> v: shape) {

            for (bool e : v) {

                std::cout<<e;
            }
            std::cout<<std::endl;
        }
    }

    void add_rotations() {

        //initializing rotations
        int rows = static_cast<int>(shape.size());
        int cols = static_cast<int>(shape[0].size());
        Block* rotations[3] = {new Block{}, new Block{}, new Block{}};

        rotations[0]->shape.resize(cols, vector<bool>(rows, false));
        rotations[1]->shape.resize(rows, vector<bool>(cols, false));
        rotations[2]->shape.resize(cols, vector<bool>(rows, false));

        //adding shapes to rotations
        for (int i = 0; i < rows; ++i) {

            for (int j = 0; j < cols; ++j) {

                rotations[0]->shape[j][rows-1-i] = shape[i][j];
                rotations[1]->shape[rows-1-i][cols-1-j] = shape[i][j];
                rotations[2]->shape[cols-1-j][i] = shape[i][j];
            }
        }


        //checks for symmetry
        if (this->shape == rotations[0]->shape) {

            this->right_rotation = this;
            this->left_rotation = this;

            //deletes unnecessary rotations;
            for ( Block* rotation : rotations) {
                delete rotation;
            }
        }
        else {

            this->right_rotation = rotations[0];
            this->right_rotation->left_rotation = this;

            if (this->shape == rotations[1]->shape) {

                this->left_rotation = this->right_rotation;
                this->right_rotation->right_rotation = this;
                for (int i = 1; i < 3; ++i) {

                    delete rotations[i];
                }
            }
            else{

                this->right_rotation->right_rotation = rotations[1];
                this->right_rotation->right_rotation->left_rotation = this->right_rotation;
                this->right_rotation->right_rotation->right_rotation = rotations[2];
                this->right_rotation->right_rotation->right_rotation->left_rotation = this->right_rotation->right_rotation;
                this->right_rotation->right_rotation->right_rotation->right_rotation = this;
                this->left_rotation = this->right_rotation->right_rotation->right_rotation;
            }
        }
    }
};


#endif //PA2_BLOCK_H

#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {

    delete root;
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {

    ifstream file(filename);
    string line;
    int
    x, y, z;

    file >> line;
    while (file >> line) {

        replace(line.begin(), line.end(), ',', ' ');
        stringstream line_stream(line);
        line_stream >> x >> y >> z;

        insertSectorByCoordinates(x, y, z);
    }

    file.close();
}

void insert_sector(Sector* parent, Sector* new_sector) {

    if (
            (new_sector->x < parent->x)
            ||
            (new_sector->x == parent->x && new_sector->y < parent->y)
            ||
            (new_sector->x == parent->x && new_sector->y == parent->y && new_sector->z < parent->z)
            ) {

        if (parent->left != nullptr) insert_sector(parent->left, new_sector);
        else {

            new_sector->parent = parent;
            parent->left = new_sector;
        }
    }
    else {

        if (parent->right != nullptr) insert_sector(parent->right, new_sector);
        else {

            new_sector->parent = parent;
            parent->right = new_sector;
        }
    }
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {

    if (root == nullptr) root = new Sector(x, y, z);
    else insert_sector(root, new Sector(x, y, z));
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {

    if (root != nullptr) {

        Sector* old_sector = root->find_by_sector_code(sector_code);
        Sector* new_sector = nullptr; //successor of the old sector.

        if(old_sector != nullptr) {

            //case 1: both children of the sector is null.
            if(old_sector->left == nullptr && old_sector->right == nullptr) {

                if (old_sector->parent != nullptr) {

                    if (old_sector->parent->left == old_sector) old_sector->parent->left = nullptr;
                    else old_sector->parent->right = nullptr;
                } else root = nullptr;

                old_sector->parent = nullptr;
            }
            //case 2: sector has both a left child and a right child.
            else if (old_sector->left != nullptr && old_sector->right != nullptr) {

                new_sector = old_sector->right->get_minimum();//finds successor

                //makes the connections with parent of the sector
                if (old_sector->parent != nullptr) {
                    if (old_sector->parent->left == old_sector) old_sector->parent->left = new_sector;
                    else old_sector->parent->right = new_sector;
                }
                else root = new_sector;

                if(old_sector->right == new_sector) old_sector->right = new_sector->right;
                else new_sector->parent->left = new_sector->right;
                if (new_sector->right != nullptr) new_sector->right->parent = new_sector->parent;

                new_sector->left = old_sector->left;
                new_sector->left->parent = new_sector;
                new_sector->right = old_sector->right;
                if (new_sector->right != nullptr) new_sector->right->parent = new_sector;
                new_sector->parent = old_sector->parent;

                old_sector->parent = nullptr;
                old_sector->left = nullptr;
                old_sector->right = nullptr;
            }
            //case 3: section has only one child
            else {

                if(old_sector->left == nullptr) {

                    new_sector = old_sector->right;
                    old_sector->right = nullptr;
                }
                else {

                    new_sector = old_sector->left;
                    old_sector->left = nullptr;
                }

                new_sector->parent = old_sector->parent;

                //makes the connections with the parent of the sector
                if (old_sector->parent != nullptr) {

                    if (old_sector->parent->left == old_sector) old_sector->parent->left = new_sector;
                    else old_sector->parent->right = new_sector;
                } else root = new_sector;

                old_sector->parent = nullptr;
            }

            delete old_sector;
        }
    }
}

void print_in_order(Sector* sector){

    if(sector->left != nullptr) print_in_order(sector->left);
    cout << sector->sector_code << endl;
    if(sector->right != nullptr) print_in_order(sector->right);
}

void SpaceSectorBST::displaySectorsInOrder() {

    std::cout << "Space sectors inorder traversal:\n";
    print_in_order(root);
    std::cout << std::endl;
}

void print_pre_order(Sector* sector){

    cout << sector->sector_code << endl;
    if(sector->left != nullptr) print_pre_order(sector->left);
    if(sector->right != nullptr) print_pre_order(sector->right);
}

void SpaceSectorBST::displaySectorsPreOrder() {

    std::cout << "Space sectors preorder traversal:\n";
    print_pre_order(root);
    std::cout << std::endl;
}

void print_post_order(Sector* sector){

    if(sector->left != nullptr) print_post_order(sector->left);
    if(sector->right != nullptr) print_post_order(sector->right);
    cout << sector->sector_code << endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {

    std::cout << "Space sectors postorder traversal:\n";
    print_post_order(root);
    std::cout << std::endl;
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {

    std::vector<Sector*> path;
    Sector* node = nullptr;
    if (root != nullptr) node = root->find_by_sector_code(sector_code);

    if (node != nullptr) {

        node->get_path(path);
    }

    std::reverse(path.begin(), path.end());

    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    if (path.empty()) std::cout << "A path to Dr. Elara could not be found.\n";
    else {

        std::cout << "The stellar path to Dr. Elara: ";

        for (int i = 0; i < path.size(); ++i) {

            std::cout << path[i]->sector_code;

            if(i != path.size() - 1) std::cout << "->";
        }

        std::cout << std::endl;
    }
}


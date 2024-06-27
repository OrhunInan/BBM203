#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

SpaceSectorLLRBT::~SpaceSectorLLRBT() {

    delete root;
}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {

    ifstream file(filename);
    string line;
    int x, y, z;

    file >> line;
    while (file >> line) {

        replace(line.begin(), line.end(), ',', ' ');
        stringstream line_stream(line);
        line_stream >> x >> y >> z;

        insertSectorByCoordinates(x, y, z);
    }

    file.close();
}

Sector* SpaceSectorLLRBT::rotate_left(Sector* sector) {

    Sector* parent = sector->parent;
    Sector* child = sector->right;
    Sector* second_child = child->left;

    child->left = sector;
    child->parent = parent;
    sector->parent = child;
    sector->right = second_child;
    if (second_child != nullptr)second_child->parent = sector;
    if (parent != nullptr){
        if (sector == parent->right) parent->right = child;
        else parent->left = child;
    }
    else root = child;

    return child;
}

Sector* SpaceSectorLLRBT::rotate_right(Sector* sector) {

    Sector* parent = sector->parent;
    Sector* child = sector->left;
    Sector* second_child = child->right;

    child->right = sector;
    child->parent = parent;
    sector->parent = child;
    sector->left = second_child;
    if (second_child != nullptr) second_child->parent = sector;
    if (parent != nullptr){
        if (sector == parent->right) parent->right = child;
        else parent->left = child;
    }
    else root = child;

    return child;
}

void swap_color(Sector* one, Sector* two){

    bool temp = one->color;
    one->color = two->color;
    two->color = temp;
}

void SpaceSectorLLRBT::insert_sector(Sector* parent, Sector* new_sector) {

    //standard BST insert.
    if (
            (parent->x > new_sector->x)
            ||
            (parent->x == new_sector->x && parent->y > new_sector->y)
            ||
            (parent->x == new_sector->x && parent->y == new_sector->y && parent->z > new_sector->z)
            ) {

        if (parent->left == nullptr) {

            new_sector->parent = parent;
            parent->left = new_sector;
        } else insert_sector(parent->left, new_sector);
    } else {

        if (parent->right == nullptr) {

            new_sector->parent = parent;
            parent->right = new_sector;
        } else insert_sector(parent->right, new_sector);
    }

    // case 1.
    // when right child is Red but left child is Black or doesn't exist.
    if (parent->right != nullptr && parent->right->color &&
        (parent->left == nullptr or (parent->left != nullptr && !parent->left->color))) {

        swap_color(parent, parent->right);
        parent = rotate_left(parent);
    }

    // case 2
    // when left child as well as left grand child is Red
    if (parent->left != nullptr && parent->left->color && parent->left->left != nullptr && parent->left->left->color) {

        swap_color(parent, parent->left);
        parent = rotate_right(parent);
    }

    // case 3
    // when both left and right child are colored Red.
    if (parent->left != nullptr && parent->right != nullptr && parent->left->color && parent->right->color) {

        parent->color = !parent->color;

        parent->left->color = false;
        parent->right->color = false;
    }
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    if (root == nullptr) root = new Sector(x, y, z);
    else insert_sector(root, new Sector(x, y, z));

    root->color = false; // root must always be black
}

void print_in_order_LLRBT(Sector* sector) {

    if (sector->left) print_in_order_LLRBT(sector->left);
    if (sector->color) cout << "RED";
    else cout << "BLACK";
    cout << " sector: " << sector->sector_code << endl;
    if (sector->right) print_in_order_LLRBT(sector->right);
}

void SpaceSectorLLRBT::displaySectorsInOrder() {

    std::cout << "Space sectors inorder traversal:\n";
    print_in_order_LLRBT(root);
    std::cout << std::endl;
}

void print_pre_order_LLRBT(Sector* sector) {

    if (sector->color) cout << "RED";
    else cout << "BLACK";
    cout << " sector: " << sector->sector_code << endl;
    if (sector->left) print_pre_order_LLRBT(sector->left);
    if (sector->right) print_pre_order_LLRBT(sector->right);
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {

    std::cout << "Space sectors preorder traversal:\n";
    print_pre_order_LLRBT(root);
    std::cout << std::endl;
}

void print_post_order_LLRBT(Sector* sector) {

    if (sector->left) print_post_order_LLRBT(sector->left);
    if (sector->right) print_post_order_LLRBT(sector->right);
    if (sector->color) cout << "RED";
    else cout << "BLACK";
    cout << " sector: " << sector->sector_code << endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {

    std::cout << "Space sectors postorder traversal:\n";
    print_post_order_LLRBT(root);
    std::cout << std::endl;
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {

    std::vector<Sector*> path;

    if (root != nullptr) {

        Sector* target = root->find_by_sector_code(sector_code);
        Sector* earth = root->find_by_sector_code("0SSS");
        Sector* last_duplicate_sector = nullptr;

        std::vector<Sector*> target_root_line, earth_root_line;


        if (target != nullptr && earth != nullptr) {
            target->get_path(target_root_line);
            earth->get_path(earth_root_line);
        }

        //deletes every duplicate in the path except the last one.
        while (!target_root_line.empty() && !earth_root_line.empty() &&
                target_root_line[target_root_line.size() - 1] == earth_root_line[earth_root_line.size() - 1]
        ) {

            last_duplicate_sector = target_root_line[target_root_line.size() - 1];
            target_root_line.pop_back();
            earth_root_line.pop_back();
        }

        earth_root_line.push_back(last_duplicate_sector);
        std::reverse(target_root_line.begin(), target_root_line.end());


        path.reserve( target_root_line.size() + earth_root_line.size() );
        path.insert( path.end(), earth_root_line.begin(), earth_root_line.end() );
        path.insert( path.end(), target_root_line.begin(), target_root_line.end() );
    }


    //I don't know why but if the path is empty it adds one nullptr to itself so this is my fix for it :)
    if(path[0] == nullptr) path.pop_back();

    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {

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
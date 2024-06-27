#include "Sector.h"

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {

    distance_from_earth = pow((pow(x,2) + pow(y,2) + pow(z,2)), 0.5);

    sector_code += std::to_string(static_cast<int>(distance_from_earth));

    if (x < 0) sector_code += "L";
    else if (x==0) sector_code += "S";
    else sector_code += "R";

    if (y < 0) sector_code += "D";
    else if (y==0) sector_code += "S";
    else sector_code += "U";

    if (z < 0) sector_code += "B";
    else if (z==0) sector_code += "S";
    else sector_code += "F";
}

Sector::~Sector() {

    delete left;
    delete right;

    //makes sure that parent doesn't point to a freed memory
    if (parent != nullptr){
        if (parent->right == this) parent->right = nullptr;
        else parent->left = nullptr;
    }
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}

Sector* Sector::find_by_sector_code(const std::string &code) {

    if(code == this->sector_code) return this;

    Sector* node = nullptr;
    if (left != nullptr) node = left->find_by_sector_code(code);
    if (right != nullptr && node == nullptr) node = right->find_by_sector_code(code);
    return node;
}

Sector* Sector::get_minimum() {

    if(left != nullptr) return left->get_minimum();
    return this;
}

Sector* Sector::get_root() {

    if(parent != nullptr) return parent->get_root();
    else return this;
}

void Sector::get_path(std::vector<Sector *> &path) {

    path.push_back(this);
    if(parent != nullptr) parent->get_path(path);
}
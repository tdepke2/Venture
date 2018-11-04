#include "TileBreakable.h"

using namespace std;

TileBreakable::TileBreakable() {
    init();
}

TileBreakable::TileBreakable(const TileBreakable& tileBreakable) : Tile(tileBreakable) {
    health = tileBreakable.health;
}

void TileBreakable::init(int health) {
    this->health = health;
}

void TileBreakable::init(const vector<string>& data, int& index) {
    Tile::init(data, index);
    init(stoi(data[index]));
    index += 1;
}

string TileBreakable::toString() const {
    return Tile::toString() + "," + to_string(health);
}

TileBreakable* TileBreakable::makeCopy() const {
    return new TileBreakable(*this);
}
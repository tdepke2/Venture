#include "Entity.h"
#include "Tile.h"

using namespace std;

Tile::Tile() {
    entity = nullptr;
    init();
    lightLevel = 0;
}

Tile::~Tile() {}

Tile::Tile(const Tile& tile) : Object(tile) {
    entity = nullptr;
    solid = tile.solid;
    transparent = tile.transparent;
    lightColor = tile.lightColor;
    lightLevel = 0;
}

void Tile::init(bool solid, bool transparent, short lightColor) {
    this->solid = solid;
    this->transparent = transparent;
    this->lightColor = lightColor;
}

void Tile::init(const vector<string>& data, int& index) {
    Object::init(data, index);
    init(data[index] == "true" ? true : false, data[index + 1] == "true" ? true : false, Object::stringToColor(data[index + 2]));
    index += 3;
}

bool Tile::isDefault() const {
    Tile tile;
    return Object::isDefault() && solid == tile.solid && transparent == tile.transparent && lightColor == tile.lightColor && lightLevel == tile.lightLevel;
}

void Tile::draw(Window& win, int x, int y) {
    lightLevel = 3;    // ##################################################################
    
    if (lightLevel != 0) {
        if (entity != nullptr) {
            entity->setBackground(lightColor + lightLevel - 1);
            entity->draw(win, x, y);
        } else {
            setBackground(lightColor + lightLevel - 1);
            Object::draw(win, x, y);
        }
    }
}

string Tile::toString() const {
    return Object::toString() + "," + (solid ? "true" : "false") + "," + (transparent ? "true" : "false") + "," + Object::colorToString(lightColor);
}

Tile* Tile::makeCopy() const {
    return new Tile(*this);
}
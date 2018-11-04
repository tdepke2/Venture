#ifndef _TILE_H
#define _TILE_H

class Entity;
class Window;

#include "Object.h"
#include <string>
#include <vector>

using namespace std;

class Tile : public Object {
    public:
    Entity* entity;
    bool solid, transparent;
    short lightColor, lightLevel;
    
    Tile();
    virtual ~Tile();
    Tile(const Tile& tile);
    void init(bool solid = false, bool transparent = true, short lightColor = COLOR_YELLOW1);
    void init(const vector<string>& data, int& index);
    bool isDefault() const;
    void draw(Window& win, int x, int y);
    virtual string toString() const;
    virtual Tile* makeCopy() const;
};

#endif
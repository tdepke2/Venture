#ifndef _TILEBREAKABLE_H
#define _TILEBREAKABLE_H

#include "Tile.h"
#include <string>
#include <vector>

using namespace std;

class TileBreakable : public Tile {
    public:
    int health;
    
    TileBreakable();
    TileBreakable(const TileBreakable& tileBreakable);
    void init(int health = 0);
    void init(const vector<string>& data, int& index);
    string toString() const;
    TileBreakable* makeCopy() const;
};

#endif
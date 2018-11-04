#ifndef _ENTITYPASSIVE_H
#define _ENTITYPASSIVE_H

class Window;

#include "CombatStats.h"
#include "Entity.h"
#include "PairXY.h"
#include <string>
#include <vector>

using namespace std;

class EntityPassive : public Entity, public CombatStats {
    public:
    int timer;
    
    EntityPassive();
    EntityPassive(const EntityPassive& entityPassive);
    void init(int timer = 0);
    void init(const vector<string>& data, int& index);
    string toString() const;
    EntityPassive* makeCopy() const;
    void update(Window& win, const PairXYi& playerPos);
};

#endif
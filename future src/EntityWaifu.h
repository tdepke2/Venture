#ifndef _ENTITY_WAIFU_H
#define _ENTITY_WAIFU_H

using namespace std;

#include "EntityPassive.h"
#include "PairXY.h"
#include <string>
#include <vector>

class EntityWaifu : public EntityPassive {
    public:
    EntityWaifu();
    virtual ~EntityWaifu();
    bool init(Window& win, const PairXYi& position, int lightRadius = 0);
    bool init(Window& win, const PairXYi& position, const vector<string>& data, int index);
    bool move(Window& win, int dx, int dy);
    virtual void update(Window& win, const PairXYi& playerPos);
    virtual string toString() const;
}

#endif
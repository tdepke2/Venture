#ifndef _ENTITYPLAYER_H
#define _ENTITYPLAYER_H

class Window;

#include "Entity.h"
#include "PairXY.h"

using namespace std;

class EntityPlayer : public Entity {
    public:
    void load(const string& playerDataFilename);
    void update(Window& win, const PairXYi& playerPos);
};

#endif
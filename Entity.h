#ifndef _ENTITY_H
#define _ENTITY_H

class Window;

#include "Object.h"
#include "PairXY.h"
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Entity : public Object {
    public:
    int lightRadius;
    
    Entity();
    virtual ~Entity();
    Entity(const Entity& entity);
    bool setPosition(Window& win, const PairXYi& position);
    const PairXYi& getPosition() const;
    void init(int lightRadius = 0);
    void init(const vector<string>& data, int& index);
    bool move(Window& win, int dx, int dy);
    void resetPosition(Window& win);
    virtual void update(Window& win, const PairXYi& playerPos);
    virtual string toString() const;
    virtual Entity* makeCopy() const;
    
    protected:
    void _updateLight(Window& win, const PairXYi& playerPos);
    
    private:
    static unordered_map<int, vector<PairXYi>> _radiusLookup;
    PairXYi _position;
    
    void _traceCircle(Window& win, short val, int radius);
    void _traceLine(Window& win, short val, int x0, int y0, int x1, int y1);
};

#endif
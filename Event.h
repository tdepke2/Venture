#ifndef _EVENT_H
#define _EVENT_H

class Condition;
class Entity;
class Tile;

#include "PairXY.h"
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Event {
    public:
    enum Type {
        None = 0, SetTile, FillTile, CloneTile, SetEntity, FillEntity, CloneEntity, typeSize
    };
    
    static const unordered_map<string, Type> STRING_TO_TYPE;
    Condition* condition;
    Event* falseEvent;
    Event* trueEvent;
    
    static Type stringToType(const string& s);
    static const string& typeToString(Type t);
    Event();
    void init(const vector<string>& data, int index);
    string toString() const;
    
    private:
    PairXYi _position1, _position2;
    Tile* _tile;
    Entity* _entity;
};

#endif
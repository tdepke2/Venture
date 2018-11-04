#ifndef _ROOM_H
#define _ROOM_H

#include "Level.h"
#include <string>
#include <vector>

using namespace std;

class Room : public Level {
    public:
    struct Connection {
        int x;
        int y;
        int maxOffset;
        char direction;
    };
    
    vector<Connection> connections;
    int minConnections, maxConnections;
    bool canRotate;
    float weight;
    
    Room();
    Room(const string& loadFile);
    ~Room();
    void load(const string& loadFile);
};

#endif
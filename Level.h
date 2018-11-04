#ifndef _LEVEL_H
#define _LEVEL_H

class Room;
class Tile;
class Window;

#include "Entity.h"
#include "EntityPlayer.h"
#include "PairXY.h"
#include <list>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class Level {
    public:
    Window* win;    // Copy of the window in Game class, not deleted by destructor.
    
    Level();
    ~Level();
    void save(const string& saveFilename);
    void load(const string& loadFilename);
    void generateLevel(const string& structureDir, int levelID);
    void drawLevel();
    void updateAll();
    
    protected:
    Tile*** _tileArray;
    PairXYi _tileArraySize;
    string _levelName;
    EntityPlayer _player;
    list<Entity*> _entities;
    
    void _reallocate(const PairXYi& size);
    vector<vector<string>> _loadFile(const string& loadFilename);    // Loads level data from a file into this level. Returns all custom data with line number found in the file.
    void _mergeRoom(const Room* room, PairXYi position, int rotation);
    
    private:
    void _addRoom(const Room* room, vector<pair<PairXYi, PairXYi>>& roomBounds, vector<pair<const Room*, int>>& roomData);
    static vector<string> _parseCSV(const string& str);    // Parse a line of a CSV file. Splits the line and returns as a vector of strings, there will always be at least one string in the vector. Comments must be on their own line and start with a "#".
    static float _scoreLastRoom(const vector<pair<PairXYi, PairXYi>>& roomBounds, const vector<pair<const Room*, int>>& roomData);
};

#endif
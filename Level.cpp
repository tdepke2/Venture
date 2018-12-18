#include "EntityPassive.h"
#include "Game.h"
#include "Level.h"
#include "Room.h"
#include "Tile.h"
#include "TileBreakable.h"
#include "Window.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#undef max
#include <limits>
#include <stdexcept>

using namespace std;

Level::Level() {
    win = nullptr;
    _tileArray = nullptr;
}

Level::~Level() {
    for (int y = 0; y < _tileArraySize.y; ++y) {
        for (int x = 0; x < _tileArraySize.x; ++x) {
            delete _tileArray[y][x];
        }
        delete[] _tileArray[y];
    }
    delete[] _tileArray;
    
    for (auto listIter = _entities.begin(); listIter != _entities.end(); ++listIter) {
        delete *listIter;
    }
}

void Level::save(const string& saveFilename) {
    ofstream saveFile(saveFilename, ios::trunc);
    if (!saveFile.is_open()) {
        throw runtime_error(saveFilename + ": Unable to save level data.");
    }
    
    saveFile << "version,1.0\nname," << _levelName << "\nsize," << to_string(_tileArraySize.x) << "," << to_string(_tileArraySize.y) << "\n\ndata:\n";
    saveFile << "playerPos," << to_string(_player.getPosition().x) << "," << to_string(_player.getPosition().y) << "\nend\n\nconditions:\n";
    
    // Save conditions (with names).
    
    saveFile << "end\n\nevents:\n";
    
    // Save events (with names).
    
    saveFile << "end\n\ntiles:\n";
    for (int y = 0; y < _tileArraySize.y; ++y) {
        for (int x = 0; x < _tileArraySize.x; ++x) {
            _tileArray[y][x]->lightLevel = 0;
            _tileArray[y][x]->setBackground(COLOR_BLACK);
            if (typeid(*_tileArray[y][x]) == typeid(Tile)) {
                if (!_tileArray[y][x]->isDefault()) {
                    saveFile << "Tile," << x << "," << y << "," << _tileArray[y][x]->toString() << "\n";
                }
            } else if (typeid(*_tileArray[y][x]) == typeid(TileBreakable)) {
                saveFile << "TileBreakable," << x << "," << y << "," << _tileArray[y][x]->toString() << "\n";
            }
        }
    }
    saveFile << "end\n\nentities:\n";
    for (auto listIter = _entities.begin(); listIter != _entities.end(); ++listIter) {
        (*listIter)->setBackground(COLOR_BLACK);
        if (typeid(**listIter) == typeid(Entity)) {
            saveFile << "Entity," << (*listIter)->getPosition().x << "," << (*listIter)->getPosition().y << "," << (*listIter)->toString() << "\n";
        } else if (typeid(**listIter) == typeid(EntityPassive)) {
            saveFile << "EntityPassive," << (*listIter)->getPosition().x << "," << (*listIter)->getPosition().y << "," << (*listIter)->toString() << "\n";
        }
    }
    saveFile << "end";
    saveFile.close();
}

void Level::load(const string& loadFilename) {
    vector<vector<string>> customData = _loadFile(loadFilename);
    string lineNumberStr;
    int numEntries = 0;
    try {
        for (const vector<string>& data : customData) {
            lineNumberStr = data[0];
            if (numEntries == 0 && data.size() == 4 && data[1] == "playerPos") {
                PairXYi playerPos(stoi(data[2]), stoi(data[3]));
                if (_tileArray[playerPos.y][playerPos.x] == nullptr) {
                    _tileArray[playerPos.y][playerPos.x] = new Tile;
                }
                if (!_player.setPosition(*win, playerPos)) {
                    throw runtime_error("Failed to initialize the player.");
                }
                _player.init(10);
                _player.setChar1('p');
                _player.setChar2('p');
                _player.setForeground(COLOR_BLACK);
                _player.setBackground(COLOR_BLACK);
                ++numEntries;
            } else {
                throw runtime_error("Unexpected data type.");
            }
        }
    } catch (exception& ex) {
        throw runtime_error(loadFilename + " at line " + lineNumberStr + ": " + ex.what());
    }
    if (numEntries != 1) {
        throw runtime_error(loadFilename + ": Missing some custom data.");
    }
    
    for (int y = 0; y < _tileArraySize.y; ++y) {    // Fill in any holes where tiles were not initialized.
        for (int x = 0; x < _tileArraySize.x; ++x) {
            if (_tileArray[y][x] == nullptr) {
                _tileArray[y][x] = new Tile;
            }
        }
    }
}

void Level::generateLevel(const string& structureDir, int levelID) {
    vector<Room*> rooms;    // The first two elements are the starting room and boss room.
    //vector<Path*> paths;
    PairXYi minSize, maxSize;
    int minRooms, maxRooms;
    if (levelID == 0) {
        rooms.resize(5);
        rooms[0] = new Room(structureDir + "room0.csv");
        rooms[1] = new Room(structureDir + "room1.csv");
        rooms[2] = new Room(structureDir + "room2.csv");
        rooms[3] = new Room(structureDir + "room3.csv");
        rooms[4] = new Room(structureDir + "room4.csv");
        minSize = PairXYi(40, 40);
        maxSize = PairXYi(60, 60);
        minRooms = 6;
        maxRooms = 6;
    } else {
        assert(false);
    }
    float maxWeight = 0.0;
    for (unsigned int i = 2; i < rooms.size(); ++i) {
        maxWeight += rooms[i]->weight;
    }
    for (Room* room : rooms) {
        if ((room->canRotate && max(room->_tileArraySize.x, room->_tileArraySize.y) > min(minSize.x, minSize.y)) || (!room->canRotate && (room->_tileArraySize.x > minSize.x || room->_tileArraySize.y > minSize.y))) {
            throw runtime_error("Room \"" + room->_levelName + "\" is too large for level.");
        }
        room->weight /= maxWeight;
    }
    
    _reallocate(PairXYi(Game::randomInteger(minSize.x, maxSize.x), Game::randomInteger(minSize.y, maxSize.y)));
    int remainingRooms = Game::randomInteger(minRooms, maxRooms);
    vector<pair<PairXYi, PairXYi>> roomBounds;
    vector<pair<const Room*, int>> roomData;
    roomBounds.reserve(remainingRooms);
    roomData.reserve(remainingRooms);
    /*try {
        while (true) {
            _addRoom(rooms[Game::randomInteger(rooms.size())], roomBounds, roomData);
        }
    } catch (exception& ex) {
        string desc = ex.what();
        if (desc != "Unable to add enough rooms.") {
            throw runtime_error("Uh oh we in trouble: " + desc);
        }
    }*/
    /*while (remainingRooms > 0) {
        _addRoom(rooms[Game::randomInteger(rooms.size())], roomBounds, roomData);
        --remainingRooms;
    }*/
    _addRoom(rooms[0], roomBounds, roomData);
    _addRoom(rooms[1], roomBounds, roomData);
    _addRoom(rooms[2], roomBounds, roomData);
    _addRoom(rooms[2], roomBounds, roomData);
    _addRoom(rooms[2], roomBounds, roomData);
    _addRoom(rooms[2], roomBounds, roomData);
    _addRoom(rooms[3], roomBounds, roomData);
    _addRoom(rooms[3], roomBounds, roomData);
    _addRoom(rooms[3], roomBounds, roomData);
    _addRoom(rooms[3], roomBounds, roomData);
    _addRoom(rooms[4], roomBounds, roomData);
    _addRoom(rooms[1], roomBounds, roomData);
    Game::logFile.close();
    
    for (int y = 0; y < _tileArraySize.y; ++y) {    // Fill in any holes where tiles were not initialized.
        for (int x = 0; x < _tileArraySize.x; ++x) {
            if (_tileArray[y][x] == nullptr) {
                _tileArray[y][x] = new Tile;
            }
        }
    }
    
    assert(_player.getPosition() == PairXYi(-1, -1));
    for (int y = 0; y < _tileArraySize.y; ++y) {
        for (int x = 0; x < _tileArraySize.x; ++x) {
            if (!_tileArray[y][x]->solid) {
                assert(_player.setPosition(*win, PairXYi(x, y)));
                x = _tileArraySize.x;
                y = _tileArraySize.y;
            }
        }
    }
    _player.init(10);
    _player.setChar1('p');
    _player.setChar2('p');
    _player.setForeground(COLOR_BLACK);
    _player.setBackground(COLOR_BLACK);
    
    for (Room* room : rooms) {
        delete room;
    }
}

void Level::drawLevel() {
    PairXYi halfWinSize((win->getSize().x - 2) / 4, (win->getSize().y - 2) / 2);
    PairXYi topLeft(max(0, _player.getPosition().x - halfWinSize.x), max(0, _player.getPosition().y - halfWinSize.y));
    PairXYi bottomRight(min(_tileArraySize.x - 1, _player.getPosition().x + halfWinSize.x), min(_tileArraySize.y - 1, _player.getPosition().y + halfWinSize.y));
    PairXYi offset(halfWinSize.x - _player.getPosition().x, halfWinSize.y - _player.getPosition().y);
    for (int y = topLeft.y; y <= bottomRight.y; ++y) {
        for (int x = topLeft.x; x <= bottomRight.x; ++x) {
            _tileArray[y][x]->draw(*win, (x + offset.x) * 2 + 1, y + offset.y + 1);
        }
    }
}

void Level::updateAll() {
    _player.update(*win, _player.getPosition());
    for (auto listIter = _entities.begin(); listIter != _entities.end(); ++listIter) {
        (*listIter)->update(*win, _player.getPosition());
    }
}

void Level::_reallocate(const PairXYi& size) {
    _player.resetPosition(*win);
    for (int y = 0; y < _tileArraySize.y; ++y) {    // Delete old level data.
        for (int x = 0; x < _tileArraySize.x; ++x) {
            delete _tileArray[y][x];
        }
        delete[] _tileArray[y];
    }
    delete[] _tileArray;
    for (auto listIter = _entities.begin(); listIter != _entities.end(); ++listIter) {
        delete *listIter;
    }
    _entities.clear();
    
    _tileArraySize = size;
    _tileArray = new Tile**[_tileArraySize.y];    // Allocate memory for tileArray but leave tiles as null.
    for (int y = 0; y < _tileArraySize.y; ++y) {
        _tileArray[y] = new Tile*[_tileArraySize.x];
        for (int x = 0; x < _tileArraySize.x; ++x) {
            _tileArray[y][x] = nullptr;
        }
    }
    win->tileArray = _tileArray;
    win->tileArraySize = _tileArraySize;
}

vector<vector<string>> Level::_loadFile(const string& loadFilename) {    // Loads level data from a file into this level. Returns all custom data with line number found in the file.
    ifstream loadFile(loadFilename);
    if (!loadFile.is_open()) {
        throw runtime_error(loadFilename + ": Unable to access level data.");
    }
    
    float levelVersion;
    string line;
    int lineNumber = 0, numEntries = 0;
    vector<vector<string>> customData;
    try {
        while (getline(loadFile, line)) {
            ++lineNumber;
            vector<string> data = _parseCSV(line);
            
            if (data.size() == 1 && data[0].length() == 0) {
                continue;
            } else if (numEntries == 0 && data.size() == 2 && data[0] == "version") {
                levelVersion = stof(data[1]);
                if (levelVersion != 1.0f) {
                    throw runtime_error("Invalid map version.");
                }
                ++numEntries;
            } else if (numEntries == 1 && data.size() == 2 && data[0] == "name") {
                _levelName = data[1];
                ++numEntries;
            } else if (numEntries == 2 && data.size() == 3 && data[0] == "size") {
                _reallocate(PairXYi(stoi(data[1]), stoi(data[2])));
                ++numEntries;
            } else if (numEntries == 3 && data.size() == 1 && data[0] == "data:") {
                ++numEntries;
            } else if (numEntries == 4) {    // Parse data for all custom data.
                if (data.size() == 1 && data[0] == "end") {
                    ++numEntries;
                } else {
                    data.insert(data.begin(), to_string(lineNumber));
                    customData.push_back(data);
                }
            } else if (numEntries == 5 && data.size() == 1 && data[0] == "conditions:") {
                ++numEntries;
            } else if (numEntries == 6) {    // Parse data for each type of condition.
                if (data.size() == 1 && data[0] == "end") {
                    ++numEntries;
                } else {
                    throw runtime_error("Unrecognized condition type.");
                }
            } else if (numEntries == 7 && data.size() == 1 && data[0] == "events:") {
                ++numEntries;
            } else if (numEntries == 8) {    // Parse data for each type of event.
                if (data.size() == 1 && data[0] == "end") {
                    ++numEntries;
                } else {
                    throw runtime_error("Unrecognized event type.");
                }
            } else if (numEntries == 9 && data.size() == 1 && data[0] == "tiles:") {
                ++numEntries;
            } else if (numEntries == 10) {    // Parse data for each type of tile.
                int x, y;
                if (data.size() >= 3) {
                    x = stoi(data[1]);
                    y = stoi(data[2]);
                    if (x < 0 || x >= _tileArraySize.x || y < 0 || y >= _tileArraySize.y) {
                        throw runtime_error("Tile out of bounds.");
                    }
                }
                
                if (data.size() == 12 && data[0] == "Tile") {
                    Tile* tile = new Tile;
                    int index = 3;
                    tile->init(data, index);
                    _tileArray[y][x] = tile;
                } else if (data.size() == 13 && data[0] == "TileBreakable") {
                    TileBreakable* tileBreakable = new TileBreakable;
                    int index = 3;
                    tileBreakable->init(data, index);
                    _tileArray[y][x] = tileBreakable;
                } else if (data.size() == 1 && data[0] == "end") {
                    ++numEntries;
                } else {
                    throw runtime_error("Unrecognized tile type.");
                }
            } else if (numEntries == 11 && data.size() == 1 && data[0] == "entities:") {
                ++numEntries;
            } else if (numEntries == 12) {    // Parse data for each type of entity.
                PairXYi position;
                if (data.size() >= 3) {
                    position.x = stoi(data[1]);
                    position.y = stoi(data[2]);
                    if (_tileArray[position.y][position.x] == nullptr) {
                        _tileArray[position.y][position.x] = new Tile;
                    }
                }
                
                if (data.size() == 10 && data[0] == "Entity") {
                    Entity* entity = new Entity;
                    if (!entity->setPosition(*win, position)) {
                        throw runtime_error("Failed to initialize entity.");
                    }
                    int index = 3;
                    entity->init(data, index);
                    _entities.push_back(entity);
                } else if (data.size() == 16 && data[0] == "EntityPassive") {
                    EntityPassive* entityPassive = new EntityPassive;
                    if (!entityPassive->setPosition(*win, position)) {
                        throw runtime_error("Failed to initialize entity.");
                    }
                    int index = 3;
                    entityPassive->init(data, index);
                    _entities.push_back(entityPassive);
                } else if (data.size() == 1 && data[0] == "end") {
                    ++numEntries;
                } else {
                    throw runtime_error("Unrecognized entity type.");
                }
            } else {
                throw runtime_error("Invalid level data.");
            }
        }
    } catch (exception& ex) {
        throw runtime_error(loadFilename + " at line " + to_string(lineNumber) + ": " + ex.what());
    }
    
    if (numEntries != 13) {
        throw runtime_error(loadFilename + ": Missing level data, end of file reached.");
    }
    loadFile.close();
    return customData;
}

void Level::_mergeRoom(const Room* room, PairXYi position, int rotation) {
    PairXYi newPos = position, xScale, yScale;
    if (rotation == 0) {
        xScale = PairXYi(1, 0);
        yScale = PairXYi(0, 1);
    } else if (rotation == 1) {
        newPos.x += room->_tileArraySize.y - 1;
        xScale = PairXYi(0, -1);
        yScale = PairXYi(1, 0);
    } else if (rotation == 2) {
        newPos += room->_tileArraySize - PairXYi(1, 1);
        xScale = PairXYi(-1, 0);
        yScale = PairXYi(0, -1);
    } else {
        newPos.y += room->_tileArraySize.x - 1;
        xScale = PairXYi(0, 1);
        yScale = PairXYi(-1, 0);
    }
    for (int y = 0; y < room->_tileArraySize.y; ++y) {
        for (int x = 0; x < room->_tileArraySize.x; ++x) {
            int x2 = newPos.x + xScale.x * x + xScale.y * y;
            int y2 = newPos.y + yScale.x * x + yScale.y * y;
            assert(x2 >= 0 && x2 < _tileArraySize.x && y2 >= 0 && y2 < _tileArraySize.y && _tileArray[y2][x2] == nullptr);
            if (room->_tileArray[y][x] != nullptr) {
                _tileArray[newPos.y + yScale.x * x + yScale.y * y][newPos.x + xScale.x * x + xScale.y * y] = room->_tileArray[y][x]->makeCopy();
            }
        }
    }
    for (Entity* entity : room->_entities) {
        Entity* entityCopy = entity->makeCopy();
        if (!entityCopy->setPosition(*win, PairXYi(newPos.x + xScale.x * entity->getPosition().x + xScale.y * entity->getPosition().y, newPos.y + yScale.x * entity->getPosition().x + yScale.y * entity->getPosition().y))) {
            throw runtime_error("Failed to initialize entity in room \"" + room->_levelName + "\".");
        }
        _entities.push_back(entityCopy);
    }
}

void Level::_addRoom(const Room* room, vector<pair<PairXYi, PairXYi>>& roomBounds, vector<pair<const Room*, int>>& roomData) {
    roomBounds.push_back(make_pair(PairXYi(0, 0), PairXYi(0, 0)));
    roomData.push_back(make_pair(room, 0));
    int numFails;
    Game::logFile << "Room name: " << roomData.back().first->_levelName << ", weight = " << roomData.back().first->weight << endl;
    for (numFails = 0; numFails < 5000; ++numFails) {
        int rotation = room->canRotate ? Game::randomInteger(4) : 0;
        if (rotation == 0 || rotation == 2) {
            PairXYi position(Game::randomInteger(0, _tileArraySize.x - room->_tileArraySize.x), Game::randomInteger(0, _tileArraySize.y - room->_tileArraySize.y));
            roomBounds.back() = make_pair(position, position + room->_tileArraySize - PairXYi(1, 1));
        } else {
            PairXYi position(Game::randomInteger(0, _tileArraySize.x - room->_tileArraySize.y), Game::randomInteger(0, _tileArraySize.y - room->_tileArraySize.x));
            roomBounds.back() = make_pair(position, position + PairXYi(room->_tileArraySize.y - 1, room->_tileArraySize.x - 1));
        }
        roomData.back().second = rotation;
        
        bool collision = false;
        for (unsigned int i = 0; i < roomBounds.size() - 1; ++i) {
            if (roomBounds.back().first.x <= roomBounds[i].second.x && roomBounds.back().second.x >= roomBounds[i].first.x && roomBounds.back().first.y <= roomBounds[i].second.y && roomBounds.back().second.y >= roomBounds[i].first.y) {
                collision = true;
            }
        }
        if (!collision) {
            Game::logFile << "  Final position at (" << roomBounds.back().first.x << ", " << roomBounds.back().first.y << ") with rotation " << roomData.back().second * 90 << " degress CW" << endl;
            Game::logFile << "  (numFails reached " << numFails << ")" << endl << endl;
            _mergeRoom(room, roomBounds.back().first, roomData.back().second);
            return;
        }
    }
    
    throw runtime_error("Unable to add enough rooms.");
}

vector<string> Level::_parseCSV(const string& str) {    // Parse a line of a CSV file. Splits the line and returns as a vector of strings, there will always be at least one string in the vector. Comments must be on their own line and start with a "#".
    vector<string> values;
    if (str.length() > 0 && str[0] == '#') {
        values.push_back("");
        return values;
    }
    
    size_t index = 0;
    while (index < str.length() && str[index] != ',') {
        ++index;
    }
    
    vector<size_t> points;
    points.push_back(index);
    while (index < str.length()) {
        ++index;
        while (index < str.length() && str[index] != ',') {
            ++index;
        }
        points.push_back(index);
    }
    
    values.reserve(points.size() - 1);
    values.push_back(str.substr(0, points[0]));
    for (unsigned int i = 1; i < points.size(); ++i) {
        values.push_back(str.substr(points[i - 1] + 1, points[i] - points[i - 1] - 1));
    }
    return values;
}
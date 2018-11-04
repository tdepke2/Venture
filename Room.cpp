#include "PairXY.h"
#include "Room.h"
#include "Window.h"

using namespace std;

Room::Room() {
    win = new Window();
}

Room::Room(const string& loadFile) {
    win = new Window();
    load(loadFile);
}

Room::~Room() {
    delete win;
}

void Room::load(const string& loadFile) {
    vector<vector<string>> customData = _loadFile(loadFile);
    string lineNumberStr;
    int numEntries = 0, numConnections = 0;
    try {
        for (const vector<string>& data : customData) {
            lineNumberStr = data[0];
            if (numEntries == 0 && data[1] == "connectionX") {
                numConnections = data.size() - 2;
                connections.resize(numConnections);
                for (unsigned int i = 2; i < data.size(); ++i) {
                    connections[i - 2].x = stoi(data[i]);
                }
                ++numEntries;
            } else if (numEntries == 1 && data.size() - 2 == numConnections && data[1] == "connectionY") {
                for (unsigned int i = 2; i < data.size(); ++i) {
                    connections[i - 2].y = stoi(data[i]);
                }
                ++numEntries;
            } else if (numEntries == 2 && data.size() - 2 == numConnections && data[1] == "connectionOffset") {
                for (unsigned int i = 2; i < data.size(); ++i) {
                    connections[i - 2].maxOffset = stoi(data[i]);
                }
                ++numEntries;
            } else if (numEntries == 3 && data.size() - 2 == numConnections && data[1] == "connectionDir") {
                for (unsigned int i = 2; i < data.size(); ++i) {
                    if (data[i] != "N" && data[i] != "E" && data[i] != "S" && data[i] != "W") {
                        throw runtime_error("Invalid direction (" + data[i] + ").");
                    }
                    connections[i - 2].direction = data[i][0];
                }
                ++numEntries;
            } else if (numEntries == 4 && data.size() == 4 && data[1] == "minMaxConnections") {
                minConnections = stoi(data[2]);
                maxConnections = stoi(data[3]);
                ++numEntries;
            } else if (numEntries == 5 && data.size() == 3 && data[1] == "weight") {
                weight = stof(data[2]);
                ++numEntries;
            } else if (numEntries == 6 && data.size() == 3 && data[1] == "canRotate") {
                canRotate = (data[2] == "true" ? true : false);
                ++numEntries;
            } else {
                throw runtime_error("Unexpected data type.");
            }
        }
    } catch (exception& ex) {
        throw runtime_error(loadFile + " at line " + lineNumberStr + ": " + ex.what());
    }
    if (numEntries != 7) {
        throw runtime_error(loadFile + ": Missing some custom data.");
    }
    
    /*ofstream levelFile("temp/roomData.csv", ios::trunc);
    if (!levelFile.is_open()) {
        throw runtime_error(loadFile + ": Unable to save room data.");
    }
    levelFile << "connectionX";
    for (const Connection& c : connections) {
        levelFile << "," << c.x;
    }
    levelFile << "\nconnectionY";
    for (const Connection& c : connections) {
        levelFile << "," << c.y;
    }
    levelFile << "\nconnectionOffset";
    for (const Connection& c : connections) {
        levelFile << "," << c.maxOffset;
    }
    levelFile << "\nconnectionDir";
    for (const Connection& c : connections) {
        levelFile << "," << c.direction;
    }
    levelFile << "\nminMaxConnections," << minConnections << "," << maxConnections << "\n";
    levelFile << "canRotate," << (canRotate ? "true" : "false");
    levelFile.close();*/
}
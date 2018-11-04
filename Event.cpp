#include "Event.h"

using namespace std;

const unordered_map<string, Event::Type> Event::STRING_TO_TYPE = {
    {"None", None}, {"SetTile", SetTile}, {"FillTile", FillTile}, {"CloneTile", CloneTile}, {"SetEntity", SetEntity}, {"FillEntity", FillEntity}, {"CloneEntity", CloneEntity}
};

Event::Type Event::stringToType(const string& s) {
    Type type = typeSize;
    auto mapIter = STRING_TO_TYPE.find(s);
    if (mapIter != STRING_TO_TYPE.end()) {
        type = mapIter->second;
    }
    if (type == typeSize) {
        throw runtime_error("Invalid type (" + s + ").");
    }
    return type;
}

const string& Event::typeToString(Event::Type t) {
    for (auto mapIter = STRING_TO_TYPE.begin(); mapIter != STRING_TO_TYPE.end(); ++mapIter) {
        if (mapIter->second == t) {
            return mapIter->first;
        }
    }
    throw runtime_error("Invalid type (" + to_string(t) + ").");
}

Event::Event() {
    
}

void Event::init(const vector<string>& data, int index) {
    
}

string Event::toString() const {
    return "";
}
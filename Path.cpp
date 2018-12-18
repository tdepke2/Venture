#include "Path.h"
#include "Window.h"
#include <vector>

using namespace std;

Path::Path() {
    win = new Window();
}

Path::Path(const string& loadFile) {
    win = new Window();
    load(loadFile);
}

Path::~Path() {
    delete win;
}

void Path::load(const string& loadFile) {
    vector<vector<string>> customData = _loadFile(loadFile);
    string lineNumberStr;
    int numEntries = 0;
    try {
        for (const vector<string>& data : customData) {
            lineNumberStr = data[0];
            if (numEntries == 0 && data.size() == 3 && data[1] == "variance") {
                variance = stof(data[2]);
                ++numEntries;
            } else {
                throw runtime_error("Unexpected data type.");
            }
        }
    } catch (exception& ex) {
        throw runtime_error(loadFile + " at line " + lineNumberStr + ": " + ex.what());
    }
    if (numEntries != 1) {
        throw runtime_error(loadFile + ": Missing some custom data.");
    }
}
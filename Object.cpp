#include "Object.h"
#include "Window.h"
#include <stdexcept>

using namespace std;

const unordered_map<string, short> Object::STRING_TO_COLOR = {
    {"COLOR_BLACK", COLOR_BLACK}, {"COLOR_RED", COLOR_RED}, {"COLOR_GREEN", COLOR_GREEN}, {"COLOR_YELLOW", COLOR_YELLOW}, {"COLOR_BLUE", COLOR_BLUE}, {"COLOR_MAGENTA", COLOR_MAGENTA}, {"COLOR_CYAN", COLOR_CYAN},
    {"COLOR_WHITE", COLOR_WHITE}, {"COLOR_GRAY1", COLOR_GRAY1}, {"COLOR_GRAY2", COLOR_GRAY2}, {"COLOR_GRAY3", COLOR_GRAY3}, {"COLOR_YELLOW1", COLOR_YELLOW1}, {"COLOR_YELLOW2", COLOR_YELLOW2}, {"COLOR_YELLOW3", COLOR_YELLOW3}
};

void Object::setupColors() {
    start_color();
    init_color(COLOR_GRAY1, 300, 300, 300);
    init_color(COLOR_GRAY2, 600, 600, 600);
    init_color(COLOR_GRAY3, 900, 900, 900);
    init_color(COLOR_YELLOW1, 327, 314, 229);
    init_color(COLOR_YELLOW2, 653, 627, 457);
    init_color(COLOR_YELLOW3, 980, 941, 686);
    
    for (short i = 0; i < colorSize; ++i) {    // Pair each color with each other, setting COLOR_PAIR(0) fails because its reserved.
        for (short j = 0; j < colorSize; ++j) {
            init_pair(j + i * colorSize, j, i);
        }
    }
}

int Object::getColorPair(short foreground, short background) {
    return COLOR_PAIR(foreground + background * colorSize);
}

short Object::stringToColor(const string& s) {
    short color = colorSize;
    auto mapIter = STRING_TO_COLOR.find(s);
    if (mapIter != STRING_TO_COLOR.end()) {
        color = mapIter->second;
    }
    if (color == colorSize) {
        throw runtime_error("Invalid color (" + s + ").");
    }
    return color;
}

const string& Object::colorToString(short c) {
    for (auto mapIter = STRING_TO_COLOR.begin(); mapIter != STRING_TO_COLOR.end(); ++mapIter) {
        if (mapIter->second == c) {
            return mapIter->first;
        }
    }
    throw runtime_error("Invalid color (" + to_string(c) + ").");
}

Object::Object() {
    init();
}

Object::~Object() {}

Object::Object(const Object& object) {
    _ch1 = object._ch1;
    _ch2 = object._ch2;
    _attr1 = object._attr1;
    _attr2 = object._attr2;
    _foreground = object._foreground;
    _background = object._background;
    _cht1 = object._cht1;
    _cht2 = object._cht2;
}

void Object::setChar1(char ch) {
    _ch1 = ch;
    _updateCht1();
}

void Object::setChar2(char ch) {
    _ch2 = ch;
    _updateCht2();
}

void Object::setAttr1(int attr) {
    _attr1 = attr;
    _updateCht1();
}

void Object::setAttr2(int attr) {
    _attr2 = attr;
    _updateCht2();
}

void Object::setForeground(short color) {
    if (_foreground != color) {
        _foreground = color;
        _updateCht1();
        _updateCht2();
    }
}

void Object::setBackground(short color) {
    if (_background != color) {
        _background = color;
        _updateCht1();
        _updateCht2();
    }
}

char Object::getChar1() const {
    return _ch1;
}

char Object::getChar2() const {
    return _ch2;
}

int Object::getAttr1() const {
    return _attr1;
}

int Object::getAttr2() const {
    return _attr2;
}

short Object::getForeground() const {
    return _foreground;
}

short Object::getBackground() const {
    return _background;
}

void Object::init(char ch1, char ch2, int attr1, int attr2, short foreground, short background) {
    _ch1 = ch1;
    _ch2 = ch2;
    _attr1 = attr1;
    _attr2 = attr2;
    _foreground = foreground;
    _background = background;
    _updateCht1();
    _updateCht2();
}

void Object::init(const vector<string>& data, int& index) {
    init(data[index][0], data[index + 1][0], stoi(data[index + 2]), stoi(data[index + 3]), stringToColor(data[index + 4]), stringToColor(data[index + 5]));
    index += 6;
}

string Object::toString() const {
    string ch1Str(1, _ch1), ch2Str(1, _ch2);
    return ch1Str + "," + ch2Str + "," + to_string(_attr1) + "," + to_string(_attr2) + "," + colorToString(_foreground) + "," + colorToString(_background);
}

bool Object::isDefault() const {
    Object object;
    return _cht1 == object._cht1 && _cht2 == object._cht2;
}

void Object::draw(Window& win, int x, int y) {
    win.drawChar(_cht1, x, y);
    win.drawChar(_cht2);
}

void Object::_updateCht1() {
    _cht1 = static_cast<chtype>(_ch1 | _attr1 | getColorPair(_foreground, _background));
}

void Object::_updateCht2() {
    _cht2 = static_cast<chtype>(_ch2 | _attr2 | getColorPair(_foreground, _background));
}
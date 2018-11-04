#include "Entity.h"
#include "Tile.h"
#include "Window.h"
#include <algorithm>



#include <cassert>



using namespace std;

unordered_map<int, vector<PairXYi>> Entity::_radiusLookup;

Entity::Entity() {
    init();
    _position = PairXYi(-1, -1);
}

Entity::~Entity() {}

Entity::Entity(const Entity& entity) : Object(entity) {
    lightRadius = entity.lightRadius;
    _position = PairXYi(-1, -1);
}

bool Entity::setPosition(Window& win, const PairXYi& position) {
    if (position.x < 0 || position.x >= win.tileArraySize.x || position.y < 0 || position.y >= win.tileArraySize.y || win.tileArray[position.y][position.x]->solid || win.tileArray[position.y][position.x]->entity != nullptr) {
        return false;
    }
    
    win.tileArray[position.y][position.x]->entity = this;
    if (_position != PairXYi(-1, -1)) {
        assert(win.tileArray[_position.y][_position.x]->entity == this);
        win.tileArray[_position.y][_position.x]->entity = nullptr;
    }
    _position = position;
    return true;
}

const PairXYi& Entity::getPosition() const {
    return _position;
}

void Entity::init(int lightRadius) {
    this->lightRadius = lightRadius;
}

void Entity::init(const vector<string>& data, int& index) {
    Object::init(data, index);
    init(stoi(data[index]));
    index += 1;
}

bool Entity::move(Window& win, int dx, int dy) {
    return setPosition(win, PairXYi(_position.x + dx, _position.y + dy));
}

void Entity::resetPosition(Window& win) {
    if (win.tileArraySize != PairXYi(0, 0)) {
        assert(win.tileArray[_position.y][_position.x]->entity == this);
        win.tileArray[_position.y][_position.x]->entity = nullptr;
    }
    _position = PairXYi(-1, -1);
}

void Entity::update(Window& win, const PairXYi& playerPos) {
    _updateLight(win, playerPos);
}

string Entity::toString() const {
    return Object::toString() + "," + to_string(lightRadius);
}

Entity* Entity::makeCopy() const {
    return new Entity(*this);
}

void Entity::_updateLight(Window& win, const PairXYi& playerPos) {
    if (lightRadius <= 0 || abs(_position.x - playerPos.x) - lightRadius > (win.getSize().x - 2) / 4 || abs(_position.y - playerPos.y) - lightRadius > (win.getSize().y - 2) / 2) {
        return;
    }
    
    _traceCircle(win, 3, lightRadius / 3);
    _traceCircle(win, 2, lightRadius * 2 / 3);
    _traceCircle(win, 1, lightRadius);
}

void Entity::_traceCircle(Window& win, short val, int radius) {
    auto mapIter = _radiusLookup.find(radius);
    if (mapIter == _radiusLookup.end()) {
        pair<int, vector<PairXYi>> newPair;
        newPair.first = radius;
        mapIter = _radiusLookup.insert(newPair).first;
        if (radius <= 0) {
            return;
        }
        
        Window winTemp;
        winTemp.tileArraySize = PairXYi(radius + 1, radius + 1);
        winTemp.tileArray = new Tile**[winTemp.tileArraySize.y];
        for (int y = 0; y < winTemp.tileArraySize.y; ++y) {
            winTemp.tileArray[y] = new Tile*[winTemp.tileArraySize.x];
            for (int x = 0; x < winTemp.tileArraySize.x; ++x) {
                winTemp.tileArray[y][x] = new Tile;
            }
        }
        
        int x = -radius, y = 0, error1 = 2 - radius * 2, error2;    // Bresenham circle algorithm, draw a circle into winTemp centered at (radius, radius). http://members.chello.at/~easyfilter/bresenham.html
        do {
            _traceLine(winTemp, 1, radius, radius, radius + x, radius - y);
            mapIter->second.push_back(PairXYi(-x, y));
            
            error2 = error1;
            if (error2 <= y) {
                ++y;
                error1 += y * 2 + 1;
            }
            if (error2 > x || error1 > y) {
                ++x;
                error1 += x * 2 + 1;
            }
        } while (x < 0);
        
        for (int y = 1; y < radius; ++y) {
            int x = 1;
            while (winTemp.tileArray[y][x]->lightLevel == 0) {
                ++x;
            }
            while (x < radius) {
                if (winTemp.tileArray[y][x]->lightLevel == 0) {
                    _traceLine(winTemp, 1, radius, radius, x, y);
                    mapIter->second.push_back(PairXYi(radius - x, radius - y));
                }
                ++x;
            }
        }
        
        for (int y = 0; y < winTemp.tileArraySize.y; ++y) {
            for (int x = 0; x < winTemp.tileArraySize.x; ++x) {
                delete winTemp.tileArray[y][x];
            }
            delete[] winTemp.tileArray[y];
        }
        delete[] winTemp.tileArray;
    }
    
    if (_position.x - radius >= 0 && _position.x + radius < win.tileArraySize.x && _position.y - radius >= 0 && _position.y + radius < win.tileArraySize.y) {
        for (const PairXYi& p : mapIter->second) {
            _traceLine(win, val, _position.x, _position.y, _position.x + p.x, _position.y + p.y);
            _traceLine(win, val, _position.x, _position.y, _position.x + p.y, _position.y - p.x);
            _traceLine(win, val, _position.x, _position.y, _position.x - p.x, _position.y - p.y);
            _traceLine(win, val, _position.x, _position.y, _position.x - p.y, _position.y + p.x);
        }
    } else {
        for (const PairXYi& p : mapIter->second) {
            _traceLine(win, val, _position.x, _position.y, min(_position.x + p.x, win.tileArraySize.x - 1), min(_position.y + p.y, win.tileArraySize.y - 1));
            _traceLine(win, val, _position.x, _position.y, min(_position.x + p.y, win.tileArraySize.x - 1), max(_position.y - p.x, 0));
            _traceLine(win, val, _position.x, _position.y, max(_position.x - p.x, 0), max(_position.y - p.y, 0));
            _traceLine(win, val, _position.x, _position.y, max(_position.x - p.y, 0), min(_position.y + p.x, win.tileArraySize.y - 1));
        }
    }
}

void Entity::_traceLine(Window& win, short val, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = -abs(y1 - y0);    // Bresenham line algorithm.
    int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
    int error1 = dx + dy, error2;
    if (val > win.tileArray[y0][x0]->lightLevel) {
        win.tileArray[y0][x0]->lightLevel = val;
    }
    while ((x0 != x1 || y0 != y1) && win.tileArray[y0][x0]->transparent) {
        error2 = error1 * 2;
        if (error2 >= dy) {
            error1 += dy;
            x0 += sx;
        }
        if (error2 <= dx) {
            error1 += dx;
            y0 += sy;
        }
        if (val > win.tileArray[y0][x0]->lightLevel) {
            win.tileArray[y0][x0]->lightLevel = val;
        }
    }
}
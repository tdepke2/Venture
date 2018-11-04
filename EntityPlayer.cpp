#include "EntityPlayer.h"
#include "Tile.h"
#include "Window.h"

#include <algorithm>

using namespace std;

void EntityPlayer::load(const string& playerDataFilename) {
    
}

void EntityPlayer::update(Window& win, const PairXYi& playerPos) {
    if (Window::checkKeyState('W')) {
        move(win, 0, -1);
    }
    if (Window::checkKeyState('A')) {
        move(win, -1, 0);
    }
    if (Window::checkKeyState('S')) {
        move(win, 0, 1);
    }
    if (Window::checkKeyState('D')) {
        move(win, 1, 0);
    }
    if (Window::checkKeyState(VK_UP)) {
        lightRadius += 1;
    }
    if (Window::checkKeyState(VK_DOWN)) {
        lightRadius -= 1;
    }
    
    PairXYi halfWinSize((win.getSize().x - 2) / 4, (win.getSize().y - 2) / 2);
    PairXYi topLeft(max(0, getPosition().x - halfWinSize.x), max(0, getPosition().y - halfWinSize.y));
    PairXYi bottomRight(min(win.tileArraySize.x - 1, getPosition().x + halfWinSize.x), min(win.tileArraySize.y - 1, getPosition().y + halfWinSize.y));
    for (int y = topLeft.y; y <= bottomRight.y; ++y) {
        for (int x = topLeft.x; x <= bottomRight.x; ++x) {
            win.tileArray[y][x]->lightLevel = 0;
        }
    }
    
    _updateLight(win, getPosition());
}
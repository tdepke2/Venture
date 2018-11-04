#include "Window.h"
#include <cassert>

using namespace std;

int Window::_numActive = 0;

void Window::startCurses() {
    initscr();
    cbreak();
    noecho();
    nonl();
    ::clear();
    ::refresh();
}

void Window::endCurses() {
    assert(_numActive == 0);
    endwin();
}

bool Window::checkKeyState(int key) {
    return static_cast<bool>(GetAsyncKeyState(key) & 0x8000);
}

Window::Window() {
    tileArray = nullptr;
    _win = nullptr;
    _isActive = false;
}

Window::Window(const PairXYi& size, const PairXYi& topLeft) {
    tileArray = nullptr;
    _win = nullptr;
    _isActive = false;
    setup(size, topLeft);
}

Window::~Window() {
    deallocate();
}

const PairXYi& Window::getSize() const {
    return _size;
}

void Window::setup(const PairXYi& size, const PairXYi& topLeft) {
    assert(!isendwin() && !_isActive);
    ++_numActive;
    _win = newwin(size.y, size.x, topLeft.y, topLeft.x);
    _size = size;
    _isActive = true;
}

void Window::deallocate() {
    if (_isActive) {
        --_numActive;
        delwin(_win);
        _isActive = false;
    }
}

unsigned int Window::checkKeyTime(int key) {
    return _keyTimes[key];
}

bool Window::isKeyReady(int key, unsigned int tickTime) {
    if (_keyTimes[key] == 0 && checkKeyState(key)) {
        _keyTimes[key] = tickTime;
        return true;
    }
    return false;
}

void Window::updateKeys() {
    while (wgetch(_win) != ERR) {}
    for (auto mapIter = _keyTimes.begin(); mapIter != _keyTimes.end(); ++mapIter) {
        if (mapIter->second > 0) {
            --mapIter->second;
        }
    }
}

int Window::attrOff(int attr) {
    return wattroff(_win, attr);
}

int Window::attrOn(int attr) {
    return wattron(_win, attr);
}

int Window::clear() {
    return wclear(_win);
}

void Window::drawBorder() {
    box(_win, 0, 0);
}

void Window::drawChar(chtype c) {
    waddch(_win, c);
}

void Window::drawChar(chtype c, int x, int y) {
    mvwaddch(_win, y, x, c);
}

void Window::drawStr(const string& s) {
    waddstr(_win, s.c_str());
}

void Window::drawStr(const string& s, int x, int y) {
    mvwaddstr(_win, y, x, s.c_str());
}

int Window::erase() {
    return werase(_win);
}

int Window::getChar() {
    return wgetch(_win);
}

int Window::interruptFlush(bool state) {
    return intrflush(_win, state);
}

int Window::keypad(bool state) {
    return ::keypad(_win, state);
}

int Window::moveCur(int x, int y) {
    return wmove(_win, y, x);
}

int Window::noDelay(bool state) {
    return nodelay(_win, state);
}

int Window::refresh() {
    return wrefresh(_win);
}
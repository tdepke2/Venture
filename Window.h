#ifndef _WINDOW_H
#define _WINDOW_H

class Tile;

#include "PairXY.h"
#include <curses.h>
#include <string>
#include <unordered_map>
#undef MOUSE_MOVED
#include <Windows.h>

using namespace std;

class Window {
    public:
    Tile*** tileArray;    // Copy of data in Level, not deleted by destructor.
    PairXYi tileArraySize;
    
    static void startCurses();
    static void endCurses();
    static bool checkKeyState(int key);
    Window();
    Window(const PairXYi& size, const PairXYi& topLeft);
    ~Window();
    const PairXYi& getSize() const;
    void setup(const PairXYi& size, const PairXYi& topLeft);
    void deallocate();
    unsigned int checkKeyTime(int key);
    bool isKeyReady(int key, unsigned int tickTime);
    void updateKeys();
    int attrOff(int attr);
    int attrOn(int attr);
    int clear();
    void drawBorder();
    void drawChar(chtype c);
    void drawChar(chtype c, int x, int y);
    void drawStr(const string& s);
    void drawStr(const string& s, int x, int y);
    int erase();
    int getChar();
    int interruptFlush(bool state);
    int keypad(bool state);
    int moveCur(int x, int y);
    int noDelay(bool state);
    int refresh();
    
    protected:
    WINDOW* _win;
    PairXYi _size;
    unordered_map<int, unsigned int> _keyTimes;
    
    private:
    static int _numActive;
    bool _isActive;
};

#endif
#ifndef _GAME_H
#define _GAME_H

#include "Level.h"
#include "Window.h"
#include <fstream>
#include <random>

using namespace std;

class Game {
    public:
    static ofstream logFile;
    
    
    
    static int start();
    static int randomInteger(int min, int max);
    static int randomInteger(int n);
    
    private:
    enum class State {
        uninitialized, menu, playing, paused, exiting
    };
    
    static State state;
    static Window winGame, winUI;
    static Level level;
    static mt19937 mainRNG;
    
    static void save();
    static void load();
    static void showMenu();
    static void redraw();
    static void update();
    static void showPause();
};

#endif
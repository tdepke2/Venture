#include "Condition.h"
#include "Event.h"
#include "Game.h"
#include "Object.h"
#include "PairXY.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <curses.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using namespace std;

ofstream Game::logFile;


Game::State Game::state = State::uninitialized;
Window Game::winGame, Game::winUI;
Level Game::level;
mt19937 Game::mainRNG;

int Game::start() {
    try {
        logFile.open("log.txt", ios::trunc);
        if (!logFile.is_open()) {
            throw runtime_error("log.txt: Unable to write to log.");
        }
        
        
        
        assert(state == State::uninitialized);
        assert(Condition::STRING_TO_TYPE.size() == Condition::typeSize);
        assert(Condition::STRING_TO_LOGIC_OP.size() == Condition::logicOpSize);
        assert(Event::STRING_TO_TYPE.size() == Event::typeSize);
        assert(Object::STRING_TO_COLOR.size() == colorSize);
        state = State::menu;
        mainRNG.seed(static_cast<unsigned long>(chrono::high_resolution_clock::now().time_since_epoch().count()));
        Window::startCurses();
        Object::setupColors();
        
        PairXYi winGameSize(44, 23), winUISize(44, 23);
        PairXYi topLeft(max(COLS / 2 - (winGameSize.x + winUISize.x) / 2, 0), max(LINES / 2 - winGameSize.y / 2, 0));
        winGame.setup(winGameSize, topLeft);
        winGame.keypad(true);
        winGame.interruptFlush(true);
        topLeft.x += winGameSize.x;
        winUI.setup(winUISize, topLeft);
        level.win = &winGame;
        
        while (state != State::exiting) {
            if (state == State::menu) {    // Main menu.
                showMenu();
                while (state == State::menu) {
                    int ch = winGame.getChar();
                    if (ch == 's') {
                        load();
                        state = State::playing;
                    } else if (ch == 'q') {
                        state = State::exiting;
                    }
                }
            }
            
            winGame.noDelay(true);
            if (state == State::playing) {    // Game loop.
                while (state == State::playing) {
                    update();
                    redraw();
                    this_thread::sleep_for(chrono::milliseconds(Window::checkKeyState(VK_LSHIFT) ? 50 : 100));
                }
            }
            while (winGame.getChar() != ERR) {}
            winGame.noDelay(false);
            
            if (state == State::paused) {    // Game paused.
                showPause();
                while (state == State::paused) {
                    int ch = winGame.getChar();
                    if (ch == 'p') {
                        state = State::playing;
                    } else if (ch == 'm') {
                        save();
                        state = State::menu;
                    } else if (ch == 'q') {
                        save();
                        state = State::exiting;
                    }
                }
            }
        }
    } catch(exception& ex) {    // Catch any exceptions here and handle them as a game crash.
        winGame.deallocate();
        winUI.deallocate();
        Window::endCurses();
        
        cout << "\n*******************************************" << endl;
        cout << "Oh noes! The game crashed unexpectedly. X_X" << endl;
        cout << "\n- Details -" << endl;
        cout << "  Exception thrown: " << ex.what() << endl;
        cout << "(Press enter)" << endl;
        cout << "*******************************************" << endl;
        cin.get();
        
        return 1;
    }
    
    winGame.deallocate();
    winUI.deallocate();
    Window::endCurses();
    
    return 0;
}

int Game::randomInteger(int min, int max) {
    uniform_int_distribution<int> minMaxRange(min, max);
    return minMaxRange(mainRNG);
}

int Game::randomInteger(int n) {
    return randomInteger(0, n - 1);
}

void Game::save() {
    //level.save("levels/level0.csv");
    level.save("levels/testDungeon.csv");
}

void Game::load() {
    //level.load("levels/level0.csv");
    level.generateLevel("structures/", 0);
}

void Game::showMenu() {
    winGame.erase();
    winGame.drawBorder();
    winGame.moveCur(winGame.getSize().x / 2 - 18, winGame.getSize().y / 2);
    winGame.attrOn(Object::getColorPair(COLOR_BLACK, COLOR_GREEN));
    winGame.drawStr("Main menu, [s] to start, [q] to quit.");
    winGame.attrOff(Object::getColorPair(COLOR_BLACK, COLOR_GREEN));
    winUI.erase();
    winUI.drawBorder();
    
    winGame.refresh();
    winUI.refresh();
}

void Game::redraw() {
    winGame.erase();
    winGame.drawBorder();
    winGame.moveCur(1, 1);
    level.drawLevel();
    winUI.erase();
    winUI.drawBorder();
    
    winGame.refresh();
    winUI.refresh();
}

void Game::update() {
    winGame.updateKeys();
    
    if (winGame.isKeyReady('P', 10)) {
        state = State::paused;
    } else {
        level.updateAll();
    }
}

void Game::showPause() {
    redraw();
    winGame.drawStr("Game paused, press [p], [m], or [q].", winGame.getSize().x / 2 - 18, winGame.getSize().y / 2);
    winGame.refresh();
}
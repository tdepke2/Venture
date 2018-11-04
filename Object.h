#ifndef _OBJECT_H
#define _OBJECT_H

class Window;

#include <curses.h>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

enum Color : short {    // First 8 colors are declared by curses: COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE.
    COLOR_GRAY1 = 8, COLOR_GRAY2, COLOR_GRAY3, COLOR_YELLOW1, COLOR_YELLOW2, COLOR_YELLOW3, colorSize
};

class Object {
    public:
    static const unordered_map<string, short> STRING_TO_COLOR;
    
    static void setupColors();
    static int getColorPair(short foreground, short background);
    static short stringToColor(const string& s);
    static const string& colorToString(short c);
    Object();
    virtual ~Object();
    Object(const Object& object);
    void setChar1(char ch);
    void setChar2(char ch);
    void setAttr1(int attr);
    void setAttr2(int attr);
    void setForeground(short color);
    void setBackground(short color);
    char getChar1() const;
    char getChar2() const;
    int getAttr1() const;
    int getAttr2() const;
    short getForeground() const;
    short getBackground() const;
    void init(char ch1 = ' ', char ch2 = ' ', int attr1 = A_NORMAL, int attr2 = A_NORMAL, short foreground = COLOR_WHITE, short background = COLOR_BLACK);
    void init(const vector<string>& data, int& index);
    string toString() const;
    bool isDefault() const;
    virtual void draw(Window& win, int x, int y);
    
    private:
    char _ch1, _ch2;
    int _attr1, _attr2;
    short _foreground, _background;
    chtype _cht1, _cht2;
    
    void _updateCht1();
    void _updateCht2();
};

#endif
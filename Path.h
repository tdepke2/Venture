#ifndef _PATH_H
#define _PATH_H

#include "Level.h"
#include <string>

using namespace std;

class Path : public Level {
    public:
    float variance;
    
    Path();
    Path(const string& loadFile);
    ~Path();
    void load(const string& loadFile);
};

#endif
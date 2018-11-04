#ifndef _PAIRXY_H
#define _PAIRXY_H

#define PairXYi PairXY<int>
#define PairXYf PairXY<float>

#include <iostream>

using namespace std;

template<typename T>
class PairXY {
    public:
    T x, y;
    
    PairXY() : x(0), y(0) {}
    PairXY(T X, T Y) : x(X), y(Y) {}
    template<typename U>
    PairXY(const PairXY<U>& pair2) : x(pair2.x), y(pair2.y) {}
};

template<typename T>
PairXY<T> operator-(const PairXY<T>& a) {
    return PairXY<T>(-a.x, -a.y);
}

template<typename T>
PairXY<T> operator+(const PairXY<T>& a, const PairXY<T>& b) {
    return PairXY<T>(a.x + b.x, a.y + b.y);
}

template<typename T>
PairXY<T>& operator+=(PairXY<T>& a, const PairXY<T>& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

template<typename T>
PairXY<T> operator-(const PairXY<T>& a, const PairXY<T>& b) {
    return PairXY<T>(a.x - b.x, a.y - b.y);
}

template<typename T>
PairXY<T>& operator-=(PairXY<T>& a, const PairXY<T>& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

template<typename T>
PairXY<T> operator*(const PairXY<T>& a, T b) {
    return PairXY<T>(a.x * b, a.y * b);
}

template<typename T>
PairXY<T> operator*(T a, const PairXY<T>& b) {
    return PairXY<T>(a * b.x, a * b.y);
}

template<typename T>
PairXY<T>& operator*=(PairXY<T>& a, T b) {
    a.x *= b;
    a.y *= b;
    return a;
}

template<typename T>
PairXY<T> operator/(const PairXY<T>& a, T b) {
    return PairXY<T>(a.x / b, a.y / b);
}

template<typename T>
PairXY<T>& operator/=(PairXY<T>& a, T b) {
    a.x /= b;
    a.y /= b;
    return a;
}

template<typename T>
bool operator==(const PairXY<T>& a, const PairXY<T>& b) {
    return (a.x == b.x && a.y == b.y);
}

template<typename T>
bool operator!=(const PairXY<T>& a, const PairXY<T>& b) {
    return (a.x != b.x || a.y != b.y);
}

template<typename T>
ostream& operator<<(ostream& out, const PairXY<T>& p) {
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}

#endif
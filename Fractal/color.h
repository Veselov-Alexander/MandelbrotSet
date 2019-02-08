#ifndef COLOR_H 
#define COLOR_H

#include "glut.h"

typedef long double T;

struct Color
{
    T R, G, B;
    explicit Color(T R = 0, T G = 0, T B = 0);
    void set() const;
};

#endif
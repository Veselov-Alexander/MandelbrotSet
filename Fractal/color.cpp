#include "color.h"

Color::Color(T R, T G, T B) :
    R(R), G(G), B(B)
{}

void Color::set() const
{
    glColor3f(R, G, B);
}

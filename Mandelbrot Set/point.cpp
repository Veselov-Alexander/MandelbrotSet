#include "point.h"


template <typename T>
Point<T>::Point(T x, T y) :
    x(x), y(y)
{}

template <typename T>
Point<T> Point<T>::operator+(const Point<T> &point) const
{
    return Point(x + point.x, y + point.y);
}

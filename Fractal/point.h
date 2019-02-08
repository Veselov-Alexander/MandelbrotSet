#ifndef POINT_H
#define POINT_H

template <typename T>
struct Point
{
    T x, y;

    explicit Point(T x = 0, T y = 0);

    Point operator+(const Point &point) const;
};

#endif

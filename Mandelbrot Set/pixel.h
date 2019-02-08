#ifndef PIXEL_H
#define PIXEL_H

#include "color.h"
#include "point.h"


template <typename T>
struct Pixel
{
    Point<T>                    location, point, z;
    unsigned                    iterations;
    Color                       color;

    unsigned mandelbrot();
    explicit Pixel(const Point<T> &location = Point<T>());
    Color display_type_1(unsigned iterations);
    Color display_type_2(unsigned iterations);
    Color display_type_3(unsigned iterations);
    Color display_type_4(unsigned iterations);
    void update();
    void draw() const;
};

#endif

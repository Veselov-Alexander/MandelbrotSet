#include "pixel.h"
#include "program.h"


template <typename T>
Pixel<T>::Pixel(const Point<T> &location) :
    location(location),
    point(location.x * zoom + offset.x,
          location.y * zoom + offset.y),
    iterations(0)
{
    update();
}

template <typename T>
unsigned Pixel<T>::mandelbrot()
{
    while (iterations < max_iterations)
    {
        z = Point<T>(z.x * z.x - z.y * z.y, 2 * z.x * z.y) + point;

        if (z.x * z.x + z.y * z.y > 4.0f)
        {
            return iterations;
        }

        ++iterations;
    }

    return max_iterations;
}

template <typename T>
void Pixel<T>::update()
{
    unsigned it = mandelbrot();
    switch (mode)
    {
    case 1:
        color = display_type_1(it);
        break;
    case 2:
        color = display_type_2(it);
        break;
    case 3:
        color = display_type_3(it);
        break;
    case 4:
        color = display_type_4(it);
        break;
    }

}

template<typename T>
Color Pixel<T>::display_type_1(unsigned iterations)
{
    T intense = 1.0 - static_cast<T>(iterations) / max_iterations;
    return Color(intense, intense, intense);
}

template <typename T>
Color Pixel<T>::display_type_2(unsigned iterations)
{
    T intense = static_cast<T>(iterations) / max_iterations;
    return Color(intense, intense, intense);
}

template <typename T>
Color Pixel<T>::display_type_3(unsigned iterations)
{
    double r, g, b;
    double r1 = 0, b1 = max_iterations / 16, gr = max_iterations / 8,
           r2 = max_iterations / 4, b2 = max_iterations / 2;

    if (iterations < 0)
    {
        return Color();
    }

    if (iterations == 0) 
    {
        r = max_iterations;
        g = 0;
        b = 0;
    }
    else 
    {
        if (iterations < b1) {
            r = b1 * (b1 - iterations);
            g = 0;
            b = b1 * iterations - 1;
        }
        else if (iterations < gr)
        {
            r = 0;
            g = b1 * (iterations - b1);
            b = b1 * (gr - iterations) - 1;
        }
        else if (iterations < r2)
        {
            r = 8 * (iterations - gr);
            g = 8 * (r2 - iterations) - 1;
            b = 0;
        }
        else 
        {
            r = max_iterations - (iterations - r2) * 4;
            g = 0;
            b = 0;
        }
    }

    return Color(r / max_iterations, g / max_iterations, b / max_iterations);
}

template<typename T>
Color Pixel<T>::display_type_4(unsigned iterations)
{
    if (iterations > max_iterations * 0.8)
    {
        return Color(1, 1, 1);
    }
    else
    {
        return Color(0, 0, 0);
    }
}

template <typename T>
void Pixel<T>::draw() const
{
    glBegin(GL_POINTS);
    color.set();
    glVertex2d(location.x, location.y);
    glEnd();
}
#include "program.h"
#include "pixel.cpp"
#include "point.cpp"

unsigned                    width, height;
unsigned                    max_iterations;
unsigned                    mode;
double                      multiplier;
T                           default_zoom, zoom;
vector<vector<Pixel<T>>>    map;
Point<T>                    offset;
bool                        need_to_display;

void initialize(unsigned _width, unsigned _height, unsigned _max_iterations)
{
    width = _width;
    height = _height;
    need_to_display = true;
    max_iterations = _max_iterations;
    multiplier = 1.0;
    mode = 2;
    default_zoom = 2.5 / min(width, height);
    zoom = default_zoom;
    offset = Point<T>(-zoom / 5.0 * width, 0);
    map.assign(width, vector<Pixel<T>>(height));
    process_map();
}

void process_map()
{
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            Point<T> point(x - width / 2.0, y - height / 2.0);
            map[x][y] = Pixel<T>(point);
        }
    }
    need_to_display = true;
}

void display()
{
    if (!need_to_display)
    {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (vector<Pixel<T>> &line : map)
    {
        for (Pixel<T> &pixel : line)
        {
            pixel.draw();
        }
    }
    glEnd();
    glutSwapBuffers();
    need_to_display = false;
}

void keyboard(unsigned char key, int x, int y)
{
    long double dx = zoom / 10.0 * width;
    long double dy = zoom / 10.0 * height;
    switch (key)
    {
    case 'e':
        zoom *= 0.8;
        update_title();
        break;
    case 'q':
        zoom /= 0.8;
        update_title();
        break;
    case 'w':
        offset = Point<T>(offset.x, offset.y - dy);
        break;
    case 'a':
        offset = Point<T>(offset.x - dx, offset.y);
        break;
    case 's':
        offset = Point<T>(offset.x, offset.y + dy);
        break;
    case 'd':
        offset = Point<T>(offset.x + dx, offset.y);
        break;
    case '+':
        max_iterations += 64;
        update_title();
        break;
    case '-':
        max_iterations -= 64;
        update_title();
        break;
    case 'x':
        save_image();
        return;
    case '1':
        mode = 1;
        break;
    case '2':
        mode = 2;
        break;
    case '3':
        mode = 3;
        break;
    case '4':
        mode = 4;
        break;
    case '[':
        multiplier -= 0.25;
        update_title();
        return;
    case ']':
        multiplier += 0.25;
        update_title();
        return;
    default:
        return;
    }
    process_map();
}

void update_title()
{
    std::string title = "Zoom: " + std::to_string(default_zoom / zoom) +
                        "x [" + std::to_string(max_iterations) +
                        ", " + std::to_string(multiplier).substr(0, 4) +"]";
    glutSetWindowTitle(title.c_str());
}

void save_image()
{
    unsigned new_width = width * multiplier;
    unsigned new_height = height * multiplier;

    vector<vector<Pixel<T>>> new_map;

    new_map.assign(new_width, vector<Pixel<T>>(new_height));
    zoom /= multiplier;
    for (int x = 0; x < new_width; ++x)
    {
        for (int y = 0; y < new_height; ++y)
        {
            Point<T> point(x - new_width / 2.0, y - new_height / 2.0);
            new_map[x][y] = Pixel<T>(point);
        }
        if (x % 50 == 0)
        {
            std::cout << double(x) / new_width * 100 << "           \r";
        }
    }
    std::cout << 100 << "%           \r";
    zoom *= multiplier;

    Image image(new_map);

    std::time_t date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    image.save(std::ctime(&date));
}
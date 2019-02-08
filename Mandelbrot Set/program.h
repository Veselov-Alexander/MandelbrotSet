#ifndef PROGRAM_H
#define PROGRAM_H

#include <ctime>
#include <chrono>
#include <vector>

#include "glut.h"
#include "point.h"
#include "pixel.h"
#include "image.h"

using std::vector;
typedef long double T;

extern unsigned                    width, height;
extern unsigned                    max_iterations;
extern unsigned                    mode;
extern double                      multiplier;
extern T                           default_zoom, zoom;
extern vector<vector<Pixel<T>>>    map;
extern Point<T>                    offset;

void process_map();
void initialize(unsigned width, unsigned height, unsigned max_iterations);
void display();
void keyboard(unsigned char key, int x, int y);
void update_title();
void save_image();

#endif
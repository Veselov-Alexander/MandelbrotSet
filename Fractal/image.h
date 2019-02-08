#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include "pixel.h"

using std::vector;
typedef long double T;

extern const char *SCREENSHOT_FOLDER;

class Image
{
public:
    Image(const vector<vector<Pixel<T>>> &map);
    ~Image();
    void save(const std::string &file);
private:
    bool create(int width, int height);
    void set_pen_color(DWORD clr);
    bool save_bitmap(const char* path);

    HBITMAP bmp_;
    HDC hdc_;
    HPEN pen_;
    int width_, height_;
    vector<vector<Pixel<T>>> map;
};

#endif

#include "image.h"

const char *SCREENSHOT_FOLDER = "Screenshots";

Image::Image(const vector<vector<Pixel<T>>> &map) :
    map(map), pen_(nullptr)
{
    create(map.size(), map[0].size());
}

Image::~Image()
{
    DeleteObject(pen_);
    DeleteDC(hdc_);
    DeleteObject(bmp_);
}

bool Image::create(int width, int height)
{
    BITMAPINFO	bi;
    ZeroMemory(&bi, sizeof(bi));

    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biBitCount = sizeof(DWORD) * 8;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biWidth = width;
    bi.bmiHeader.biHeight = -height;

    void *bits_ptr = nullptr;
    HDC dc = GetDC(GetConsoleWindow());
    bmp_ = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits_ptr, nullptr, 0);

    if (!bmp_)
    {
        return false;
    }

    hdc_ = CreateCompatibleDC(dc);
    SelectObject(hdc_, bmp_);
    ReleaseDC(GetConsoleWindow(), dc);

    width_ = width;
    height_ = height;

    return true;
}

void Image::set_pen_color(DWORD clr)
{
    if (pen_)
    {
        DeleteObject(pen_);
    }
    pen_ = CreatePen(PS_SOLID, 1, clr);
    SelectObject(hdc_, pen_);
}

bool Image::save_bitmap(const char* path)
{
    HANDLE file = CreateFile(path, GENERIC_WRITE, 0, nullptr, 
                             CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    BITMAPFILEHEADER fileheader;
    BITMAPINFO infoheader;
    BITMAP bitmap;
    GetObject(bmp_, sizeof(bitmap), &bitmap);

    DWORD* dwp_bits = new DWORD[bitmap.bmWidth * bitmap.bmHeight];
    ZeroMemory(dwp_bits, bitmap.bmWidth * bitmap.bmHeight * sizeof(DWORD));
    ZeroMemory(&infoheader, sizeof(BITMAPINFO));
    ZeroMemory(&fileheader, sizeof(BITMAPFILEHEADER));

    infoheader.bmiHeader.biBitCount = sizeof(DWORD) * 8;
    infoheader.bmiHeader.biCompression = BI_RGB;
    infoheader.bmiHeader.biPlanes = 1;
    infoheader.bmiHeader.biSize = sizeof(infoheader.bmiHeader);
    infoheader.bmiHeader.biHeight = bitmap.bmHeight;
    infoheader.bmiHeader.biWidth = bitmap.bmWidth;
    infoheader.bmiHeader.biSizeImage = bitmap.bmWidth * bitmap.bmHeight * sizeof(DWORD);

    fileheader.bfType = 0x4D42;
    fileheader.bfOffBits = sizeof(infoheader.bmiHeader) + sizeof(BITMAPFILEHEADER);
    fileheader.bfSize = fileheader.bfOffBits + infoheader.bmiHeader.biSizeImage;

    GetDIBits(hdc_, bmp_, 0, height_, (LPVOID)dwp_bits, &infoheader, DIB_RGB_COLORS);

    DWORD wb;
    WriteFile(file, &fileheader, sizeof(BITMAPFILEHEADER), &wb, nullptr);
    WriteFile(file, &infoheader.bmiHeader, sizeof(infoheader.bmiHeader), &wb, nullptr);
    WriteFile(file, dwp_bits, bitmap.bmWidth * bitmap.bmHeight * 4, &wb, nullptr);
    CloseHandle(file);

    delete[] dwp_bits;
    return true;
}

void Image::save(const std::string &file)
{
    std::string filename = file.substr(0, file.size() - 1);
    CreateDirectory(SCREENSHOT_FOLDER, NULL);
    filename = std::string(SCREENSHOT_FOLDER) + "/" + filename;
    for (int i = 0; i < filename.size(); ++i)
    {
        if (filename[i] == ':')
        {
            filename[i] = '-';
        }
    }
    filename += ".bmp";

    int start_x = -map[0][0].location.x;
    int start_y = -map[0][0].location.y;
    for (vector<Pixel<T>> &line : map)
    {
        for (Pixel<T> &pixel : line)
        {
            unsigned R = max(0, round(pixel.color.R * 255));
            unsigned G = max(0, round(pixel.color.G * 255));
            unsigned B = max(0, round(pixel.color.B * 255));

            SetPixel(hdc_, start_x + pixel.location.x, start_y + pixel.location.y, RGB(R, G, B));
        }
    }
    std::cout << filename << " is saved.\n";
    save_bitmap(filename.c_str());
}

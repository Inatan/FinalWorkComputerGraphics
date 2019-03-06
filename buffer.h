#ifndef BUFFER_H
#define BUFFER_H
#include "colorVector.h"
#include <cstdlib>


#define COLOR 0
#define DEPTH 1

class buffer
{
public:
    buffer();
    virtual ~buffer();
    float *depth;
    colorVector *colors;
    int width, height;
    bool checkDepth;
    buffer(int width, int height);
    colorVector getColor(int x, int y);
    void defineSize(int w, int h);
    void setPixelColor(int x, int y, colorVector color);
    void setPixel(int x, int y, float _depth, colorVector color);
    float getDepth(int x, int y);
    void setDepth(int x, int y, float _depth);
    void clearColor();
    void clearDepth();
    void setCheckDepth(bool enable);
};

#endif // BUFFER_H

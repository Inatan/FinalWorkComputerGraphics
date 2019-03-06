#include "buffer.h"

buffer::buffer()
{
}

buffer::~buffer()
{
    if(colors!=NULL){
        free(depth);
        free(colors);
    }
}

buffer::buffer(int width, int height)
{
    checkDepth = false;
    defineSize(width, height);
}

void buffer::defineSize(int w, int h)
{
    width =w;
    height =h;
    depth  = (float *)malloc(width * height * sizeof(float));
    colors = (colorVector *)malloc(width * height * sizeof(colorVector));

}

void buffer::setPixelColor(int x, int y, colorVector color)
{
    int i = x+y*width;
    colors[i] = color;
}
void buffer::setPixel(int x, int y, float _depth, colorVector color)
{
    int i = x+y*width;
    //printf("(%d+%d)*%f = %d",x,y,width,i);
    if(checkDepth==false || (checkDepth==true && ((depth[i] >= _depth && depth[i] != 0.0) || (depth[i] == 0.0))))
    {
        colors[i] = color;
        depth[i] = _depth;
    }

}
float buffer::getDepth(int x, int y)
{
    return depth[x+y*width];
}

void buffer::setDepth(int x, int y, float _depth)
{
    depth[x+y*width] = _depth;
}

void buffer::clearColor()
{
    for(int i=0; i<width*height; i++)
    {
        colors[i] = colorVector();
    }
}
void buffer::clearDepth()
{
    for(int i=0; i<width*height; i++)
        depth[i] = 0.0f;
}

void buffer::setCheckDepth(bool enable)
{
    checkDepth = enable;
}

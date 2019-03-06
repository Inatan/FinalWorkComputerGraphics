#include "pixelLine.h"

pixelLine::pixelLine(const colorVector &color1_, int x1_, int y1_, float depth1_, float w1_,vector3f textel1_,const colorVector &color2_, int x2_, int y2_, float depth2_, float w2_, vector3f textel2_)
{
    if(y1_ < y2_ || (y1_==y2_ && x1_<x2_))
    {
        color1 = color1_;
        x1 = x1_;
        y1 = y1_;
        depth1=depth1_;
        w1 = w1_;
        textel1=textel1_;
        color2 = color2_;
        x2 = x2_;
        y2 = y2_;
        depth2=depth2_;
        w2 = w2_;
        textel2=textel2_;
    }
    else
    {
        color1 = color2_;
        x1 = x2_;
        y1 = y2_;
        depth1=depth2_;
        w1 = w2_;
        textel1=textel2_;
        color2 = color1_;
        x2 = x1_;
        y2 = y1_;
        depth2=depth1_;
        w2 = w1_;
        textel2=textel1_;
    }
    length=y2-y1;
}

pixelLine::~pixelLine()
{
    //dtor
}

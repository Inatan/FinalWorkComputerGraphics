#ifndef PIXELLINE_H
#define PIXELLINE_H
#include "colorVector.h"
#include "vector3f.h"
#include <math.h>

class pixelLine
{
    public:
        colorVector color1, color2;
        vector3f textel1, textel2;
		int x1, y1, x2, y2,length;
		float depth1, depth2, w1, w2;
        pixelLine(const colorVector &color1_, int x1_, int y1_, float depth1_, float w1_,vector3f textel1_,const colorVector &color2_, int x2_, int y2_, float depth2_, float w2_, vector3f textel2_);
        virtual ~pixelLine();
};

#endif // PIXELLINE_H

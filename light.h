#ifndef LIGHT_H
#define LIGHT_H
#include "vector3f.h"
#include "colorVector.h"

class light
{
    public:

        colorVector diffuse,ambient,specular,color;
        vector3f position;
        light();
        light(colorVector d, colorVector a, colorVector s,colorVector c,vector3f pos){
            diffuse=d;
            ambient=a;
            specular=s;
            color=c;
            position=pos;
        }
};

#endif // LIGHT_H

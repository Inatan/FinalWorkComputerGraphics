#ifndef TRIANGLE_H
#define TRIANGLE_H
#define NUMBER_OF_EDGES 3

#include "vector3f.h"

class triangle
{
public:
    triangle();
    vector3f v0;
    vector3f v1;
    vector3f v2;
    vector3f normal[NUMBER_OF_EDGES];
    vector3f textel[NUMBER_OF_EDGES];
    vector3f face_normal;
    float Color[NUMBER_OF_EDGES];
};

#endif // TRIANGLE_H

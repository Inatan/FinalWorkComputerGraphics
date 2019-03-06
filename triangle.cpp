#include "triangle.h"

triangle::triangle()
{
    v0=vector3f();
    v1=vector3f();
    v2=vector3f();
    normal[0] = vector3f();
    normal[1] = vector3f();
    normal[2] = vector3f();
    face_normal = vector3f();
    textel[0]= vector3f(0.0f,0.0f,1.0f);
    textel[1]= vector3f(0.0f,0.0f,1.0f);
    textel[2]= vector3f(0.0f,0.0f,1.0f);
}

#ifndef FILEREADER_H
#define FILEREADER_H
#define MAX_MATERIAL_COUNT 1
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "triangle.h"
#include <cfloat>
#include <GL/glui.h>


using namespace std;

class filereader
{
public:
    filereader();
    float   maxX,minX,maxY,minY,maxZ,minZ;
    int NumTris;
    int color_index[3];
    vector3f    ambient[MAX_MATERIAL_COUNT],
                diffuse[MAX_MATERIAL_COUNT],
                specular[MAX_MATERIAL_COUNT];
    float shine[MAX_MATERIAL_COUNT];
    vector<triangle> Tris;
    void readfile(const char *FileName);
    void read_texture_image_and_set(float *pixels, int w, int h);
    bool fileRead=false;
    bool textureEnable;
    GLuint texName;
};
/************


******/
#endif // FILEREADER_H

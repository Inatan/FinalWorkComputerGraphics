#ifndef TEXTURE_H
#define TEXTURE_H
#include <cstdlib>
#include "colorVector.h"
#include <cmath>
#include <GL/glui.h>
#include <vector>
#include "vector3f.h"
#define NEAREST 0
#define BILINEAR 1
#define MIPMAP 2

class Texture
{
    public:
        int width, height, mode;
        colorVector *colors;
        GLuint id;
        vector<Texture> children;
        Texture();
        void defineSize(float s, float t);
        void setMode(int m);
        colorVector getColor(float s, float t,int x,int y);
        colorVector getBilinearColor(float s, float t,int level);
        void setColor(int x, int y, float R,float G,float B);
        void setColor(colorVector p, int s, int t);
        float *colorsgl;
        void buildPyramidLevel(int level);
        void buildPyramid();
        void setTexture(colorVector* p);
        void geraTroncoPiramide(int nivel);
        colorVector getMipMapTex(int _x, int _y, float _s, float _t);
        virtual ~Texture();
    protected:
    private:
};

#endif // TEXTURE_H

#ifndef _CLOSE2GL_H_
#define _CLOSE2GL_H_

#include "filereader.h"
#include "camera.h"
#include "triangle.h"
#include "matrix4x4.h"
#include "buffer.h"
#include "light.h"
#include "pixelLine.h"
#include <vector>
#include <math.h>
#include <GL/glui.h>
#include "Texture.h"

#define ENABLE true
#define DISABLE false
#define POINTS 0
#define WIREFRAME 1
#define SOLID 2
#define CW 0
#define CCW 1
#define FLAT 0
#define GOURAUD 1


class Close2GL{
	public:
		Camera c;
		buffer C2GLbuffer;
		colorVector clearc, color;
		light Light;
		matrix4x4f ModelView,Projection,Viewport;
		bool backfaceCulling,ilumination,textEn;
		int format, cullingMode,shading;
		filereader *object;
		Texture tex;
		Close2GL(){
		    C2GLbuffer = buffer();
			c= Camera();
			ilumination=ENABLE;
			shading=GOURAUD;
			backfaceCulling = ENABLE;
			cullingMode = CCW;
			format = SOLID;
			C2GLsetModelViewMatrix();
			C2GLsetProjectionMatrix();
		}
		void C2GLsetModelViewMatrix();
		void Close2GLtranslatef(float x, float y, float z);
		void Close2GLscalef(float x, float y, float z);
		void C2GLrotatef(float angle,float x, float y, float z);
		void C2GLookat(float eyeX,float eyeY,float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ);
		void C2GLperspective(float fovY, float aspectRatio, float near, float far);
		void C2GLsetProjectionMatrix();
		void C2GLfrustum(float left, float right,float bottom, float top, float near, float far);
		void C2GLOrtho2D(float  lv,float rv, float bv, float tv);
		void C2GLCullingOrientation(int orientantion);
		void C2GLCull(bool cull);
		void C2GLDrawMode(int mode);
		void C2GLrender(filereader *reader);
		void C2GLmapObject(triangle t);
		void printMatrix(matrix4x4f matrix);
		void printVector(vector3f v);
		void Drawline( const colorVector &color0, vector3f v0, float w0,const colorVector &color1, vector3f v1, float w1);
		void DrawSpan(const pixelLine &line, int y);
		void DrawSpansBetweenEdges(pixelLine e1,pixelLine e2);
		void DrawTriangle(const colorVector &color0, vector3f v0, float w0,vector3f tex0,const colorVector &color1, vector3f v1, float w1,vector3f tex1,const colorVector &color2, vector3f v2, float w2,vector3f tex2);
        colorVector phongLighintModel(float d, vector3f N);
        void C2GLShadingMode(int mode);
        void C2GLenableTex(bool enable);
        void updateLight(float * ambient, float * diffuse, float * specular,float R,float G,float B,float A ,float * position);
        void C2GLZbufferMode(bool zbuffering);
        void read_texture_image_and_set(colorVector *textel, int w, int h);
};
#endif

#include "close2gl.h"

/** Gera Modelview Matrix **/
void Close2GL::C2GLsetModelViewMatrix()
{
    ModelView.m[0] = c.u.x;
    ModelView.m[1] = c.v.x;
    ModelView.m[2] = c.n.x;
    ModelView.m[3] = 0;
    ModelView.m[4] = c.u.y;
    ModelView.m[5] = c.v.y;
    ModelView.m[6] = c.n.y;
    ModelView.m[7] = 0;
    ModelView.m[8] = c.u.z;
    ModelView.m[9] = c.v.z;
    ModelView.m[10] = c.n.z;
    ModelView.m[11] = 0 ;
    ModelView.m[12] = -dotProduct(c.u,c.position);
    ModelView.m[13] = -dotProduct(c.v,c.position);
    ModelView.m[14] = -dotProduct(c.n,c.position);
    ModelView.m[15] = 1;
}

/****** Realiza translação na modelview matrix *******/
void Close2GL::Close2GLtranslatef(float x, float y, float z)
{
    matrix4x4f result;
    result.m[0] = 1;
    result.m[1] = 0;
    result.m[2] = 0;
    result.m[3] = 0;
    result.m[4] = 0;
    result.m[5] = 1;
    result.m[6] = 0;
    result.m[7] = 0;
    result.m[8] = 0;
    result.m[9] = 0;
    result.m[10] = 1;
    result.m[11] = 0;
    result.m[12] = x;
    result.m[13] = y;
    result.m[14] = z;
    result.m[15] = 1;
    ModelView=result*ModelView;
}

/********** Operação de escala ******************/
void Close2GL::Close2GLscalef(float x, float y, float z)
{
    matrix4x4f result;
    result.m[0] = x;
    result.m[1] = 0;
    result.m[2] = 0;
    result.m[3] = 0;
    result.m[4] = 0;
    result.m[5] = y;
    result.m[6] = 0;
    result.m[7] = 0;
    result.m[8] = 0;
    result.m[9] = 0;
    result.m[10] = z;
    result.m[11] = 0;
    result.m[12] = 0;
    result.m[13] = 0;
    result.m[14] = 0;
    result.m[15] = 1;
    ModelView=ModelView*result;
}

void Close2GL::C2GLrotatef(float angle,float x, float y, float z)
{
    matrix4x4f result;
    angle*= (x+y+z);
    float rad= (angle*PI)/180.0f;
    if(abs(x)==1.0f && y==0.0f && z==0.0f)
    {
        result.m[0] = 1;
        result.m[1] = 0;
        result.m[2] = 0;
        result.m[3] = 0;
        result.m[4] = 0;
        result.m[5] = cos(rad);
        result.m[6] = sin(rad);
        result.m[7] = 0;
        result.m[8] = 0;
        result.m[9] = -sin(rad);
        result.m[10] = cos(rad);
        result.m[11] = 0;
        result.m[12] = 0;
        result.m[13] = 0;
        result.m[14] = 0;
        result.m[15] = 1;
    }
    else if(x==0.0f && abs(y)==1.0f && z==0.0f)
    {
        result.m[0] = cos(rad);
        result.m[1] = 0;
        result.m[2] = -sin(rad);
        result.m[3] = 0;
        result.m[4] = 0;
        result.m[5] = 1;
        result.m[6] = 0;
        result.m[7] = 0;
        result.m[8] = sin(rad);
        result.m[9] = 0;
        result.m[10] = cos(rad);
        result.m[11] = 0;
        result.m[12] = 0;
        result.m[13] = 0;
        result.m[14] = 0;
        result.m[15] = 1;
    }
    else if(x==0.0f && y==0.0f && abs(z)==1.0f)
    {
        result.m[0] = cos(rad);
        result.m[1] = sin(rad);
        result.m[2] = 0;
        result.m[3] = 0;
        result.m[4] = -sin(rad);
        result.m[5] = cos(rad);
        result.m[6] = 0;
        result.m[7] = 0;
        result.m[8] = 0;
        result.m[9] = 0;
        result.m[10] = 1;
        result.m[11] = 0;
        result.m[12] = 0;
        result.m[13] = 0;
        result.m[14] = 0;
        result.m[15] = 1;
    }
    ModelView=result*ModelView;
}

/************ Gera posição e visão da camera na modelview matrix  ****************/
void Close2GL::C2GLookat(float eyeX,float eyeY,float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ)
{
    vector3f eye, look, up;
    eye = vector3f(eyeX,eyeY,eyeZ);
    look = vector3f(lookX,lookY,lookZ);
    up = vector3f(upX,upY,upZ);
    c.position= eye;
    c.lookat=look;
    c.n = eye-look;
    c.n.normalize();
    c.u = crossProduct(up,c.n);
    c.u.normalize();
    c.v = crossProduct(c.n, c.u);
    c.v.normalize();
    // gera elementos para a modelviewmatrix para a posição e visão de camera no espaço
    ModelView.m[0] =  c.u.x;
    ModelView.m[1] =  c.v.x;
    ModelView.m[2] =  c.n.x;
    ModelView.m[3] =  0.0f;
    ModelView.m[4] =  c.u.y;
    ModelView.m[5] =  c.v.y;
    ModelView.m[6] = c.n.y;;
    ModelView.m[7] =  0.0f;
    ModelView.m[8]  =  c.u.z;
    ModelView.m[9]  =  c.v.z;
    ModelView.m[10] =  c.n.z;
    ModelView.m[11] =  0.0f;
    ModelView.m[12] = -dotProduct(eye,c.u);
    ModelView.m[13] = -dotProduct(eye,c.v);
    ModelView.m[14] = -dotProduct(eye,c.n);
    ModelView.m[15] =  1.0f;
}

/**** Gera visão perspectiva na matriz  ****/
void Close2GL::C2GLperspective(float fovY, float aspectRatio, float znear, float zfar)
{
    float rad = fovY*PI/180.0f;
    float top = znear * tan(rad/2.0f);
    float bottom= -znear* tan(rad/2.0f);
    float left = -top * aspectRatio;
    float right = top* aspectRatio;
    C2GLfrustum(left, right, bottom, top, znear, zfar);
}
/** Cria matriz de projeção**/
void Close2GL::C2GLsetProjectionMatrix()
{
    Projection.m[0] = 2*c.znear/(c.right-c.left);
    Projection.m[1] = 0.0f;
    Projection.m[2] = 0.0f;
    Projection.m[3] =  0.0f;
    Projection.m[4] =  0.0f;
    Projection.m[5] =  2*c.znear/(c.top-c.bottom);
    Projection.m[6] =  0.0f;
    Projection.m[7] =  0.0f;
    Projection.m[8]  =(c.right+c.left)/(c.right-c.left);
    Projection.m[9]  =(c.top+c.bottom)/(c.top-c.bottom);
    Projection.m[10] =  -(c.zfar+c.znear)/(c.zfar-c.znear);
    Projection.m[11] =  -1.0f;
    Projection.m[12] = 0.0f;
    Projection.m[13] = 0.0f;
    Projection.m[14] =  -(2*c.zfar*c.znear)/(c.zfar-c.znear);
    Projection.m[15] =  0.0f;
}

/****** Gera espaço de projeção da camera na Projection Matrix ********/
void Close2GL::C2GLfrustum(float left, float right,float bottom, float top, float znear, float zfar)
{
    matrix4x4f project;
    c.left = left;
    c.right= right;
    c.bottom= bottom;
    c.top = top;
    c.zfar=zfar;
    c.znear=znear;

    Projection.m[0] = 2*c.znear/(c.right-c.left);
    Projection.m[1] = 0.0f;
    Projection.m[2] = 0.0f;
    Projection.m[3] =  0.0f;
    Projection.m[4] =  0.0f;
    Projection.m[5] =  2*c.znear/(c.top-c.bottom);
    Projection.m[6] =  0.0f;
    Projection.m[7] =  0.0f;
    Projection.m[8]  =(c.right+c.left)/(c.right-c.left);
    Projection.m[9]  =(c.top+c.bottom)/(c.top-c.bottom);
    Projection.m[10] =  -(c.zfar+c.znear)/(c.zfar-c.znear);
    Projection.m[11] =  -1.0f;
    Projection.m[12] = 0.0f;
    Projection.m[13] = 0.0f;
    Projection.m[14] =  -(2*c.zfar*c.znear)/(c.zfar-c.znear);
    Projection.m[15] =  0.0f;
}

/************* Constroi a Viewport Matrix *********************/
void Close2GL::C2GLOrtho2D(float lv,float rv, float bv, float tv)
{
    C2GLbuffer.defineSize(rv-lv,tv-bv);
    Viewport.m[0] = (rv-lv) / 2.0f;
    Viewport.m[1] = 0;
    Viewport.m[2] = 0;
    Viewport.m[3] = 0;
    Viewport.m[4] = 0;
    Viewport.m[5] = (tv-bv) / 2.0f;
    Viewport.m[6] = 0;
    Viewport.m[7] = 0;
    Viewport.m[8] = 0;
    Viewport.m[9] = 0;
    Viewport.m[10] =1;
    Viewport.m[11] = 0;
    Viewport.m[12] = (rv+lv) / 2.0f;
    Viewport.m[13] = (tv+bv) / 2.0f;
    Viewport.m[14] = 0;
    Viewport.m[15] = 1;
}
/*** set orientação para  a geração de poligonoes***/
void Close2GL::C2GLCullingOrientation(int orientantion)
{
    cullingMode=orientantion;
}

/** Set se tem ou não backface culling **/
void Close2GL::C2GLCull(bool cull)
{
    backfaceCulling=cull;
}

/*** Modo de renderização - pontos, grade ou sólido ***/
void Close2GL::C2GLDrawMode(int mode)
{
    format=mode;
}

void Close2GL::C2GLShadingMode(int mode)
{
    shading=mode;
}

void Close2GL::C2GLZbufferMode(bool zbuffering)
{
    C2GLbuffer.setCheckDepth(zbuffering);
}

void Close2GL::C2GLenableTex(bool enable)
{
    textEn=enable;
}
void Close2GL::Drawline( const colorVector &color0, vector3f v0, float w0,const colorVector &color1, vector3f v1, float w1)
{
    float lenghtX = v1.x - v0.x;
    float lenghtY = v1.y - v0.y;
    float depth0, depth1;
    if(lenghtX == 0.0f && lenghtY == 0.0f)
    {
        C2GLbuffer.setPixel(v0.x, v0.y, v0.z, color0);
    }
    else
    {
        float factorW;
        depth0 = min(v0.z,v1.z);
        depth1 = max(v0.z,v1.z);
        float minlen, maxlen;
        colorVector newcolor;
        if(fabs(lenghtX) > fabs(lenghtY))
        {
            minlen = min(v0.x,v1.x);
            maxlen = max(v0.x,v1.x);
            float a = lenghtY / lenghtX;
            for(float x = minlen; x <= maxlen; x += 1.0f)
            {
                float y = v0.y + ((x - v0.x) * a);
                factorW = 1/w0 + (((1/w1) - (1/w0)) * ((x - v0.x) / lenghtX));
                newcolor = (color0+((color1-color0) * ((x-v0.x)/lenghtX))) * factorW;
                float depth = (depth0 + ((depth1-depth0)*((x - v0.x) / lenghtX))) * factorW;
                C2GLbuffer.setPixel(x, y, depth, newcolor);
            }
        }
        else
        {
            minlen = min(v0.y,v1.y);
            maxlen = max(v0.y,v1.y);
            float a = lenghtX / lenghtY;
            for(float y = minlen; y <= maxlen; y += 1.0f)
            {
                float x = v0.x + ((y - v0.y) * a);
                factorW = 1/w0 + (((1/w1) - (1/w0)) * ((y - v0.y) / lenghtY));
                newcolor = (color0 + ((color1 - color0) * ((y - v0.y) / lenghtY))) * factorW;
                float depth = (depth0 + ((depth1-depth0)*((y - v0.y) / lenghtY))) * factorW;
                C2GLbuffer.setPixel(x, y, 1.0, newcolor);
            }
        }
    }
}

void Close2GL::DrawSpan(const pixelLine &line, int y)
{
    int xdiff = line.x2 - line.x1;
    if(xdiff != 0)
    {
        float depthdiff = line.depth2 - line.depth1;
        float invwpdiff = (1.0/line.w2) - (1.0/line.w1);
        float factor = 0.0f;
        float factorStep = 1.0f / (float)xdiff;
        float depthfactor = 0.0f;
        if(object->textureEnable && textEn) {
            vector3f texdiff = line.textel2 - line.textel1;
            for(int x = min(line.x1,line.x2); x < max(line.x2,line.x1); x++) {
                float factorW = (1.0/line.w1) + (invwpdiff * factor);

                vector3f tx = (line.textel1 + (texdiff * factor))*factorW ;
                //printf(" w1: %f , invwpdiff: %f factorw: %f \n txx: %f txy: %f\n x: %d y: %d \n",line.w1,invwpdiff,factorW,tx.x,tx.y,x,y);
                if(tx.x>1)
                    tx.x=1;
                if(tx.y>1)
                    tx.y=1;
                colorVector color = tex.getColor(tx.y,tx.x ,x,y);
                C2GLbuffer.setPixel(x, y, (line.depth1 + (depthdiff * factor)), color);
                factor += factorStep;
            }
        } else {
            colorVector colordiff = line.color2 - line.color1;
            for(int x = min(line.x1,line.x2); x < max(line.x2,line.x1); x++)
            {
                float factorW = (1.0/line.w1) + (invwpdiff * factor);
                colorVector color_ = (line.color1 + (colordiff * factor)) * factorW;

                C2GLbuffer.setPixel(x, y, (line.depth1 + (depthdiff * factor)), color_);
                factor += factorStep;
            }
        }
    }

}

void Close2GL::DrawSpansBetweenEdges(pixelLine e1, pixelLine e2)
{
    float e1ydiff = (float)(e1.y2 - e1.y1);
    float e2ydiff = (float)(e2.y2 - e2.y1);

    if(e1ydiff != 0.0f && e2ydiff != 0.0f)
    {
        float e1xdiff = (float)(e1.x2 - e1.x1);
        float e2xdiff = (float)(e2.x2 - e2.x1);
        float depth1diff = e1.depth2 - e1.depth1;
        float depth2diff = e2.depth2 - e2.depth1;
        float invwp1diff = e1.w2 - e1.w1;
        float invwp2diff = e2.w2 - e2.w1;
        colorVector e1colordiff = (e1.color2 - e1.color1);
        colorVector e2colordiff = (e2.color2 - e2.color1);
        vector3f e1texdiff = (e1.textel2 - e1.textel1);
        vector3f e2texdiff = (e2.textel2 - e2.textel1);
        float factor1 = (float)(e2.y1 - e1.y1) / e1ydiff;
        float factorStep1 = 1.0f / e1ydiff;
        float factor2 = 0.0f;
        float factorStep2 = 1.0f / e2ydiff;
        for(int y = e2.y1; y <e2.y2; y++)
        {
            pixelLine line = pixelLine(e1.color1 + (e1colordiff * factor1),
                                        e1.x1 + (int)(e1xdiff * factor1),0,
                                        e1.depth1 + (depth1diff * factor1),
                                        e1.w1 + (invwp1diff * factor1),
                                        e1.textel1 + (e1texdiff * factor1),
                                        e2.color1 + (e2colordiff * factor2),
                                        e2.x1+ (int)(e2xdiff * factor2),0,
                                        e2.depth1 + (depth2diff * factor2),
                                        e2.w1 + (invwp2diff * factor2),
                                        e2.textel1 + (e2texdiff * factor1));
            DrawSpan(line, y);
            factor1 += factorStep1;
            factor2 += factorStep2;
        }
    }
}

void Close2GL::DrawTriangle(const colorVector &color0, vector3f v0, float w0, vector3f tex0,
                            const colorVector &color1, vector3f v1, float w1, vector3f tex1,
                            const colorVector &color2, vector3f v2, float w2, vector3f tex2 )
{
    pixelLine edges[3] =
    {
        pixelLine(color0, (int)v0.x, (int)v0.y, v0.z, w0,tex0 ,color1, (int)v1.x, (int)v1.y, v1.z, w1,tex1),
        pixelLine(color1, (int)v1.x, (int)v1.y, v1.z, w1,tex1 ,color2, (int)v2.x, (int)v2.y, v2.z, w2,tex2),
        pixelLine(color2, (int)v2.x, (int)v2.y, v2.z, w2,tex2, color0, (int)v0.x, (int)v0.y, v0.z, w0,tex0)
    };
    int maxLength = 0;
    int longEdge = 0;
    for(int i = 0; i < 3; i++)
    {
        if(edges[i].length > maxLength)
        {
            maxLength = edges[i].length;
            longEdge = i;
        }
    }
    int shortEdge1 = (longEdge + 1) % 3;
    int shortEdge2 = (longEdge + 2) % 3;
    DrawSpansBetweenEdges(edges[longEdge], edges[shortEdge1]);
    DrawSpansBetweenEdges(edges[longEdge], edges[shortEdge2]);
}

colorVector Close2GL::phongLighintModel(float d, vector3f N)
{
    colorVector I;
    vector3f L = Light.position;
    L.normalize();
    N.normalize();
    float dotNL = dotProduct(N, L);

    vector3f R =(2 * dotNL * N)-L;
    R.normalize();

    vector3f V = c.position-c.lookat;
    V.normalize();
    float c1, c2, c3;
    c1=c2=c3=1.0f;

    //float Att = 1.0/(c1+(c2*d)+(c3*d*d));
    //if(Att > 1.0)
    float  Att = 1.0;
    float s = object->shine[0];
    if(s==0)
        s=1;
    float powVRn = pow(dotProduct(V, R), s);
    //printf("obj: %.2f %.2f\n",dotNL,powVRn);
    I.R = (Light.ambient.R) + ( Att * Light.color.R * ( (Light.diffuse.R * dotNL ) + (Light.specular.R * powVRn)) );
    I.G = (Light.ambient.G) + ( Att * Light.color.G * ( (Light.diffuse.G * dotNL ) + (Light.specular.G * powVRn)) );
    I.B = (Light.ambient.B) + ( Att * Light.color.B * ( (Light.diffuse.B * dotNL ) + (Light.specular.B * powVRn)) );
    //printf("c: %.2f %.2f %.2f\n",I.R,I.G,I.B);
    return I;
}


/** renderização de todos os polígonos **/
void Close2GL::C2GLrender(filereader *reader)
{
    int i;
    object=reader;
    C2GLbuffer.clearColor();
    C2GLbuffer.clearDepth();
    if(cullingMode == CW)
        for(i=0; i<reader->NumTris; i++)
            C2GLmapObject(reader->Tris[i]);
    else if(cullingMode==CCW)
        for(i=reader->NumTris-1; i>=0; i--)
            C2GLmapObject(reader->Tris[i]);
    glRasterPos2i(0, 0);
    glDrawPixels(C2GLbuffer.width, C2GLbuffer.height, GL_RGBA, GL_FLOAT, C2GLbuffer.colors);
}

/*** Mapeia e desenha um triangulo ****/
void Close2GL::C2GLmapObject(triangle t)
{

    matrix4x4f multMatrix = Projection*ModelView;
    float wv0,wv1,wv2;
    vector3f v0, v1, v2,t0,t1,t2,Normal, Origin;

    colorVector c0,c1,c2;
    float cosnp= -1;
    // PM*vi
    v0 = multMatrix * t.v0;
    v1 = multMatrix * t.v1;
    v2 = multMatrix * t.v2;
    Origin = c.position;
    if(backfaceCulling)
    {
        if(cullingMode==CW)
        {
            Normal = crossProduct((t.v1 - t.v0), (t.v2 - t.v0));
        }
        else if(cullingMode==CCW)
        {
            Normal = crossProduct((t.v2 - t.v0), (t.v1 - t.v0));
        }
        Normal.normalize();
        cosnp = dotProduct(Origin,Normal) /*/ (c.position.length() * Normal.length())*/;
    }
    //clipping
   // printf("w1: %f w2: %f w3: %f\n",v0.w,v1.w,v2.w);
    wv0 = 1/v0.w;
    wv1 = 1/v1.w;
    wv2 = 1/v2.w;
    c0 = color /* wv0*/;
    c1 = color /* wv1*/;
    c2 = color /* wv2*/;
    t0 = t.textel[0]*wv0;
    t1 = t.textel[1]*wv1;
    t2 = t.textel[2]*wv2;

    if(v0.insideOfCamSpace() && v1.insideOfCamSpace() && v2.insideOfCamSpace() && cosnp <= 0.0f)
    {
        v0.divW();
        v1.divW();
        v2.divW();

        if(ilumination == ENABLE && shading == FLAT)
        {
            vector3f bar = (t.v0 + t.v1 + t.v2) * 0.333333333f;
            float d0 = vdistance(Light.position, bar);
            colorVector cflat = phongLighintModel(d0, t.face_normal);
            c0 = cflat * wv0;
            c1 = cflat * wv1;
            c2 = cflat * wv2;
        }
        else if(ilumination== ENABLE && shading == GOURAUD)
        {
            float d0 = vdistance(Light.position, t.v0);
            float d1 = vdistance(Light.position, t.v1);
            float d2 = vdistance(Light.position, t.v2);

            c0 = phongLighintModel(d0, t.normal[0]) * wv0;
            c1 = phongLighintModel(d1, t.normal[1]) * wv1;
            c2 = phongLighintModel(d2, t.normal[2]) * wv2;
        }
        else
        {
            c0 = color * wv0;
            c1 = color * wv1;
            c2 = color * wv2;
        }
        //Viewport*vi
        v0 = Viewport * v0;
        v1 = Viewport * v1;
        v2 = Viewport * v2;
        if(format==POINTS)
        {
            C2GLbuffer.setPixel(v0.x, v0.y, v0.z, color);
            C2GLbuffer.setPixel(v1.x, v1.y, v1.z, color);
            C2GLbuffer.setPixel(v2.x, v2.y, v2.z, color);
        }
        else if(format==WIREFRAME)
        {
            Drawline(c0, v0, wv0, c1, v1, wv1);
            Drawline(c1, v1, wv1, c2, v2, wv2);
            Drawline(c2, v2, wv2, c0, v0, wv0);
        }
        else if(format==SOLID)
        {
            DrawTriangle(c0, v0, wv0,t0,c1, v1,wv1,t1,c2, v2,wv2,t2);
        }
    }
    else if( cosnp < 0.0f)
    {
        printf("\nV0\n");
        printVector(v0);
        printf("\nV1\n");
        printVector(v1);
        printf("\nV2\n");
        printVector(v2);
        v0.divW();
        v1.divW();
        v2.divW();

        v0 = Viewport * v0;
        v1 = Viewport * v1;
        v2 = Viewport * v2;

        printf("\nV0\n");
        printVector(v0);
        printf("\nV1\n");
        printVector(v1);
        printf("\nV2\n");
        printVector(v2);

    }
}

void Close2GL::updateLight(float * ambient, float * diffuse, float * specular,float R,float G,float B,float A ,float * position)
{
    color = colorVector(R,G,B,A);
    Light = light(colorVector(ambient[0],ambient[1],ambient[2],ambient[3]),
                  colorVector(diffuse[0],diffuse[1],diffuse[2],diffuse[3]),
                  colorVector(specular[0],specular[1],specular[2],specular[3]),
                  color,
                  vector3f(position[0],position[1],position[2]));
}

void Close2GL::read_texture_image_and_set(colorVector *textel, int w, int h) {
    tex.defineSize(w,h);
    tex.setTexture(textel);
    /*for(int i=0;i<w;i++)
        for(int j=0;j<h;j++)
            printf("R: %.3f G: %.3f B: %.3f\n",textel[j+(i*w)].R,textel[j+(i*w)].G,textel[j+(i*w)].B);*/
}

/*** debug para exibir a Matriz ***/
void Close2GL::printMatrix(matrix4x4f matrix)
{
    for(int i = 0; i<4; i++)
    {
        printf("%f \t %f \t %f \t %f \n", matrix.m[i%4], matrix.m[(i%4)+4], matrix.m[(i%4)+8], matrix.m[(i%4)+12]);
    }
    printf("\n\n");

}
/** debug para exibir vertices **/
void Close2GL::printVector(vector3f v)
{
    printf("x:%f y:%f z:%f w:%f\n",v.x,v.y,v.z,v.w);
}

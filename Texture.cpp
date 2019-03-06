#include "texture.h"

Texture::Texture()
{
    mode=NEAREST;
}

Texture::~Texture()
{
   /* if(colors!=NULL)
        free(colors);
    if(colorsgl!=NULL)
        free(colorsgl);*/
}

void Texture::setMode(int m)
{
    mode=m;
}

colorVector Texture::getColor(float s, float t,int x,int y)
{
    colorVector color=colorVector();

    int i = int(s*(width-1)) + (int(t*(height-1)) *width);
    if(mode == NEAREST){
        color = colors[i];
    } else  if(mode == BILINEAR){
        color=getBilinearColor(s,t,5);
    } else if(mode == MIPMAP) {
        color= getMipMapTex(x,y,s,t);
    }
    return color;
}

colorVector Texture::getBilinearColor(float s, float t,int level){
    int i1,i2,i3,i4;
    colorVector color=colorVector();
    i1=int(truncf(s*(width-1))) + (int(t*(height-1)) *width);
    if(roundf(s*(width-1))<width)
        i2=int(roundf(s*(width-1))) + (int(t*(height-1)) *width);
    else
        i2=i1;
    i3=int(s*(width-1)) + (int(truncf(t*(height-1))) *width);
    if(roundf(t*(height-1))<height)
        i4=int(s*(width-1)) + (int(roundf(t*(height-1))) *width);
    else
        i4=i3;
    color= (colors[i1]+colors[i2]+colors[i3]+colors[i4])/4;
    return color;
}

void Texture::setColor(colorVector p, int s, int t)
{
    int i = s+t*width;
    colors[i].R = p.R;
    colors[i].G = p.G;
    colors[i].B = p.B;
    colors[i].A = p.A;
}

void Texture::defineSize(float s, float t){
    width =s;
    height =t;
    colors = (colorVector *)malloc(width * height * sizeof(colorVector));
    colorsgl = (float *)malloc(width * height * 3 * sizeof(float *));
}

void Texture::setColor(int x, int y, float R,float G,float B)
{
    int i = x+y*width;
    colors[i].R = R;
    colors[i].G = G;
    colors[i].B = B;
    colors[i].A = 1.0f;
}

void Texture::setTexture(colorVector* p)
{
    colors= p;
}

void Texture::buildPyramid(){
    int pyramidSize = int(log2(width));
    for(int i=0; i <= pyramidSize; i++){
        buildPyramidLevel(i);
    }
}

void Texture::buildPyramidLevel(int level)
{
    Texture tex;
    if(level==0){
        tex=*this;
    } else {
        tex.defineSize(children[level-1].width/2,children[level-1].height/2);
        int position;
        int width = children[level-1].width;
        colorVector color, color1, color2, color3, color4;
        for(int j= 0; j < tex.height; j++){
            for(int i = 0 ; i< tex.width ; i++){
                color1 = children[level-1].colors[(i+ j*width)];
                color2= children[level-1].colors[((i+1)+ j*width)];
                color3= children[level-1].colors[(i+ (j+1)*width)];
                color4= children[level-1].colors[((i+1)+(j+1)*width)];
                color = (color1 + color2 + color3 + color4) /4;
                position = (j * tex.width + i);
                tex.colors[position] = color;
            }
        }
    }
	children.push_back(tex);
}

colorVector Texture::getMipMapTex(int x, int y, float s, float t)
{
    colorVector color=colorVector();
	vector3f diffX, diffY, pos, posH, posV;
	float xVal, yVal, maxVal;
	float index;
	int pyramidIndex;
	colorVector color1, color2;

	pos = vector3f(x, y,1);
	if(x!=0)
        posH = vector3f(x-1, y,1);
	else
        posH.set(0.0, 0.0,1.0);

	if(y!= (height-1))
        posV = vector3f(x, y-1,1);
	else
        posV.set(0.0, 0.0, 1.0);
	diffX = pos - posH;
	diffX = diffX * diffX;
	xVal = diffX.x + diffX.y;
	xVal = sqrt(xVal);

	diffY = pos - posV;
	diffY = diffY * diffY;
	yVal = diffY.x + diffY.y;
	yVal = sqrt(yVal);
	maxVal = max(xVal , yVal);
	maxVal *= width;
	if(maxVal <1)
        maxVal =1;

	index = (log2(maxVal));
	pyramidIndex = (int)index;
	color1 = getBilinearColor( s, t, pyramidIndex);
    color2 = getBilinearColor(s, t, pyramidIndex +1);
	float ind = index - pyramidIndex;
	//printf("%f %f %d\n",ind,index,pyramidIndex);
	color = ( color1* (1 - ind)) + color2 * ind;
	return color;
}

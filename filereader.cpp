#include "filereader.h"
#include "vector3f.h"
#include "triangle.h"


filereader::filereader()
{
    maxX=FLT_MIN;
    minX=FLT_MAX;
    maxY=FLT_MIN;
    minY=FLT_MAX;
    maxZ=FLT_MIN;
    minZ=FLT_MAX;
}

void filereader::readfile(const char *FileName)
{
    int material_count, color_index[NUMBER_OF_EDGES], i;
    char ch,tex;

    FILE* fp = fopen(FileName,"r");
    if (fp==NULL) {
          printf("ERROR: unable to open TriObj [%s]!\n",FileName);
          fileRead=false;
          return;
    }
    maxX=FLT_MIN;
    minX=FLT_MAX;
    maxY=FLT_MIN;
    minY=FLT_MAX;
    maxZ=FLT_MIN;
    minZ=FLT_MAX;
    fscanf(fp, "%c", &ch);
    while(ch!= '\n') // skip the first line – object’s name
        fscanf(fp, "%c", &ch);
    fscanf(fp,"# triangles = %d\n", &NumTris); // read # of triangles
    fscanf(fp,"Material count = %d\n", &material_count); // read material count
    for (i=0; i<material_count; i++) {
        fscanf(fp, "ambient color %f %f %f\n", &(ambient[i].x), &(ambient[i].y), &(ambient[i].z));
        fscanf(fp, "diffuse color %f %f %f\n", &(diffuse[i].x), &(diffuse[i].y), &(diffuse[i].z));
        fscanf(fp, "specular color %f %f %f\n", &(specular[i].x), &(specular[i].y), &(specular[i].z));
        fscanf(fp, "material shine %f\n", &(shine[i]));
        fscanf(fp, "Texture = %c", &tex);
        textureEnable = tex=='Y';
    }

     fscanf(fp, "%c", &ch);
    while(ch!= '\n') // skip documentation line
        fscanf(fp, "%c", &ch);
    //
    fscanf(fp, "%c", &ch);
    while(ch!= '\n') // skip documentation line
        fscanf(fp, "%c", &ch);
    // allocate triangles for tri model
    printf ("Reading in %s (%d triangles). . .\n", FileName, NumTris);
    printf ("Texture %d\n",textureEnable);
    Tris.reserve(NumTris);
    for (i=0; i<NumTris; i++){ // read triangles
        Tris[i]=triangle();
        if(textureEnable)
            fscanf(fp, "v0 %f %f %f %f %f %f %d %f %f\n", &(Tris[i].v0.x), &(Tris[i].v0.y), &(Tris[i].v0.z),
            &(Tris[i].normal[0].x), &(Tris[i].normal[0].y), &(Tris[i]. normal [0].z),&(color_index[0]),
            &(Tris[i].textel[0].x), &(Tris[i].textel[0].y));
        else
            fscanf(fp, "v0 %f %f %f %f %f %f %d\n", &(Tris[i].v0.x), &(Tris[i].v0.y), &(Tris[i].v0.z),
            &(Tris[i].normal[0].x), &(Tris[i].normal[0].y), &(Tris[i]. normal [0].z),&(color_index[0]));
        if(Tris[i].v0.x<minX)
            minX=Tris[i].v0.x;
        if(Tris[i].v0.x>maxX)
            maxX=Tris[i].v0.x;
        if(Tris[i].v0.y<minY)
            minY=Tris[i].v0.y;
        if(Tris[i].v0.y>maxY)
            maxY=Tris[i].v0.y;
        if(Tris[i].v0.z<minZ)
            minZ=Tris[i].v0.z;
        if(Tris[i].v0.z>maxZ)
            maxZ=Tris[i].v0.z;
        if(textureEnable)
            fscanf(fp, "v1 %f %f %f %f %f %f %d %f %f\n", &(Tris[i].v1.x), &(Tris[i].v1.y), &(Tris[i].v1.z),
            &(Tris[i].normal[1].x), &(Tris[i].normal[1].y), &(Tris[i].normal[1].z),&(color_index[1]),
            &(Tris[i].textel[1].x), &(Tris[i].textel[1].y));
        else
            fscanf(fp, "v1 %f %f %f %f %f %f %d\n", &(Tris[i].v1.x), &(Tris[i].v1.y), &(Tris[i].v1.z),
            &(Tris[i].normal[1].x), &(Tris[i].normal[1].y), &(Tris[i].normal[1].z),&(color_index[1]));
        if(Tris[i].v1.x<minX)
            minX=Tris[i].v1.x;
        if(Tris[i].v1.x>maxX)
            maxX=Tris[i].v1.x;
        if(Tris[i].v1.y<minY)
            minY=Tris[i].v1.y;
        if(Tris[i].v1.y>maxY)
            maxY=Tris[i].v1.y;
        if(Tris[i].v1.z<minZ)
            minZ=Tris[i].v1.z;
        if(Tris[i].v1.z>maxZ)
            maxZ=Tris[i].v1.z;
        if(textureEnable)
            fscanf(fp, "v2 %f %f %f %f %f %f %d %f %f\n",&(Tris[i].v2.x), &(Tris[i].v2.y), &(Tris[i].v2.z),
            &(Tris[i].normal[2].x), &(Tris[i].normal[2].y), &(Tris[i].normal[2].z), &(color_index[2]),
            &(Tris[i].textel[2].x), &(Tris[i].textel[2].y));
        else
            fscanf(fp, "v2 %f %f %f %f %f %f %d\n",&(Tris[i].v2.x), &(Tris[i].v2.y), &(Tris[i].v2.z),
            &(Tris[i].normal[2].x), &(Tris[i].normal[2].y), &(Tris[i].normal[2].z), &(color_index[2]));
        if(Tris[i].v2.x<minX)
            minX=Tris[i].v2.x;
        if(Tris[i].v2.x>maxX)
            maxX=Tris[i].v2.x;
        if(Tris[i].v2.y<minY)
            minY=Tris[i].v2.y;
        if(Tris[i].v2.y>maxY)
            maxY=Tris[i].v2.y;
        if(Tris[i].v2.z<minZ)
            minZ=Tris[i].v2.z;
        if(Tris[i].v2.z>maxZ)
            maxZ=Tris[i].v2.z;
        fscanf(fp, "face normal %f %f %f\n", &(Tris[i].face_normal.x), &(Tris[i].face_normal.y),&(Tris[i].face_normal.z));
    }
    fclose(fp);
    fileRead=true;
}

void filereader::read_texture_image_and_set(float *pixels, int w, int h)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//
	//    setting mip map pyramid
	//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, pixels);

	gluBuild2DMipmaps(GL_TEXTURE_2D,  GL_RGBA, w, h, GL_RGBA, GL_FLOAT, pixels);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


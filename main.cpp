

#include <string.h>
#include <GL/glui.h>
#include <math.h>
#include <jpeglib.h>
#include <jerror.h>
#include <jconfig.h>
#include <jmorecfg.h>
#include "filereader.h"
#include "close2gl.h"
#include "Texture.h"

#define ORIGIN_X 0.0f
#define ORIGIN_Y 0.0f
#define ORIGIN_Z 0.0f
#define ORIGIN_LOOK_X 1
#define ORIGIN_LOOK_Y 1
#define ORIGIN_LOOK_Z -1
#define ORIGIN_ZNEAR 0.1f
#define ORIGIN_ZFAR 3000.0f
#define VFOV 600.0f
#define HFOV 600.0f
#define PI 3.1415926535897

using namespace std;

float xy_aspect;
int   last_x, last_y;
filereader reader;

/** These are the live variables passed into GLUI ***/
float angle=0.0f;
int   light0_enabled = 1;
int   light1_enabled = 1;
float light0_intensity = 1.0f;
float light1_intensity = .4;
int   main_window,windows2CGL;
float r;
float   eyex = ORIGIN_X;
float   eyey = ORIGIN_Y;
float   eyez = ORIGIN_Z;
float   lookx= 0.0f;
float   looky= 0.0f;
float   lookz= -1.0f;
float   znear= ORIGIN_ZNEAR;
float   zfar= ORIGIN_ZFAR;
float   ux=0;
float   uy=1;
float   uz=0;
float sphere_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float torus_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };
int fixedlook=1;
int num_format  = 0;
float anglez=0.0f;
float anglex=0.0f;
float angley=0.0f;
float width;
float height;
float origineyex;
float origineyey;
float origineyez;
int backface=1;
int shading=1;
int zbuffer=1;
int texEn=1;
GLfloat ambient[4];
GLfloat diffuse[4];
GLfloat specular[4];
GLfloat colorlight[4] ={0.5,0.5,0.5,0.5};
Close2GL close2gl = Close2GL();
Texture tx;


/** Pointers to the windows and some of the controls we'll create **/
GLUI *glui;
GLUI_Spinner    *light0_spinner;
GLUI_RadioGroup *radio;
GLUI_Panel      *obj_panel;
GLUI_Spinner *znear_spinner;
GLUI_Spinner *zfar_spinner;
GLUI_Spinner *rotx_spinner;
GLUI_Spinner *roty_spinner;
GLUI_Spinner *rotz_spinner;
GLUI_Spinner *eyex_spinner;
GLUI_Spinner *eyey_spinner;
GLUI_Spinner *eyez_spinner;
GLUI_RadioButton *radio_points;
GLUI_RadioButton *radio_lines;
GLUI_RadioButton *radio_solid;
GLUI_FileBrowser *fb;
GLUI_FileBrowser *jb;
GLUI_RadioButton *radio_cw;
GLUI_RadioButton *radio_ccw;
GLUI_RadioGroup * culling ;
GLUI_Checkbox *backfaceculling;
GLUI_Checkbox *shadingcheckbox;
GLUI_Checkbox *zbuffercheckbox;
GLUI_Spinner *width_spinner;
GLUI_Spinner *height_spinner;
GLUI_RadioButton *radio_gouraud;
GLUI_RadioButton *radio_flat;
GLUI_RadioButton *radio_nearestNeighbor;
GLUI_RadioButton *radio_bilinear;
GLUI_RadioButton *radio_mipmap;
GLUI_Checkbox *texturecheckbox;

/********** User IDs for callbacks ********/
#define LIGHT0_INTENSITY_ID  250
#define SET_ORIGIN           304
#define READ_FILE            308
#define READ_TEXTURE         312

void setTexture();
void read_texture_image_and_set();
/********** Miscellaneous global variables **********/
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};

GLfloat lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

/************ inicia variavies da camera *****************/
void init_lookat_vars()
{
    r = sqrt((reader.maxX - (reader.maxX + reader.minX)/2)*
                 (reader.maxX - (reader.maxX + reader.minX)/2)
                 + (reader.maxY - (reader.maxY + reader.minY)/2)*
                 (reader.maxY - (reader.maxY + reader.minY)/2));
        lookx = 0.0f;
        looky = 0.0f;
        lookz = -(4*(r/atan(30.0f)));
        eyex = lookx;
        eyey = looky;
        eyez = 0.0f; //(4*(r/atan(30.0f)));
        origineyex = eyex;
        origineyey = eyey;
        origineyez = eyez;
}

/******** control_cb() - callbacks *******************/
void control_cb( int control )
{
    if ( control == LIGHT0_INTENSITY_ID )
    {
        float v[] =
        {
            diffuse[0],  diffuse[1],
            diffuse[2],  diffuse[3]
        };

        v[0] *= light0_intensity;
        v[1] *= light0_intensity;
        v[2] *= light0_intensity;

        glLightfv(GL_LIGHT0, GL_DIFFUSE, v );

        v[0] = ambient[0] * light0_intensity;
        v[1] = ambient[1] * light0_intensity;
        v[2] = ambient[2] * light0_intensity;

        glLightfv(GL_LIGHT0, GL_AMBIENT, v );

        v[0] = specular[0] * light0_intensity;
        v[1] = specular[1] * light0_intensity;
        v[2] = specular[2] * light0_intensity;

        glLightfv(GL_LIGHT0, GL_SPECULAR, v );


    }
    else if( control == SET_ORIGIN)
    {
        if(reader.fileRead)
        {
            init_lookat_vars();
            eyex_spinner->set_float_val(eyex);
            eyey_spinner->set_float_val(eyey);
            eyez_spinner->set_float_val(eyez);

        }
        else
        {
            eyex_spinner->set_float_val(ORIGIN_X);
            eyey_spinner->set_float_val(ORIGIN_Y);
            eyez_spinner->set_float_val(ORIGIN_Z);
        }
        rotx_spinner->set_float_val(0.0f);
        roty_spinner->set_float_val(0.0f);
        rotz_spinner->set_float_val(0.0f);
        zfar_spinner->set_float_val(ORIGIN_ZFAR);
        znear_spinner->set_float_val(ORIGIN_ZNEAR);

    }
    else if( control == READ_FILE)
    {
        string filename=fb->get_file();
        reader.readfile(filename.c_str());
        if(reader.fileRead)
        {
            init_lookat_vars();
            eyex_spinner->set_float_val(eyex);
            eyey_spinner->set_float_val(eyey);
            eyez_spinner->set_float_val(eyez);
            rotx_spinner->set_float_val(0.0f);
            roty_spinner->set_float_val(0.0f);
            rotz_spinner->set_float_val(0.0f);
            zfar_spinner->set_float_val(ORIGIN_ZNEAR);
            znear_spinner->set_float_val(ORIGIN_ZFAR);

            /****************************************/
            /*       Set up OpenGL lights           */
            /****************************************/
            ambient[0] = reader.ambient[0].x;
            ambient[1] = reader.ambient[0].y;
            ambient[2] = reader.ambient[0].z;
            ambient[3]=1.0f;
            diffuse[0] = reader.diffuse[0].x;
            diffuse[1] = reader.diffuse[0].y;
            diffuse[2] = reader.diffuse[0].z;
            diffuse[3]=1.0f;
            specular[0] = reader.specular[0].x;
            specular[1] = reader.specular[0].y;
            specular[2] = reader.specular[0].z;
            specular[3]=1.0f;
            glEnable(GL_LIGHTING);
            glEnable( GL_NORMALIZE );

            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
            glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
            glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

        }
    }else if( control == READ_TEXTURE ){
        setTexture();
        if(reader.textureEnable){
            glDisable(GL_COLOR_MATERIAL);
            glEnable(GL_COLOR_MATERIAL);
            read_texture_image_and_set();
            close2gl.read_texture_image_and_set(tx.colors,tx.width,tx.height);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        }
    }
}

/****************************************************/
void set_glui_gui()
{
    printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );
    /*** Create the side subwindow ***/
    glui = GLUI_Master.create_glui( "Options",0,0,0 );

    new GLUI_StaticText( glui, "Format" );
    GLUI_RadioGroup * Render = new GLUI_RadioGroup(glui,&num_format);
    radio_solid= new GLUI_RadioButton(Render,"Solid");
    radio_lines= new GLUI_RadioButton(Render,"Lines");
    radio_points= new GLUI_RadioButton(Render,"Points");
    printf("%d\n",num_format);
    /*** Disable/Enable buttons ***/
    new GLUI_Button( glui, "RESET Camera position", SET_ORIGIN, control_cb );
    backfaceculling = new GLUI_Checkbox(glui,"Backface Culling",&backface);
    GLUI_RadioGroup * culling = new GLUI_RadioGroup(glui,&num_format);
    radio_ccw= new GLUI_RadioButton(culling,"Counter clockwise");
    radio_cw= new GLUI_RadioButton(culling,"Clockwise");
    new GLUI_StaticText( glui, "" );
    zbuffercheckbox=new GLUI_Checkbox(glui,"Z-buffer",&zbuffer);
    /**** Cam translation****/
    new GLUI_StaticText( glui, "" );
    eyex_spinner = new GLUI_Spinner( glui, "Eye x:", &eyex);
    eyex_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    eyex_spinner->set_speed(1.0f);
    eyey_spinner = new GLUI_Spinner( glui, "Eye y:", &eyey);
    eyex_spinner->set_speed(1.0f);
    eyey_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    eyez_spinner = new GLUI_Spinner( glui, "Eye z:", &eyez);
    eyex_spinner->set_speed(1.0f);
    eyez_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    /********* Radio para rotação *******/
    new GLUI_StaticText( glui, "" );
    rotx_spinner =new GLUI_Spinner( glui, "Rotate in X:", &anglex);
    rotx_spinner->set_speed(1.0f);
    rotx_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    roty_spinner = new GLUI_Spinner( glui, "Rotate in Y:", &angley);
    roty_spinner->set_speed(1.0f);
    roty_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    rotz_spinner = new GLUI_Spinner( glui, "Rotate in Z:", &anglez);
    rotz_spinner->set_speed(1.0f);
    rotz_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    GLUI_Checkbox * lookfix= new GLUI_Checkbox( glui, "Fixed look", &fixedlook );
    /**** Znear e Zfar**/
    new GLUI_StaticText( glui, "" );
    znear_spinner = new GLUI_Spinner( glui, "Znear:", &znear);
    znear_spinner->set_float_limits( 0.0f, zfar );
    znear_spinner->set_speed(1.0f);
    znear_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    zfar_spinner = new GLUI_Spinner( glui, "Zfar:", &zfar);
    zfar_spinner->set_float_limits( znear, 10000.0f );
    zfar_spinner->set_speed(1.0f);
    zfar_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    width_spinner = new GLUI_Spinner( glui, "Width:", &width);
    width_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    height_spinner = new GLUI_Spinner( glui, "Height:", &height);
    height_spinner->set_alignment( GLUI_ALIGN_RIGHT );
    new GLUI_StaticText( glui, "" );

        /******** Add some controls for lights ********/
    glui->add_column(true);
    GLUI_Panel *light0 = new GLUI_Panel( glui, "Light" );
    shadingcheckbox = new GLUI_Checkbox(light0,"Shading",&shading);
    GLUI_RadioGroup * shad = new GLUI_RadioGroup(light0,&num_format);
    radio_flat= new GLUI_RadioButton(shad,"Flat");
    radio_gouraud= new GLUI_RadioButton(shad,"Gourand");
    light0_spinner = new GLUI_Spinner( light0, "Intensity:",&light0_intensity, LIGHT0_INTENSITY_ID,control_cb );
    light0_spinner->set_float_limits( 0.0, 1.0 );
    GLUI_Scrollbar *sc;
    new GLUI_StaticText( light0, "\nColor\n" );
    sc = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,&colorlight[0],LIGHT0_INTENSITY_ID,control_cb);
    sc->set_float_limits(0,1);
    sc = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,&colorlight[1],LIGHT0_INTENSITY_ID,control_cb);
    sc->set_float_limits(0,1);
    sc = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,&colorlight[2],LIGHT0_INTENSITY_ID,control_cb);
    sc->set_float_limits(0,1);
    new GLUI_StaticText( light0, "\nDiffuse\n" );
    GLUI_Scrollbar *sb;
    sb = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,&diffuse[0],LIGHT0_INTENSITY_ID,control_cb);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,&diffuse[1],LIGHT0_INTENSITY_ID,control_cb);
    sb->set_float_limits(0,1);
    sb = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,&diffuse[2],LIGHT0_INTENSITY_ID,control_cb);
    sb->set_float_limits(0,1);
    new GLUI_StaticText( light0, "\nAmbient\n" );
    GLUI_Scrollbar *sa;
    sa = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,&ambient[0],LIGHT0_INTENSITY_ID,control_cb);
    sa->set_float_limits(0,1);
    sa = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,&ambient[1],LIGHT0_INTENSITY_ID,control_cb);
    sa->set_float_limits(0,1);
    sa = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,&ambient[2],LIGHT0_INTENSITY_ID,control_cb);
    sa->set_float_limits(0,1);
    new GLUI_StaticText( light0, "\nSpecular\n" );
    GLUI_Scrollbar *ss;
    ss = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,&specular[0],LIGHT0_INTENSITY_ID,control_cb);
    ss->set_float_limits(0,1);
    ss = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,&specular[1],LIGHT0_INTENSITY_ID,control_cb);
    ss->set_float_limits(0,1);
    ss = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,&specular[2],LIGHT0_INTENSITY_ID,control_cb);
    ss->set_float_limits(0,1);

    /****FILE CHOOSER*****/
    GLUI *edit = GLUI_Master.create_glui("File browser",0,345,460);
    GLUI_Panel *ep = new GLUI_Panel(edit,"",true);
    fb = new GLUI_FileBrowser(ep, "", false, READ_FILE, control_cb);
    fb->set_h(180);
    fb->set_w(200);
    /****TEXTURE CHOOSER*****/
    GLUI *textchooser = GLUI_Master.create_glui("Texture browser",0,620,460);
    GLUI_Panel *jp = new GLUI_Panel(textchooser,"",true);
    jb = new GLUI_FileBrowser(jp, "", false, READ_TEXTURE, control_cb);
    jb->set_h(180);
    jb->set_w(200);

    /****** A 'quit' button *****/
    new GLUI_StaticText( glui, "" );
    texturecheckbox = new GLUI_Checkbox(glui,"Texture",&texEn);
    GLUI_RadioGroup * textureMode = new GLUI_RadioGroup(glui,&num_format);
    radio_nearestNeighbor= new GLUI_RadioButton(textureMode,"Nearest Neighbor");
    radio_bilinear= new GLUI_RadioButton(textureMode,"Bilinear");
    radio_mipmap= new GLUI_RadioButton(textureMode,"Mipmap");
    new GLUI_StaticText( glui, "" );
    new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );

    /**** Link windows to GLUI, and register idle callback ******/
    glui->set_main_gfx_window( main_window );
}

void read_texture_image_and_set()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &tx.id);
	glBindTexture(GL_TEXTURE_2D, tx.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//
	//    setting mip map pyramid
	//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );        // Linear Filtering
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );        // Linear Filtering

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tx.width, tx.height, 0, GL_RGB, GL_FLOAT, tx.colorsgl);
	gluBuild2DMipmaps(GL_TEXTURE_2D,  GL_RGB, tx.width, tx.height, GL_RGB, GL_FLOAT, tx.colorsgl);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//printf("mas se me preguntar eu não uso nenhum... %d\n",tx.id);
}


void DecodeImage(jpeg_decompress_struct &cinfo)
{
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	if(tx.width != cinfo.image_width && tx.height != cinfo.image_height)
        tx.defineSize(cinfo.image_width,cinfo.image_height);
    int i,j;
	JSAMPROW buffer[1];
    buffer[0] = (unsigned char*)malloc( cinfo.output_width * cinfo.output_components);
    j=cinfo.image_height-1;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);
        for( i=0; i<cinfo.image_width*cinfo.num_components&& j>=0;i++){
            tx.colorsgl[i+(j*cinfo.image_width*cinfo.num_components)] = (float)buffer[0][i]/255;
            if(i%3==2){
                colorVector color= colorVector(
                (float)buffer[0][i-2]/255,(float)buffer[0][i-1]/255,(float)buffer[0][i]/255,1.0f);
                tx.setColor(color,j,i/3);
            }
        }
        j--;
	}
	jpeg_finish_decompress(&cinfo);
	tx.buildPyramid();
}


void LoadImage(const char *filename)
{
	Texture *pImageData = NULL;
	FILE *pFile;
    if((pFile = fopen(filename, "rb")) == NULL)
	{
		printf("Impossível carregar arquivo JPG: %s\n",filename);
		return;
	}
	jpeg_error_mgr jerr;
	jpeg_decompress_struct cinfo;
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pFile);
	//pImageData = (Texture*)malloc(sizeof(Texture));
	DecodeImage(cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(pFile);
	//return pImageData;
}

void setTexture()
{
	string filename=jb->get_file();
	LoadImage(filename.c_str());
}
/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
    case 27:
    case 'q':
        exit(0);
        break;
    };

    glutPostRedisplay();
}


/**** myGlutMenu() ***/
void myGlutMenu( int value )
{
    myGlutKeyboard( value, 0, 0 );
}

/**** Sincroniza as duas janelas ****/
void myGlutIdle( void )
{
    glutSetWindow(main_window);
    glutPostRedisplay();
    glutSetWindow(windows2CGL);
    glutPostRedisplay();
}

/********* myGlutMouse() **********/
void myGlutMouse(int button, int button_state, int x, int y )
{
}


/*********************** myGlutMotion() **********/
void myGlutMotion(int x, int y )
{
    glutPostRedisplay();
}


/**************  Reshape in close2gl ***********************/
void myClose2GLReshape(int w, int h)
{
    if (h == 0) h = 1;
    glutSetWindow(windows2CGL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, w, 0.0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    close2gl.C2GLOrtho2D(0, w, 0, h);
}


/****** Reshape opengl *****/
void myGlutReshape( int x, int y )
{
    int tx, ty, tw, th;
    GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );

    glViewport( tx, ty, tw, th );

    xy_aspect = (float)tw / (float)th;
    width=tw;
    height=th;

    glutPostRedisplay();
}

/********* Display Close2GL ***************/
void myClose2GLdisplay(void)
{
    glutSetWindow(windows2CGL);
    close2gl.C2GLperspective(60.0f,width/height,znear,zfar);
    glClearColor( .0f, .0f, .0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // initialize ModelView Matrix as the Identity Matrix
    close2gl.updateLight(ambient,diffuse,specular,colorlight[0],colorlight[1],colorlight[2],1.0f,light0_position);
    if(fixedlook)
        close2gl.C2GLookat(eyex,eyey,eyez,lookx,looky,lookz,ux,uy,uz);
    else
        close2gl.C2GLookat(eyex,eyey,eyez,lookx+(eyex-origineyex),(eyey-origineyey)+looky,-(eyez-origineyez)+lookz,ux,uy,uz);

    close2gl.C2GLZbufferMode(zbuffer);
    if(radio_points->int_val)
    {
        close2gl.C2GLDrawMode(POINTS);
    }
    else if(radio_lines->int_val)
    {
        close2gl.C2GLDrawMode(WIREFRAME);
    }
    else if(radio_solid->int_val)
    {
        close2gl.C2GLDrawMode(SOLID);
    }

    if(backface)
    {
        close2gl.C2GLCull(ENABLE);
        if(radio_ccw->int_val && backface)
        {
            close2gl.C2GLCullingOrientation(CCW);
        }
        else if(radio_cw->int_val && backface)
        {
            close2gl.C2GLCullingOrientation(CW);
        }
    }
    else
    {
        close2gl.C2GLCull(DISABLE);
        close2gl.C2GLCullingOrientation(CCW);
    }
    close2gl.ilumination=shading;
    if(close2gl.ilumination){
        radio_flat->enable();
        radio_gouraud->enable();
        if(radio_flat->int_val)
            close2gl.C2GLShadingMode(FLAT);
        else if(radio_gouraud->int_val)
            close2gl.C2GLShadingMode(GOURAUD);
    }
    else{
        radio_flat->disable();
        radio_gouraud->disable();
    }
    if(radio_nearestNeighbor->int_val){
        close2gl.tex.setMode(NEAREST);
    } else if(radio_bilinear->int_val){
        close2gl.tex.setMode(BILINEAR);
    } else if(radio_mipmap->int_val){
        close2gl.tex.setMode(MIPMAP);
    }

    close2gl.Close2GLtranslatef(-(reader.maxX + reader.minX)/2, -(reader.maxY + reader.minY)/2, -(4*(r/atan(30.0f))));
    close2gl.C2GLrotatef(anglex,1.0f,0.0f,0.0f);
    close2gl.C2GLrotatef(angley,0.0f,1.0f,0.0f);
    close2gl.C2GLrotatef(anglez, 0.0f, 0.0f, -1.0f );
    close2gl.C2GLenableTex(texEn);
    close2gl.C2GLrender(&reader);
    glutSwapBuffers();
}


/***************************************** myGlutDisplay() *****************/
void myGlutDisplay( void )
{
    glClearColor( .0f, .0f, .0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60.0f,width/height,znear,zfar);
    if(fixedlook)
        gluLookAt(eyex,eyey,eyez,lookx,looky,lookz,ux,uy,uz);
    else
        gluLookAt(eyex,eyey,-eyez,lookx+(eyex-origineyex),(eyey-origineyey)+looky,-(eyez-origineyez)+lookz,ux,uy,uz);
    glRotated(anglex,1,0,0);
    glRotated(angley,0,1,0);
    glRotated(anglez, 0, 0, -1 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glMultMatrixf( lights_rotation );
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLoadIdentity();
    glMultMatrixf( view_rotate );
    if(backface)
    {
        radio_ccw->enable();
        radio_cw->enable();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if(radio_ccw->int_val && backface)
            glFrontFace(GL_CCW);
        else if(radio_cw->int_val && backface)
            glFrontFace(GL_CW);
    }
    else
    {
        //glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        //glDisable(GL_CULL_FACE);
        radio_ccw->disable();
        radio_cw->disable();
    }
    glPushMatrix();
    glTranslated(-(reader.maxX + reader.minX)/2, -(reader.maxY + reader.minY)/2,-(4*(r/atan(30.0f))) ); // Brings object to origin
    glColor3f(colorlight[0] , colorlight[1], colorlight[2] );
    if(zbuffer)
    {
        glDisable(GL_DEPTH_TEST);
    } else {
        glEnable(GL_DEPTH_TEST);
    }

    if(radio_points->int_val)
    {
        for (int i=0; i<reader.NumTris; i++)
        {

            glBegin(GL_POINTS);
            glVertex3f(reader.Tris[i].v0.x, reader.Tris[i].v0.y, reader.Tris[i].v0.z);
            glVertex3f(reader.Tris[i].v1.x, reader.Tris[i].v1.y, reader.Tris[i].v1.z);
            glVertex3f(reader.Tris[i].v2.x, reader.Tris[i].v2.y, reader.Tris[i].v2.z);
            glNormal3f(reader.Tris[i].face_normal.x,reader.Tris[i].face_normal.y,reader.Tris[i].face_normal.z);
            glEnd();
        }
    }
    else if(radio_lines->int_val)
    {

        for (int i=0; i<reader.NumTris; i++)
        {
            glNormal3f(reader.Tris[i].face_normal.x,reader.Tris[i].face_normal.y,reader.Tris[i].face_normal.z);
            glBegin(GL_LINES);
            glVertex3f(reader.Tris[i].v0.x, reader.Tris[i].v0.y, reader.Tris[i].v0.z);
            glVertex3f(reader.Tris[i].v1.x, reader.Tris[i].v1.y, reader.Tris[i].v1.z);
            glVertex3f(reader.Tris[i].v0.x, reader.Tris[i].v0.y, reader.Tris[i].v0.z);
            glVertex3f(reader.Tris[i].v2.x, reader.Tris[i].v2.y, reader.Tris[i].v2.z);
            glVertex3f(reader.Tris[i].v1.x, reader.Tris[i].v1.y, reader.Tris[i].v1.z);
            glVertex3f(reader.Tris[i].v2.x, reader.Tris[i].v2.y, reader.Tris[i].v2.z);
            glEnd();
        }
    }
    else if(radio_solid)
    {
        for (int i=0; i<reader.NumTris; i++)
        {
            if(reader.textureEnable && texEn) {
                if(radio_nearestNeighbor->int_val){
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                } else if( radio_bilinear->int_val){
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                } else if( radio_mipmap->int_val){
                   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                }
                glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            }else {
                glDisable(GL_TEXTURE_2D);
            }
            if(shading)
            {
                    glNormal3f(reader.Tris[i].face_normal.x,reader.Tris[i].face_normal.y,reader.Tris[i].face_normal.z);
                    glShadeModel(GL_FLAT);
                    glBegin(GL_TRIANGLES);
                    glTexCoord2f(reader.Tris[i].textel[0].x,reader.Tris[i].textel[0].y);
                    glVertex3f(reader.Tris[i].v0.x, reader.Tris[i].v0.y, reader.Tris[i].v0.z);

                    glTexCoord2f(reader.Tris[i].textel[1].x,reader.Tris[i].textel[1].y);
                    //glNormal3f(reader.Tris[i].normal[0].x, reader.Tris[i].normal[0].y, reader.Tris[i].normal[0].z);
                    glVertex3f(reader.Tris[i].v1.x, reader.Tris[i].v1.y, reader.Tris[i].v1.z);
                    glTexCoord2f(reader.Tris[i].textel[2].x,reader.Tris[i].textel[2].y);
                    //glNormal3f(reader.Tris[i].normal[1].x, reader.Tris[i].normal[1].y, reader.Tris[i].normal[1].z);
                    glVertex3f(reader.Tris[i].v2.x, reader.Tris[i].v2.y, reader.Tris[i].v2.z);
                   // glNormal3f(reader.Tris[i].normal[2].x, reader.Tris[i].normal[2].y, reader.Tris[i].normal[2].z);
                    glEnd();
            }
            else
            {
                glBegin(GL_TRIANGLES);
                glVertex3f(reader.Tris[i].v0.x, reader.Tris[i].v0.y, reader.Tris[i].v0.z);
                glVertex3f(reader.Tris[i].v1.x, reader.Tris[i].v1.y, reader.Tris[i].v1.z);
                glVertex3f(reader.Tris[i].v2.x, reader.Tris[i].v2.y, reader.Tris[i].v2.z);
                glEnd();
            }
        }
    }
    glPopMatrix();
    glEnable( GL_LIGHTING );
    glPopMatrix();
    if(backface)
    {
        //glDisable(GL_CULL_FACE);
        glPopMatrix();
    }
    /*** Render the live character array 'text' ***/
    glutSwapBuffers();
}


/**************************************** main() ********************/

int main(int argc, char* argv[])
{
    /****************************************/
    /*   Initialize GLUT and create window  */
    /****************************************/
    width=HFOV;
    height=VFOV;
    glutInit(&argc, argv);
    /****************************************/
    /*          READFILE                    */
    /****************************************/
    LoadImage("mandrill_256.jpg"); //mandrill_256
    reader.readfile("cube_text.txt");
    if(reader.fileRead)
        init_lookat_vars();
    /****************************************/
    /*       Set up OpenGL lights           */
    /****************************************/
    ambient[0] = reader.ambient[0].x;
    ambient[1] = reader.ambient[0].y;
    ambient[2] = reader.ambient[0].z;
    ambient[3]=1.0f;
    diffuse[0] = reader.diffuse[0].x;
    diffuse[1] = reader.diffuse[0].y;
    diffuse[2] = reader.diffuse[0].z;
    diffuse[3]=1.0f;
    specular[0] = reader.specular[0].x;
    specular[1] = reader.specular[0].y;
    specular[2] = reader.specular[0].z;
    specular[3]=1.0f;

    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition( 345, 0 );
    glutInitWindowSize( 400, 400 );

    main_window = glutCreateWindow( "Programming Assignment 3 OpenGL" );
    glutDisplayFunc( myGlutDisplay );
    GLUI_Master.set_glutReshapeFunc( myGlutReshape );
    GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
    GLUI_Master.set_glutSpecialFunc( NULL );
    GLUI_Master.set_glutMouseFunc( myGlutMouse );
    glutMotionFunc( myGlutMotion );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
    glEnable(GL_LIGHTING);
    glEnable( GL_NORMALIZE );
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);


    if(reader.textureEnable)
    {
        read_texture_image_and_set();
        close2gl.read_texture_image_and_set(tx.colors,tx.width,tx.height);
    }
    //
    // Now, initialize Close2GL window
    //
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(745, 0);
    glutInitWindowSize(400, 400);
    windows2CGL = glutCreateWindow("Programming Assignment 3 Close2GL");
    glutDisplayFunc( myGlutDisplay );
    GLUI_Master.set_glutReshapeFunc( myGlutReshape );
    GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
    GLUI_Master.set_glutSpecialFunc( NULL );
    GLUI_Master.set_glutMouseFunc( myGlutMouse );
    glutMotionFunc( myGlutMotion );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
    GLUI_Master.set_glutIdleFunc( myGlutIdle );
    glEnable(GL_LIGHTING);
    glEnable( GL_NORMALIZE );

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    if(reader.textureEnable)
    {
        read_texture_image_and_set();
        close2gl.read_texture_image_and_set(tx.colors,tx.width,tx.height);
    }
    /****************************************/
    /*          Enable z-buferring          */
    /****************************************/
    //glEnable(GL_DEPTH_TEST);
    /****************************************/
    /*         Here's the GLUI code         */
    /****************************************/
    set_glui_gui();
#if 0
    /**** We register the idle callback with GLUI, *not* with GLUT ****/
    GLUI_Master.set_glutIdleFunc( myGlutIdle );
#endif

    /**** Regular GLUT main loop ****/

    glutMainLoop();

    return EXIT_SUCCESS;
}


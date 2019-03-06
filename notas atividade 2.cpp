#include <GL/glut.h>
#define OPENGL_WINDOW 0
#define CLOSE2GL_WINDOW 1
int win_id[2];
float Znear = 0.1, // near clipping plane
      Zfar = 10.0, // far clipping plane
      Hfov = 60.0, // horizontal and
      Vfov = 60.0; // vertical field of view
and use it with the following template of main function. Notice that I will be using C++ notation.
//*******************************************************************************
//
// main function for controlling the implementation of your assignment
//
//*******************************************************************************
int main(int argc, char *argv[])
{
//
// load the triangle model by calling your function that reads the triangle model description file
// passed as the first argument to your program
//
    <tri model>.<your function for reading an input file>(argv[1]);
//
// Initialize two windows, one for rendering OpenGL and another one for rendering Close2GL.
// Make sure you understand the meaning of the parameters used with each command. In particular,
// make sure you understand the meaning of the parameters used with glutInitDisplayMode and understand
// its relationship to the glutSwapBuffers command. You can find detailed explanations about them in the
// OpenGL red book
//
// First, initialize OpenGL window
//
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(<win width>, <win height>);
    win_id[OPENGL_WINDOW] = glutCreateWindow("OpenGL");
    glutDisplayFunc(<name of your function for rendering using OpenGL >);
    glutReshapeFunc(openglReshape);
    glutMouseFunc(<mouse control function>);
    glutMotionFunc(<mouse motion control function>);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0, 0.0, 0.0, 0);
    glEnable(GL_DEPTH_TEST);
//
// Now, initialize Close2GL window
//
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(450, 0);
    glutInitWindowSize((<win width>, <win height>);
                       win_id[CLOSE2GL_WINDOW] = glutCreateWindow("Close2GL");
                       glutDisplayFunc(<name of your function for rendering using Close2GL >);
                       glutReshapeFunc(close2glReshape)
                       glutMouseFunc(<mouse control function>);
                       glutMotionFunc(<mouse motion control function>);
                       glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                       glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
                       glClearColor(0.0, 0.0, 0.0, 0);
                       glEnable(GL_DEPTH_TEST);
//
// call your function for initializing your user interface
//
                       <your function for initializing your user interface>;
//
// call glutMainLoop()
//
                           glutMainLoop();
                           return 0;
    }
1) Supporting Functions
//*******************************************************************************
//
// opengl reshape function
//
//*******************************************************************************
void openglReshape(int w, int h)
{
glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(VFov, Hfov/Vfov, Znear, Zfar);
    glMatrixMode(GL_MODELVIEW);
}
//*******************************************************************************
//
// close2gl reshape function
//
//*******************************************************************************
void close2glReshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
//
// create a projection matrix for your close2gl implementation using the initial parameters
// yfov, aspect ratio, Znear, Zfar. Remember that the projection matrix is defined in terms of
// left, right, bottom, top, near and far. So, you will need to recover left, right, bottom and top
// from near and from Hfov and Vfov in your function.
//
    <close2gl projection matrix>.<your function to create a projection matrix>(Vfov, Hfov/Vfov, Znear, Zfar);
//
// create a viewport matrix for your close2gl implementation. Use the same (x,y) range specified in glOrtho
// above. See the tips on how to create a viewport matrix in the document that describes the programming
// assignment.
//
    <close2gl viewport matrix>.<your function to create a viewport matrix>(0.0, 1.0, 0.0, 1.0);
}
//*******************************************************************************
// opengl display function. <render mode> specifies whether you
// should render using points, wireframe or solid mode. <clip mode>
// says whether you should clip or not vertices that fall outside of the
// view frustum.
//
//******************************************************************************
void openglDisplay(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // initialize ModelView Matrix as the Identity Matrix
    gluLookAt(<put here the parameters you can extract from your camera>);
    <tri model>.<your opengl renderer> (<render mode>, <clip mode>);
}
//*******************************************************************************
// close2gl display function. <render mode> specifies whether you
// should render using points, wireframe or solid mode. <clip mode>
// says whether you should clip or not vertices that fall outside of the
// view frustum.
//
//*******************************************************************************
void close2glDisplay(void)
{
    <set a model view matrix based on the most current camera parameters to be used in your rendering>
    <tri model>.<your close2gl renderer> (<render mode>, <clip mode>);
}
/*Synchronizing the Renderings in all Windows
A simple way to guarantee synchronized renderings in all windows is to loop through all windows
and call glutPostRedisplay for each one of them. The next code fragment illustrates this for the
case of changing rendering mode and for changing of field of view.*/
    switch (Option)
    {
    case RENDER_MODE:
//
// redraw all windows
//
        for (i=0; i<MAX_WINDOWS; i++)
            {
                glutSetWindow(win_id[i]);
                glutPostRedisplay();
            }
            break;
        case H_FOV:
        case V_FOV:
//
// code for reinitializing OpenGL projection matrix
//
            glutSetWindow(OPENGL_WINDOW);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(Vfov, Hfov/Vfov, Znear, Zfar);
            glMatrixMode(GL_MODELVIEW);
//
// reinitialize Close2GL’s projection matrix and redraw all windows
//
            <close2gl projection matrix>.<your function to create a projection matrix>(Vfov, Hfov/Vfov, Znear, Zfar);
            glutSetWindow(CLOSE2GL_WINDOW);
            for (i=0; i<MAX_WINDOWS; i++)
            {
                glutSetWindow(win_id[i]);
                glutPostRedisplay();
            }
            break;
        }
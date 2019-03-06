
#define PI 3.14159265

class Camera
{
    public:
        vector3f position, u, v, n,lookat;
        float hfov,vfov,right,left,top,bottom,angle,znear,zfar;
        // Constructor
        Camera(void)
        {
            float rad;
            position = vector3f(0,0,0);
            u = vector3f(1,0,0);
            v = vector3f(0,1,0);
            n = vector3f(0,0,1);
            lookat= vector3f(0,0,1);
            angle = 60.0f;
            znear= 1.0f;
            zfar= 3000.0f;
            rad = (angle*PI)/180.0f;
            right = tan(rad/2.0f) * znear;
            left = -right;
            top = right;
            bottom = -top;
            hfov = right-left;
            vfov = top-bottom;
        }
};

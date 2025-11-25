//glutsphere
//https://web.archive.org/web/20120826075539/http://ozark.hendrix.edu/~burch/cs/490/sched/feb8/

//there wasn't a "glutSolidSphere" and "glutWireSphere" on libGL

#define PROJECT_NAME "DCSphere"

#include <kos.h>
#include <math.h>
#include <stdio.h>


#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glkos.h>
#include <GL/glu.h>

#ifdef DEBUG
#include <kos/dbgio.h>
#include <arch/gdb.h>
#endif

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);


float user_theta = 0;
float user_height = 0;

float xrot = 0;
float yrot = 0;
float zrot = 0;

float xrot2 = 0;
float yrot2 = 0;
float zrot2 = 0;


void drawWireSphereDC(float r, int lats, int longs)
{
    int i, j;
    for(i = 0; i <= lats; i++)
    {
        float lat0 = M_PI * (-0.5 + (float) (i - 1)/ lats);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float) i / lats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_LINE_STRIP);  //swap from Quad strip to line strip
        for(j=0; j<= longs; j++)
        {
            float lng = 2 * M_PI * (float) (j - 1) /longs;
            float x = cos(lng);
            float y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            //glColor3f(1.0, 0.0, 1.0);
            glVertex3f(x * zr0, y * zr0, z0);
            //glColor3f(0.0, 1.0, 0.0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);

        }
        glEnd();
    }
}







void drawSolidSphereDC(float r, int lats, int longs)
{
    int i, j;
    for(i = 0; i <= lats; i++)
    {
        float lat0 = M_PI * (-0.5 + (float) (i - 1)/ lats);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float) i / lats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);  //swap from Quad strip to line strip
        for(j=0; j<= longs; j++)
        {
            float lng = 2 * M_PI * (float) (j - 1) /longs;
            float x = cos(lng);
            float y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glColor3f(1.0, 0.0, 1.0);
            glVertex3f(x * zr0, y * zr0, z0);
            glColor3f(0.0, 1.0, 0.0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);

        }
        glEnd();
    }
}

void computeLocation()
{
    float x = 2 * cos(user_theta);
    float y = 2 * sin(user_theta);
    float z = user_height;
    float d = sqrt(x * x + y * y + z * z);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-d * 0.5, d * 0.5, -d * 0.5, d * 0.5, d -1.1, d + 1.1);
    gluLookAt(x, y, z, 0, 0, 0,  0, 0, 1);

}

void Init(int Width, int Height)
{
    GLfloat sun_direction[] = {0.0, 2.0, -1.0, 1.0};
    GLfloat sun_intensity[] = {0.7, 0.7, 0.7, 1.0};
    GLfloat ambient_intensity[] = {0.5, 0.5, 0.5, 1.0};

    GLfloat backlight_direction[] = {0.0, -3.0, -1.0, 1.0};
    GLfloat backlight_intensity[] = {0.3, 0.3, 0.3, 1.0};

    glClearColor(0.0, 0.0, 0.0, 0.5);
    //computeLocation();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f, 100.0f);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glEnable(GL_DEPTH_TEST);

    //it makes it run at 10 fps, not worth it

    /*
    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, backlight_direction);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, backlight_intensity);


    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    */


}


void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);
    drawSolidSphereDC(5.0, 30, 30);
    xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.1f;


    glTranslatef(0.0f, 0.0f, -1.0f);
    glRotatef(xrot2, 1.0, 0.0, 0.0);
    glRotatef(yrot2, 0.0, 1.0, 0.0);
    glRotatef(zrot2, 0.0, 0.0, 1.0);
    drawWireSphereDC(1.0, 10, 10);
	xrot2+=1.3f;
	yrot2+=1.2f;
	zrot2+=1.1f;

    glKosSwapBuffers();

}




int main(int argc, char *argv[])
{
    glKosInit();
    Init(640, 480);

    while(1)
    {
        draw();
    }


    return 0;
}

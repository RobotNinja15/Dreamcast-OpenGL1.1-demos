#define PROJECT_NAME "wavyTextureDC"
#include <math.h>
#include <kos.h>
#include "loadbmp.h"

// libGL
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glkos.h>
#include <GL/glu.h>

#ifdef DEBUG
#include <kos/dbgio.h>
#include <arch/gdb.h>
#endif


float points[45][45][3];
int wiggle_count = 0;

GLfloat xrot;
GLfloat yrot;
GLfloat zrot;
GLfloat hold;

GLuint texture[1];


//Load Bitmaps and convert to textures
void LoadGLTextures()
{
    Image *image1;

    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL)
    {
        printf("Error allocating space");
        //return 0;

    }

    if (!ImageLoad("/rd/Image11.bmp", image1))
    {
        printf("Image Loaded");
    }

    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    free(image1);



};

void ReSizeGLScene(int Width, int Height)
{
    if (Height == 0)
        Height = 1;
    glViewport(0,0, Width, Height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

}

int InitGL(int Width, int Height)
{
    //LoadGLTextures();
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)Width/(GLfloat)Height, 0.f, 100.0f);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glPolygonMode(GL_BACK, GL_FILL);
    glPolygonMode(GL_FRONT, GL_LINE);

    for(int x=0; x<45; x++)
    {
        for(int y=0; y<45; y++)
        {
            points[x][y][0] = (float)(x/5.0f - 4.5f);
            points[x][y][1] = (float)(y/5.0f - 4.5f);
            points[x][y][2]= (float)(sin(((x/5.0f*40.0f)/360.0f)*3.141592654*2.0f));
        }
    }

    return true;
}


int DrawGLScene()
{
    int x, y;
    float float_x, float_y, float_xb, float_yb;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -20.0f);

    //glRotatef(xrot,1.0f,0.0f,0.0f);
	//glRotatef(yrot,0.0f,1.0f,0.0f);
	//glRotatef(zrot,0.0f,0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	for( x = 0; x < 44; x++ )
	{
		for( y = 0; y < 44; y++ )
		{
			float_x = (float)x/44.0f;
			float_y = (float)y/44.0f;
			float_xb = (float)(x+1)/44.0f;
			float_yb = (float)(y+1)/44.0f;

			glTexCoord2f( float_x, float_y);
			glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

			glTexCoord2f( float_x, float_yb );
			glVertex3f( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

			glTexCoord2f( float_xb, float_yb );
			glVertex3f( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

			glTexCoord2f( float_xb, float_y );
			glVertex3f( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();

	if( wiggle_count == 2 )
	{
		for( y = 0; y < 45; y++ )
		{
			hold=points[0][y][2];
			for( x = 0; x < 44; x++)
			{
				points[x][y][2] = points[x+1][y][2];
			}
			points[44][y][2]=hold;
		}
		wiggle_count = 0;
	}

	wiggle_count++;

	xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.0f;

	glKosSwapBuffers();



    return true;

}

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);






int main(int argc, char *argv[])
{
    glKosInit();
    LoadGLTextures();
    InitGL(640, 480);
    ReSizeGLScene(640, 480);

    while(1)
    {
        DrawGLScene();
    }

    return 0;
}

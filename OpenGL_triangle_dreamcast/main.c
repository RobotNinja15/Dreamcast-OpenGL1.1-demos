/*******************************************************************************
	Sega Dreamcast Project

	Project name : OpenGL_triangle_dreamcast
	Created on   : 2025-07-26
*******************************************************************************/

#define PROJECT_NAME "OpenGL_triangle_dreamcast"

#include <stdbool.h>

#include <kos.h>


#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glkos.h>
#include <GL/glu.h>

#ifdef DEBUG
#include <kos/dbgio.h>
#include <arch/gdb.h>
#endif


bool isDepthEnabled = true;

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);


extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);


int check_start() {
#ifdef _arch_dreamcast
    static uint8_t prevButtons = 0;
    maple_device_t *cont;
    cont_state_t *state;

    cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

    if(cont) {
        state = (cont_state_t *)maple_dev_status(cont);

        if(state && (state->buttons & CONT_A) && !(prevButtons & CONT_A))
        {
            isDepthEnabled = !isDepthEnabled;
            isDepthEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        }

        prevButtons = state->buttons;

    }
#endif

    return 0;
}



void displayfunc(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef(2, 1,1,0);

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glEnd();



    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.0, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();


    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.5, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.2, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glEnd();


    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glEnd();


    glFlush();


    glKosSwapBuffers();
}



int main(int argc, char *argv[]) {

    glKosInit();
    //glEnable(GL_DEPTH_TEST);   // for enabling depth test

    while(1){
        check_start();
        displayfunc();
    }
    return 0;
}

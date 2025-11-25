#define PROJECT_NAME "Dreamcast Terrain"

#include <kos.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glkos.h>
#include <GL/glu.h> // optional: only for gluPerspective

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

#define MAP_SIZE      1024
#define STEP_SIZE     16
#define HEIGHT_RATIO  1.5f

static uint8 g_HeightMap[MAP_SIZE * MAP_SIZE];

float scaleValue = 0.15f;
bool bRender = true;
float cameraY = -40.0f;
float cameraX = -40.0f;
float cameraZ = -250.0f;

float yaw = 1;
float pitch = 1;
float roll = 1;
float angleROTZ = 1;
float angleROTX = 1;
float angleROTY = 1;

int Height(int x, int y) {
    x = x % MAP_SIZE;
    y = y % MAP_SIZE;
    return g_HeightMap[x + (y * MAP_SIZE)];
}

void SetVertexColor(int x, int y) {
    float fColor = -0.15f + (Height(x, y) / 256.0f);
    glColor3f(0.0f, 0.0f, fColor);
}

void RenderHeightMap() {
    int x, y, z;
    if (bRender)
        glBegin(GL_QUADS);
    else
        glBegin(GL_LINES);

    for (int X = 0; X < (MAP_SIZE - STEP_SIZE); X += STEP_SIZE) {
        for (int Y = 0; Y < (MAP_SIZE - STEP_SIZE); Y += STEP_SIZE) {

            // Bottom Left
            x = X;
            y = Height(X, Y);
            z = Y;
            SetVertexColor(x, z);
            glVertex3f((float)x, (float)y, (float)z);

            // Top Left
            x = X;
            y = Height(X, Y + STEP_SIZE);
            z = Y + STEP_SIZE;
            SetVertexColor(x, z);
            glVertex3f((float)x, (float)y, (float)z);

            // Top Right
            x = X + STEP_SIZE;
            y = Height(X + STEP_SIZE, Y + STEP_SIZE);
            z = Y + STEP_SIZE;
            SetVertexColor(x, z);
            glVertex3f((float)x, (float)y, (float)z);

            // Bottom Right
            x = X + STEP_SIZE;
            y = Height(X + STEP_SIZE, Y);
            z = Y;
            SetVertexColor(x, z);
            glVertex3f((float)x, (float)y, (float)z);
        }
    }

    glEnd();
    glColor3f(1, 1, 1); // reset color
}

int LoadRawFile(const char *filename, uint8 *buffer, int size) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: could not open %s\n", filename);
        return 0;
    }
    fread(buffer, 1, size, fp);
    fclose(fp);
    return 1;
}

void InitGL(int w, int h) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1.0f);
    glShadeModel(GL_SMOOTH);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void ReSizeGLScene(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}



void update_input()
{
    //static uint8_t prevButtons = 0;
    maple_device_t* cont;
    cont_state_t* state;

    cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

    if (cont)
    {
        state = (cont_state_t*)maple_dev_status(cont);

        if (state->buttons & CONT_DPAD_UP)
        {
            angleROTZ += 1.0f;

            //glRotatef(angleROT, 0,0,1);

        }

        if (state->buttons & CONT_DPAD_DOWN)
        {
            angleROTZ -= 1.0f;


        }

        if (state->buttons & CONT_DPAD_RIGHT)
        {
            angleROTX += 1.0f;


        }

        if (state->buttons & CONT_DPAD_LEFT)
        {
            angleROTX -= 1.0f;



        }

        if (state->buttons & CONT_A)
        {
            angleROTY += 1.0f;


        }

        if (state->buttons & CONT_B)
        {
            angleROTY -= 1.0f;


        }



        //prevButtons = state->buttons;

    }


}

void DrawGLScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(cameraX, cameraY, cameraZ);
    //glTranslatef(-MAP_SIZE/2.0f, -30.0f, -MAP_SIZE/2.0f);

    glScalef(scaleValue, scaleValue * HEIGHT_RATIO, scaleValue);

    //static float rot = 0.0f;
    //rot += 1.0f;

    update_input();

    // yaw = Y, pitch = Z, roll = X
    //glRotatef(0, roll, yaw, pitch);

    glRotatef(angleROTZ, 0,0,1);
    glRotatef(angleROTX, 1,0,0);
    glRotatef(angleROTY, 0,1,0);

    RenderHeightMap();

    glKosSwapBuffers();
}

int main(int argc, char **argv) {
    glKosInit();

    if (!LoadRawFile("/rd/Terrain.raw", g_HeightMap, MAP_SIZE * MAP_SIZE)) {
        printf("Failed to load terrain heightmap.\n");
        return -1;
    }

    InitGL(640, 480);
    ReSizeGLScene(640, 480);

    printf("Dreamcast Terrain Loaded. Press START to exit, D-PAD up/down to zoom, A to toggle mode.\n");

    while (1) {


        DrawGLScene();
    }

    return 0;
}


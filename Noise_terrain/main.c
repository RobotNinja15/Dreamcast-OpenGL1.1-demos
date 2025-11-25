#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _arch_dreamcast
#include <kos.h>
#endif

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glkos.h"

#define TERRAIN_SIZE 40
#define TERRAIN_SCALE 1.0f
#define TERRAIN_HEIGHT_SCALE 10.0f
#define NOISE_SCALE 5.1f

typedef struct {
    float x, y, z;
} Vec3;

Vec3 vertices[TERRAIN_SIZE * TERRAIN_SIZE];
Vec3 normals[TERRAIN_SIZE * TERRAIN_SIZE];
unsigned int indices[(TERRAIN_SIZE - 1) * TERRAIN_SIZE * 2];

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float grad(int hash, float x, float y) {
    int h = hash & 7;
    float u = h < 4 ? x : y;
    float v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

int perm[512];

void init_perlin() {
    int p[256];
    for (int i = 0; i < 256; i++) p[i] = i;

    for (int i = 255; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = p[i];
        p[i] = p[j];
        p[j] = tmp;
    }

    for (int i = 0; i < 256; i++) {
        perm[i] = perm[256 + i] = p[i];
    }
}

float perlin2D(float x, float y) {
    int xi = (int)floorf(x) & 255;
    int yi = (int)floorf(y) & 255;

    float xf = x - floorf(x);
    float yf = y - floorf(y);

    float u = fade(xf);
    float v = fade(yf);

    int aa = perm[perm[xi] + yi];
    int ab = perm[perm[xi] + yi + 1];
    int ba = perm[perm[xi + 1] + yi];
    int bb = perm[perm[xi + 1] + yi + 1];

    float x1 = lerp(grad(aa, xf, yf), grad(ba, xf - 1, yf), u);
    float x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u);
    return (lerp(x1, x2, v) + 1.0f) / 2.0f;
}

// Vector math
Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return (Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

Vec3 vec3_normalize(Vec3 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len == 0) return (Vec3){0, 1, 0};
    return (Vec3){v.x / len, v.y / len, v.z / len};
}

void GenerateTerrain() {
    const float OFFSET = (-TERRAIN_SIZE * TERRAIN_SCALE) / 2.0f;

    for (unsigned int z = 0; z < TERRAIN_SIZE; ++z) {
        for (unsigned int x = 0; x < TERRAIN_SIZE; ++x) {
            unsigned int i = z * TERRAIN_SIZE + x;
            vertices[i].x = x * TERRAIN_SCALE + OFFSET;
            vertices[i].z = z * TERRAIN_SCALE + OFFSET;

            float nx = x * NOISE_SCALE;
            float nz = z * NOISE_SCALE;
            float height = perlin2D(nx, nz);
            vertices[i].y = height * TERRAIN_HEIGHT_SCALE;
        }
    }

    for (unsigned int z = 1; z < TERRAIN_SIZE - 1; ++z) {
        for (unsigned int x = 1; x < TERRAIN_SIZE - 1; ++x) {
            unsigned int i = z * TERRAIN_SIZE + x;
            Vec3 left = vertices[z * TERRAIN_SIZE + (x - 1)];
            Vec3 right = vertices[z * TERRAIN_SIZE + (x + 1)];
            Vec3 up = vertices[(z - 1) * TERRAIN_SIZE + x];
            Vec3 down = vertices[(z + 1) * TERRAIN_SIZE + x];

            Vec3 dx = vec3_sub(right, left);
            Vec3 dz = vec3_sub(down, up);

            Vec3 normal = vec3_cross(dz, dx);
            normals[i] = vec3_normalize(normal);
        }
    }

    for (unsigned int i = 0; i < TERRAIN_SIZE * TERRAIN_SIZE; ++i) {
        if (i < TERRAIN_SIZE || i >= TERRAIN_SIZE * (TERRAIN_SIZE - 1) ||
            i % TERRAIN_SIZE == 0 || i % TERRAIN_SIZE == TERRAIN_SIZE - 1)
            normals[i] = (Vec3){0, 1, 0};
    }
}

void InitGL(int Width, int Height) {
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // sky blue
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Light setup
    GLfloat light_pos[] = {1.0f, 1.0f, 0.0f, 0.0f}; // directional
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    // Fog setup
    GLfloat fogColor[] = {0.5f, 0.7f, 1.0f, 1.0f};
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 30.0f);
    glFogf(GL_FOG_END, 100.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);

    GenerateTerrain();
    fprintf(stderr, "Terrain and normals generated\n");
}

void ReSizeGLScene(int Width, int Height) {
    if (Height == 0) Height = 1;
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

int check_start() {
#ifdef _arch_dreamcast
    maple_device_t *cont;
    cont_state_t *state;

    cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
    if (cont) {
        state = (cont_state_t *)maple_dev_status(cont);
        if (state) return state->buttons & CONT_START;
    }
#endif
    return 0;
}

void DrawGLScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, -2.5f, -60.0f);

    static float angle = 0;
    angle += 0.3f;
    glRotatef(angle, 0, 1, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);

    for (unsigned int z = 0; z < TERRAIN_SIZE - 1; ++z) {
        unsigned int idx = 0;
        for (unsigned int x = 0; x < TERRAIN_SIZE; ++x) {
            unsigned int top = (z * TERRAIN_SIZE) + x;
            unsigned int bottom = ((z + 1) * TERRAIN_SIZE) + x;
            indices[idx++] = top;
            indices[idx++] = bottom;
        }
        glDrawElements(GL_TRIANGLE_STRIP, TERRAIN_SIZE * 2, GL_UNSIGNED_INT, indices);
    }

    glKosSwapBuffers();
}

int main(int argc, char **argv) {
    glKosInit();
    InitGL(640, 480);
    ReSizeGLScene(640, 480);

    while (1) {
        if (check_start())
            break;

        DrawGLScene();
    }

    return 0;
}


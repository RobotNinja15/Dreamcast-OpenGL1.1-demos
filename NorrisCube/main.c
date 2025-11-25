#define PROJECT_NAME "NorrisCube"

#include <kos.h>


#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glkos.h>
#include <GL/glu.h>

#include <png/png.h>
#include "Png-Texture.c"

#ifdef DEBUG
#include <kos/dbgio.h>
#include <arch/gdb.h>
#endif



extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

/*

#define CLEANUP(x) { ret = (x); goto cleanup; }

struct texture {
	GLuint id;
	GLenum format;
	GLenum min_filter;
	GLenum mag_filter;
	uint16_t w, h;
};


static int png_to_gl_texture(struct texture * tex, char const * const filename) {
	int ret = 0;
	FILE * file = 0;
	uint8_t * data = 0;
	png_structp parser = 0;
	png_infop info = 0;
	png_bytep * row_pointers = 0;

	png_uint_32 w, h;
	int bit_depth;
	int color_type;


	if(!tex || !filename) {
		CLEANUP(1);
	}

	file = fopen(filename, "rb");
	if(!file) {
		CLEANUP(2);
	}

	parser = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if(!parser) {
		CLEANUP(3);
	}

	info = png_create_info_struct(parser);
	if(!info) {
		CLEANUP(4);
	}

	if(setjmp(png_jmpbuf(parser))) {
		CLEANUP(5);
	}

	png_init_io(parser, file);
	png_read_info(parser, info);
	png_get_IHDR(parser, info, &w, &h, &bit_depth, &color_type, 0, 0, 0);

	if((w & (w-1)) || (h & (h-1)) || w < 8 || h < 8) {
		CLEANUP(6);
	}

	if(png_get_valid(parser, info, PNG_INFO_tRNS) || (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) || color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_expand(parser);
	}
	if(bit_depth == 16) {
		png_set_strip_16(parser);
	}
	if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(parser);
	}
	png_read_update_info(parser, info);

	int rowbytes = png_get_rowbytes(parser, info);
	rowbytes += 3 - ((rowbytes-1) % 4); // align to 4 bytes

	data = malloc(rowbytes * h * sizeof(png_byte) + 15);
	if(!data) {
		CLEANUP(7);
	}

	row_pointers = malloc(h * sizeof(png_bytep));
	if(!row_pointers) {
		CLEANUP(8);
  }

	// set the individual row_pointers to point at the correct offsets of data
	for(png_uint_32 i = 0; i < h; ++i) {
		row_pointers[h - 1 - i] = data + i * rowbytes;
	}

	png_read_image(parser, row_pointers);

	// Generate the OpenGL texture object
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	GLenum texture_format = (color_type & PNG_COLOR_MASK_ALPHA) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, w, h, 0, texture_format, GL_UNSIGNED_BYTE, data);

	tex->id = texture_id;
	tex->w = w;
	tex->h = h;
	tex->format = texture_format;
	tex->min_filter = tex->mag_filter = GL_NEAREST;


cleanup:
	if(parser) {
		png_destroy_read_struct(&parser, info ? &info : 0, 0);
	}

	if(row_pointers) {
		free(row_pointers);
	}

	if(ret && data) {
		free(data);
	}

	if(file) {
		fclose(file);
	}

	return ret;
}

*/




void displayfunc(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef(2, 1,1,0);



    glBegin(GL_POLYGON);
    //glColor3f(1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
    glEnd();



    glBegin(GL_POLYGON);
    //glColor3f(0.5, 0.0, 0.5);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
    glEnd();


    glBegin(GL_POLYGON);
    //glColor3f(0.0, 0.5, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    glBegin(GL_POLYGON);
    //glColor3f(0.5, 0.2, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -0.5, 0.5);
    glEnd();


    glBegin(GL_POLYGON);
    //glColor3f(1.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    glBegin(GL_POLYGON);
    //glColor3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
    glEnd();


    glFlush();


    glKosSwapBuffers();
}

void init(struct texture const * tex){



    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


int main(int argc, char *argv[]) {
    struct texture tex;
    glKosInit();

    int ret = png_to_gl_texture(&tex, "/rd/image.png");
	if(ret) {
		printf("Cannot load texture, error code %d.\n", ret);
		return 1;
	}

    init(&tex);

	while(1)
    {
        //float x = (640 - tex.w) / 2.f;
        //float y = (480 - tex.h) / 2.f;
        displayfunc();

	}


    return 0;
}

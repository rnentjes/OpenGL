#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
#include <FreeImage.h>

#include "shaderUtils.h"

/**
 * Store all the file's contents in memory, useful to pass shaders
 * source code to OpenGL
 */
char* file_read(const char* filename) {
    FILE* in = fopen(filename, "rb");
    if (in == NULL) return NULL;

    int res_size = BUFSIZ;
    char* res = (char*) malloc(res_size);
    int nb_read_total = 0;

    while (!feof(in) && !ferror(in)) {
        if (nb_read_total + BUFSIZ > res_size) {
            if (res_size > 10 * 1024 * 1024) break;
            res_size = res_size * 2;
            res = (char*) realloc(res, res_size);
        }
        char* p_res = res + nb_read_total;
        nb_read_total += fread(p_res, 1, BUFSIZ, in);
    }

    fclose(in);
    res = (char*) realloc(res, nb_read_total + 1);
    res[nb_read_total] = '\0';
    return res;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void print_log(GLuint object) {
    GLint log_length = 0;
    if (glIsShader(object))
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else if (glIsProgram(object))
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else {
        fprintf(stderr, "printlog: Not a shader or a program\n");
        return;
    }

    char* log = (char*) malloc(log_length);

    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);

    fprintf(stderr, "%s", log);
    free(log);
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLuint create_shader(const char* filename, GLenum type) {
    const GLchar* source = file_read(filename);
    if (source == NULL) {
        fprintf(stderr, "Error opening %s: ", filename);
        perror("");
        return 0;
    }
    GLuint res = glCreateShader(type);
    glShaderSource(res, 1, &source, NULL);
    free((void*) source);

    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE) {
        fprintf(stderr, "%s:", filename);
        print_log(res);
        glDeleteShader(res);
        return 0;
    }

    return res;
}

/*
 * Functions for creating OpenGL objects:
 */
GLuint make_buffer(
        GLenum target,
        const void *buffer_data,
        GLsizei buffer_size
        ) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;
}

void *file_contents(const char *filename, GLint *length) {
    FILE *f = fopen(filename, "r");
    void *buffer;

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(*length + 1);
    *length = fread(buffer, 1, *length, f);
    fclose(f);
    ((char*) buffer)[*length] = '\0';

    return buffer;
}

static short le_short(unsigned char *bytes) {
    return bytes[0] | ((char) bytes[1] << 8);
}

void *read_tga(const char *filename, int *width, int *height) {

    struct tga_header {
        char id_length;
        char color_map_type;
        char data_type_code;
        unsigned char color_map_origin[2];
        unsigned char color_map_length[2];
        char color_map_depth;
        unsigned char x_origin[2];
        unsigned char y_origin[2];
        unsigned char width[2];
        unsigned char height[2];
        char bits_per_pixel;
        char image_descriptor;
    } header;
    int i, color_map_size, pixels_size;
    FILE *f;
    size_t read;
    void *pixels;

    f = fopen(filename, "rb");

    if (!f) {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    read = fread(&header, 1, sizeof (header), f);

    if (read != sizeof (header)) {
        fprintf(stderr, "%s has incomplete tga header\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.data_type_code != 2) {
        fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.bits_per_pixel != 24) {
        fprintf(stderr, "%s is not a 24-bit uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }

    for (i = 0; i < header.id_length; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete id string\n", filename);
            fclose(f);
            return NULL;
        }

    color_map_size = le_short(header.color_map_length) * (header.color_map_depth / 8);
    for (i = 0; i < color_map_size; ++i)
        if (getc(f) == EOF) {
            fprintf(stderr, "%s has incomplete color map\n", filename);
            fclose(f);
            return NULL;
        }

    *width = le_short(header.width);
    *height = le_short(header.height);
    pixels_size = *width * *height * (header.bits_per_pixel / 8);
    pixels = malloc(pixels_size);

    read = fread(pixels, 1, pixels_size, f);

    if (read != pixels_size) {
        fprintf(stderr, "%s has incomplete image\n", filename);
        fclose(f);
        free(pixels);
        return NULL;
    }

    return pixels;
}

GLuint make_texture_old(const char *filename) {
    int width, height;
    void *pixels = read_tga(filename, &width, &height);
    GLuint texture;

    if (!pixels)
        return 0;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
            GL_TEXTURE_2D, 0, /* target, level */
            GL_RGB8, /* internal format */
            width, height, 0, /* width, height, border */
            GL_BGR, GL_UNSIGNED_BYTE, /* external format, type */
            pixels /* pixels */
            );
    free(pixels);
    return texture;
}

GLuint make_texture(const char *filename) {

    GLuint texture;

    // Get the image file type from FreeImage.
    FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filename, 0);

    // Actually load the image file.
    FIBITMAP *dib = FreeImage_Load(fifmt, filename, 0);

    // Now, there is no guarantee that the image file
    // loaded will be GL_RGB, so we force FreeImage to
    // convert the image to GL_RGB.
    dib = FreeImage_ConvertTo32Bits(dib);

    if (dib != NULL) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // This is important to note, FreeImage loads textures in
        // BGR format. Now we could just use the GL_BGR extension
        // But, we will simply swap the B and R components ourselves.
        // Firstly, allocate the new bit data doe the image.
        //BYTE *bits = malloc(FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 4);

        // get a pointer to FreeImage's data.
        BYTE *pixels = (BYTE*) FreeImage_GetBits(dib);

        // Iterate through the pixels, copying the data
        // from 'pixels' to 'bits' except in RGB format.
        /*
        int pix;
        for (pix = 0; pix < FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pix++) {
            bits[pix * 4 + 0] = pixels[pix * 4 + 2];
            bits[pix * 4 + 1] = pixels[pix * 4 + 1];
            bits[pix * 4 + 2] = pixels[pix * 4 + 0];
            bits[pix * 4 + 3] = pixels[pix * 4 + 3];
        }*/

        // The new 'glTexImage2D' function, the prime difference
        // being that it gets the width, height and pixel information
        // from 'bits', which is the RGB pixel data..
        //glTexImage2D(GL_TEXTURE_2D, 0, 4, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,
        //       GL_RGBA, GL_UNSIGNED_BYTE, bits);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,
                GL_BGRA, GL_UNSIGNED_BYTE, pixels);


        // Unload the image.
        // and free the bit data.
        FreeImage_Unload(dib);
        //free(bits);
    } else {
        fprintf(stderr, "Can't load texture: %s\n", filename);
    }
    
    return texture;
}
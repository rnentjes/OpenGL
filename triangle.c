/* 
 * File:   triangle.c
 * Author: rnentjes
 *
 * Created on January 17, 2012, 7:41 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/glut.h>

#include "shaderUtils.h"
#include "texture.h"
#include "glass.h"

GLuint program;
GLuint vbo_triangle;
GLuint vbo_quad;
GLuint vbo_quad_elements;
GLuint vbo_cube_texcoords;
GLuint texture_id;
GLuint texture_id2;
GLuint texture_id3;

GLint attribute_coord2d;
GLint uniform_fade;
GLint uniform_mytexture;
GLint uniform_location;
GLint uniform_size;

/* Global */
static unsigned int program_start = 0;
static unsigned int fps_start = 0;
static unsigned int fps_frames = 0;

int init_resources() {
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

    program_start = fps_start = glutGet(GLUT_ELAPSED_TIME);

    GLuint vs, fs;
    if ((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER)) == 0) return 0;
    if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        fprintf(stderr, "glLinkProgram:");
        print_log(program);
    }

    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_VERTEX_ARRAY);
    
    texture_id = make_texture("glass.gif");
    texture_id2 = make_texture("nehe.gif");
    texture_id3 = make_texture("star.gif");

    GLfloat triangle_vertices[] = {
        0.0, 0.8,
        -0.8, -0.8,
        0.8, -0.8,
    };

    vbo_triangle = make_buffer(GL_ARRAY_BUFFER, triangle_vertices, sizeof (triangle_vertices));

    GLfloat quad_vertices[] = {
        -0.2, -0.2,
         0.2, -0.2,
        -0.2,  0.2,
         0.2,  0.2,
    };

    vbo_quad = make_buffer(GL_ARRAY_BUFFER, quad_vertices, sizeof (quad_vertices));
    
    GLushort quad_elements[] = {
        0, 1, 2, 3,
    };

    vbo_quad_elements = make_buffer(GL_ARRAY_BUFFER, quad_elements, sizeof (quad_elements));
    
    /* init_resources */
    GLfloat cube_texcoords[] = {
        // front
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        1.0, 1.0,
        1.0, 0.0,
        0.0, 0.0,
    };

    vbo_cube_texcoords = make_buffer(GL_ARRAY_BUFFER, cube_texcoords, sizeof (cube_texcoords));
    
    attribute_coord2d = glGetAttribLocation(program, "coord2d");
    if (attribute_coord2d == -1) {
        fprintf(stderr, "Could not bind attribute coord2d\n");
        return 0;
    }

    uniform_mytexture = glGetUniformLocation(program, "mytexture");
    if (uniform_mytexture == -1) {
        fprintf(stderr, "Could not bind uniform mytexture\n");
        return 0;
    }

    uniform_location = glGetUniformLocation(program, "location");
    if (uniform_location == -1) {
        fprintf(stderr, "Could not bind uniform location\n");
        return 0;
    }
    
    uniform_size = glGetUniformLocation(program, "size");
    if (uniform_location == -1) {
        fprintf(stderr, "Could not bind uniform size\n");
        return 0;
    }

    /*
        const char* uniform_name;
        uniform_name = "fade";
        uniform_fade = glGetUniformLocation(program, uniform_name);
        if (uniform_fade == -1) {
            fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
            return 0;
        }
     */

    // Enable alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Clear the background as black */
    glClearColor(0.0, 0.0, 0.0, 1.0);

    return 1;
}

void onDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    /* render */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_id2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_id3);
    
    GLfloat r = (glutGet(GLUT_ELAPSED_TIME) - program_start) / 1000.0;

    //glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
            attribute_coord2d, // attribute
            2, // number of elements per vertex, here (x,y)
            GL_FLOAT, // the type of each element
            GL_FALSE, // take our values as-is
            0, // no extra data between each position
            0 // offset of first element
            );
    //glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
    glEnableVertexAttribArray(attribute_coord2d);
    
    /* Push each element in buffer_vertices to the vertex shader */
    //glBindBuffer(GL_ARRAY_BUFFER, vbo_quad);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_quad_elements);
   
    int i = 100;
    for (; i < 500; i++) {
      r = r * 0.999;
      GLfloat factor =  (i/1000.0)*2.0;
      glUniform2f(uniform_location, sin(r) * factor, cos(r) * factor);
      glUniform2f(uniform_size, factor, factor);
      glUniform1i(uniform_mytexture, i % 3);

      glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
      
      glUniform2f(uniform_location, -sin(r) * factor, -cos(r) * factor);
      glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
      glUniform2f(uniform_location, -sin(r) * factor, cos(r) * factor);
      glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
      glUniform2f(uniform_location, sin(r) * factor, -cos(r) * factor);
      glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
    }
    
    glUniform2f(uniform_location, -0.5, -0.5);
    glUniform2f(uniform_size, 0.7, 0.7);
    glUniform1i(uniform_mytexture, 0);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
    
    glUniform2f(uniform_location, -0.5, 0.0);
    glUniform2f(uniform_size, 0.7, 0.7);
    glUniform1i(uniform_mytexture, 1);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);

    glUniform2f(uniform_location, -0.5, 0.5);
    glUniform2f(uniform_size, 0.7, 0.7);
    glUniform1i(uniform_mytexture, 2);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
    
    
    glDisableVertexAttribArray(attribute_coord2d);
    //glUseProgram(0);
    
    /* Display the result */
    glutSwapBuffers();
}

void free_resources() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_triangle);
    glDeleteBuffers(1, &vbo_quad);
    glDeleteTextures(1, &texture_id);
}

void idle() {
    fps_frames++;
    int delta_t = glutGet(GLUT_ELAPSED_TIME) - fps_start;
    if (delta_t > 1000) {
        fprintf(stdout, "FPS: %f\n", (1000.0 * fps_frames / delta_t));
        fps_frames = 0;
        fps_start = fps_start + 1000;
    }

    //float cur_fade = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2 * 3.14) / 5) / 2 + 0.5; // 0->1->0 every 5 seconds
    //glUseProgram(program);
    //glUniform1f(uniform_fade, cur_fade);

    glutPostRedisplay();
}

/*
 * 
 */
int main(int argc, char** argv) {
   /*
        if (!GLEW_VERSION_2_0) {
            fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
            return 1;
        }
     */

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1000, 800);
    glutCreateWindow("My First Triangle");

    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return 1;
    }

    if (init_resources()) {
        glutDisplayFunc(onDisplay);
        glutIdleFunc(idle);
        
        glutMainLoop();
    }

    free_resources();

    return (EXIT_SUCCESS);
}


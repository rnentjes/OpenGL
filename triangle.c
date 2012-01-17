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

GLuint program;
GLint attribute_coord2d;
GLuint vbo_triangle;
GLint uniform_fade;

/* Global */
static unsigned int fps_start = 0;
static unsigned int fps_frames = 0;

int init_resources() {
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

    fps_start = glutGet(GLUT_ELAPSED_TIME);

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

    const char* uniform_name;
    uniform_name = "fade";
    uniform_fade = glGetUniformLocation(program, uniform_name);
    if (uniform_fade == -1) {
        fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
        return 0;
    }

    GLfloat triangle_vertices[] = {
        0.0, 0.8,
        -0.8, -0.8,
        0.8, -0.8,
    };

    glGenBuffers(1, &vbo_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof (triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord2d == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
        return 0;
    }

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
    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
    glEnableVertexAttribArray(attribute_coord2d);
    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
            attribute_coord2d, // attribute
            2, // number of elements per vertex, here (x,y)
            GL_FLOAT, // the type of each element
            GL_FALSE, // take our values as-is
            0, // no extra data between each position
            0 // offset of first element
            );

    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDisableVertexAttribArray(attribute_coord2d);

    /* Display the result */
    glutSwapBuffers();
}

void free_resources() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_triangle);
}

void idle() {
    fps_frames++;
    int delta_t = glutGet(GLUT_ELAPSED_TIME) - fps_start;
    if (delta_t > 1000) {
        fprintf(stdout, "FPS: %f\n", (1000.0 * fps_frames / delta_t));
        fps_frames = 0;
        fps_start = glutGet(GLUT_ELAPSED_TIME);
    }

    float cur_fade = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2 * 3.14) / 5) / 2 + 0.5; // 0->1->0 every 5 seconds
    glUseProgram(program);
    glUniform1f(uniform_fade, cur_fade);

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
    glutInitWindowSize(800, 600);
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


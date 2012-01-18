/* 
 * File:   shaderUtils.h
 * Author: rnentjes
 *
 * Created on January 17, 2012, 7:57 PM
 */

#ifndef SHADERUTILS_H
#define	SHADERUTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
GLuint make_buffer(
    GLenum target,
    const void *buffer_data,
    GLsizei buffer_size
);
GLuint make_texture(const char *filename);

#ifdef	__cplusplus
}
#endif

#endif	/* SHADERUTILS_H */


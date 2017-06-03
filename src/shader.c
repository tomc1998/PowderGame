#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "file.h"

int create_vf_shader_pair(GLuint* v_shader, GLuint* f_shader) {
  /* Create shaders */
  *v_shader = glCreateShader(GL_VERTEX_SHADER);
  if (!*v_shader) {
    printf("Error creating vertex shader.\n");
    return 1;
  }
  *f_shader = glCreateShader(GL_FRAGMENT_SHADER);
  if (!*f_shader) {
    printf("Error creating fragment shader.\n");
    glDeleteShader(*v_shader);
    return 1;
  }
  return 0;
}

GLuint load_shader_program_from_path(const char* v_path,
                                     const char* f_path) {
  char *v_src = NULL, *f_src = NULL;  /* GLSL source */
  long v_src_len, f_src_len;         /* Source len */
  int file_err;
  GLuint program;

  /* Load GLSL source into memory */
  file_err = load_from_path(v_path, &v_src, &v_src_len);
  if (file_err) {
    printf("Error loading vertex shader file: %d\n", file_err);
    if (v_src) {free(v_src);}
    return 0;
  }
  file_err = load_from_path(f_path, &f_src, &f_src_len);
  if (file_err) {
    printf("Error loading fragment shader file: %d\n", file_err);
    free(v_src);
    if (f_src) {free(f_src);}
    return 0;
  }

  /* Compile and link shader program using the src */
  program = load_shader_program_from_src(v_src, v_src_len,
                                      f_src, f_src_len);
  free(v_src);
  free(f_src);
  return program;
}

int compile_shader(const GLuint shader, const char* src, const long src_len) {
  GLint gl_err;
  /* Set shader source code */
  glShaderSource(shader, 1,
                 (const char* const*) &src,
                 (const int*) &src_len);

  /* Compile shaders */
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &gl_err);
  if (gl_err == GL_FALSE) {
    GLint info_log_len = 0;
    char* info_log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_len);
    info_log = (char*)malloc(info_log_len);
    glGetShaderInfoLog(shader, info_log_len, NULL, info_log);
    printf("\nError compiling shader:\n\n%s\n\n", info_log);
    free(info_log);
    return 1;
  }
  return 0;
}

GLuint link_vf_shaders(GLuint v_shader, GLuint f_shader) {
  GLint gl_err;
  GLuint program = glCreateProgram();
  if (!program) {
    printf("Error creating shader program.\n");
    return 0;
  }
  /* Link program */
  glAttachShader(program, v_shader);
  glAttachShader(program, f_shader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &gl_err);
  if (gl_err == GL_FALSE) {
    GLint info_log_len = 0;
    char* info_log;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_len);
    info_log = (char*)malloc(info_log_len);
    glGetProgramInfoLog(program, info_log_len, NULL, info_log);
    printf("\nError linking shader program :\n\n%s\n\n", info_log);
    free(info_log);
    glDeleteProgram(program);
    return 0;
  }
  glDetachShader(program, v_shader);
  glDetachShader(program, f_shader);
  return program;
}

GLuint load_shader_program_from_src(const char* v_src,
                                    const long v_src_len,
                                    const char* f_src,
                                    const long f_src_len) {
  GLuint v_shader, f_shader;         /* Shader IDs */
  GLuint program;                    /* Program ID */

  /* Create vert / frag shaders */
  if (create_vf_shader_pair(&v_shader, &f_shader)) {
    return 0;
  }

  /* Compile shaders */
  if (compile_shader(v_shader, v_src, v_src_len)) {
    printf("Preceding error in vertex shader.\n\n");
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
    return 0;
  }
  if (compile_shader(f_shader, f_src, f_src_len)) {
    printf("Preceding error in fragment shader.\n\n");
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
    return 0;
  }

  /* Link program */
  program = link_vf_shaders(v_shader, f_shader);
  if (!program) {
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
    return 0;
  }

  /* Clean up */
  glDeleteShader(v_shader);
  glDeleteShader(f_shader);

  return program;
}

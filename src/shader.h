/**
   @file shader.h
   @brief Functions to load, compile and link shaders.
*/

#ifndef SHADER_H
#define SHADER_H

/**
   @brief Loads GLSL source code from memory, then compiles & links
          into a shader program.
   @param v_path The path to the vertex shader source.
   @param f_path The path to the fragment shader source.
   @return The created shader ID, or 0 for an error (Most errors
   logged using printf).
*/
GLuint load_shader_program_from_path(const char* v_path,
                                     const char* f_path);

/**
   @brief Create a vertex shader and fragment shader.
   @param[out] v_shader The vertex shader ID destination.
   @param[in]  f_shader The fragment shader ID destination.
   @return 0 for success. Errors will be printed using printf.
   
   This method should probably never be used, and is only used as a
   convenience method for other methods in this file.
*/
int create_vf_shader_pair(GLuint* v_shader, GLuint* f_shader);

/**
   @brief Compiles a shader and logs any errors.
   @param[in] shader The shader to compile to.
   @param[in] src The GLSL source to compile.
   @param[in] src_len The length of the src.
   @return 0 for success. Errors will be printed using printf.
 */
int compile_shader(const GLuint shader, const char* src, const long src_len);

/**
   @brief Links a vertex and fragment shader to a program.
   @param[in] v_shader The vertex shader to link.
   @param[in] f_shader The fragment shader to link.
   @return The program ID. 0 for failure, Errors will be printed using printf.
*/
GLuint link_vf_shaders(GLuint v_shader, GLuint f_shader);

/**
   @brief Compiles & links a shader program given vert and frag shader sources.
   @param v_src The vertex shader source.
   @param v_src_len The vertex shader source length.
   @param f_src The fragment shader source.
   @param f_src_len The fragment shader source length.
   @return The created shader ID, or 0 for an error (Most errors
   logged using printf).
*/
GLuint load_shader_program_from_src(const char* v_src,
                                    const long v_src_len,
                                    const char* f_src,
                                    const long f_src_len);

#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

/**
   @fn GLFWwindow* init_glfw(const char* title, int screen_w, int screen_h)
   @brief Initialises a GLFWwindow and returns a pointer to the window struct.
   @param title The title of the window.
   @param screen_w The width of the window.
   @param screen_h The height of the window.
   @return A pointer to the window struct created from glfwCreateWindow().

   Makes the window's GL context current for the thread running this function.
*/
GLFWwindow* init_glfw(const char* title, int screen_w, int screen_h) {
  GLFWwindow* window = NULL;
  /* Init GLFW */
  glfwInit();
  /* Set GLFW to try and use OpenGL 3.3 */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  /* We need this! This will stop us using deprecated functionality, and the */
  /* mesa drivers on linux only support GL 3.3 core profile, not compatibility! */
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(screen_w, screen_h,
      title,
      NULL, NULL);
  /* Set current context to be the window we just created, allows GL functions */
  /* to affect this window's frame */
  glfwMakeContextCurrent(window);
  /* Load OpenGL extensions with GLAD */
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    printf("Can't load OpenGL extensions\n");
  }
  printf("OpenGL Ver - %d.%d\n", GLVersion.major, GLVersion.minor);

  return window;
}

int main(int argc, char** argv) {
  GLFWwindow* window = init_glfw("PowderGame", 800, 600);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  static const GLfloat vbo_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
  };

  // Generate a vbo
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // Buffer vbo_data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data), vbo_data, GL_STATIC_DRAW);

  // Create shader program
  GLuint shader_program = load_shader_program_from_path("vert.glsl",
      "frag.glsl");
  printf("Error number: %d, %s", errno, strerror(errno));
  glUseProgram(shader_program);

  while(1) {
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );

    // Draw the triangle !
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

    /* Flip buffers, poll events */
    glfwSwapBuffers(window);
    glfwPollEvents();

    /* If window should close, exit game loop */
    if (glfwWindowShouldClose(window)) { break; }
  }

  return 0;
}


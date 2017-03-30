#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opengl_wrappers.inl>
#include <math.inl>
#include <GuirApi.h>

void button_callback(GLFWwindow* win, int bt, int action, int mods);
void cursor_callback(GLFWwindow* win, double x, double y);
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* win, unsigned int key);
void error_callback(int err, const char* desc);
void resize_callback(GLFWwindow* window, int width, int height);

/* ------------------------------------------------ */

static const char* COLOR_VS = ""
  "#version 330\n"
  ""
  "uniform mat4 u_pm;"
  ""
  "layout (location = 0) in vec4 a_pos;"
  ""
  "void main() {"
  "  gl_Position = u_pm * a_pos;"
  "  gl_Position.z = 1.0;"
  "}"
  "";

static const char* COLOR_FS = ""
  "#version 330\n"
  ""
  "uniform vec4 u_color;"
  ""
  "layout (location = 0) out vec4 fragcolor;"
  ""
  "void main() {"
  "  fragcolor = u_color;" // vec4(0.8, 0.2, 0.2, 1.0);"
  "}"
  "";

/* ------------------------------------------------ */

int win_w = 720;
int win_h = 720;

/* ------------------------------------------------ */
  
int main() {

  glfwSetErrorCallback(error_callback);

  if(!glfwInit()) {
    printf("Error: cannot setup glfw.\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
  
  GLFWwindow* win = NULL;

  win = glfwCreateWindow(win_w, win_h, "Guir Visualisation", NULL, NULL);
  if (!win) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(win, resize_callback);
  glfwSetKeyCallback(win, key_callback);
  glfwSetCharCallback(win, char_callback);
  glfwSetCursorPosCallback(win, cursor_callback);
  glfwSetMouseButtonCallback(win, button_callback);
  glfwMakeContextCurrent(win);
  glfwSwapInterval(1);

  if (!gladLoadGL()) {
    printf("Cannot load GL.\n");
    exit(1);
  }

  // ----------------------------------------------------------------
  // THIS IS WHERE YOU START CALLING OPENGL FUNCTIONS, NOT EARLIER!!
  // ----------------------------------------------------------------

  /* Remote Guir setup */
  GuirClient* guir = NULL;
  if (0 != guir_client_alloc(&guir)) {
    printf("Failed to allocate the guir client context. (exiting). \n");
    exit(EXIT_FAILURE);
  }

  if (0 != guir_client_init(guir, "127.0.0.1", 1234)) {
    printf("Failed to initialize the GuirClient. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  /* Values controlled by the Guir app. */
  int segments = 60;
  float radius = 300.0f;
  float color[4] = { 0.8, 0.2, 0.2, 1.0 };
  if (0 != guir_client_add_value_listener_for_sliderint(guir, "circle-segments", segments)) {
    printf("Failed to add the segments as value listener. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  if (0 != guir_client_add_value_listener_for_sliderfloat(guir, "circle-radius", radius)) {
    printf("Failed to add the circle radius as value listener. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  if (0 != guir_client_add_value_listener_for_colorpicker(guir, "circle-color", color)) {
    printf("Failed to add the circle color value listener. (exiting). \n");
    exit(EXIT_FAILURE);
  }

  /* When we've added some value listeners, we can load the saved values. */
  if (0 != guir_client_load_values(guir)) {
    printf("Failed to load previously saved settings. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  /* OpenGL setup */
  GLuint frag = 0;
  GLuint vert = 0;
  GLuint prog = 0;
  GLint u_pm = 0;
  GLint u_color = 0;

  if (0 != create_shader(&vert, GL_VERTEX_SHADER, COLOR_VS)) {
    printf("Failed to create vertex shader (exiting). \n");
    exit(EXIT_FAILURE);
  }

  if (0 != create_shader(&frag, GL_FRAGMENT_SHADER, COLOR_FS)) {
    printf("Failed to create fragment shader (exiting). \n");
    exit(EXIT_FAILURE);
  }

  if (0 != create_program(&prog, vert, frag, 1)) {
    printf("Failed to create the program. (exiting). \n");
    exit(EXIT_FAILURE);
  }

  glUseProgram(prog);
  
  u_pm = glGetUniformLocation(prog, "u_pm");
  if (-1 == u_pm) {
    printf("u_pm is -1, not used in shader? (exiting). \n");
    exit(EXIT_FAILURE);
  }
  
  u_color = glGetUniformLocation(prog, "u_color");
  if (-1 == u_color) {
    printf("u_color is -1, not used in shader? (exiting). \n");
    exit(EXIT_FAILURE);
  }

  float pm[16] = { 0.0f };
  create_ortho_matrix(0, win_w, win_h, 0, 0.01f, 100.0f, pm);
  glUniformMatrix4fv(u_pm, 1, GL_FALSE, pm);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  size_t nbytes_allocated = 1024 * 1024;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, nbytes_allocated, NULL, GL_STREAM_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
  
  std::vector<float> vertices;
  float center_x = win_w * 0.5;
  float center_y = win_h * 0.5;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  while(!glfwWindowShouldClose(win)) {

    glViewport(0, 0, win_w, win_h);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (0 != guir_client_update(guir)) {
      printf("Failed to update guir (exiting).\n");
      exit(EXIT_FAILURE);
    }

    /* Create a circle. */
    float angle_step = (3.1415 * 2.0f) / segments;
    float angle = 0.0f;
    vertices.clear();
    vertices.push_back(center_x);
    vertices.push_back(center_y);
    for (int i = 0; i <= segments; ++i) {
      vertices.push_back(center_x + cos(angle) * radius);
      vertices.push_back(center_y + sin(angle) * radius);
      angle += angle_step;
    }
    
    /* Update buffer (grow if needed */
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    size_t nbytes_needed = vertices.size() * sizeof(float);
    if (nbytes_needed > nbytes_allocated) {
      glBufferData(GL_ARRAY_BUFFER, nbytes_needed, NULL, GL_STREAM_DRAW);
      nbytes_allocated = nbytes_needed;
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, nbytes_needed, (const void*)&vertices[0]);  
    
    /* Draw */
    glUseProgram(prog);
    glUniform4fv(u_color, 1, color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}

void char_callback(GLFWwindow* win, unsigned int key) {
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  if (GLFW_PRESS == action) {
    switch(key) {
      case GLFW_KEY_ESCAPE: {
        glfwSetWindowShouldClose(win, GL_TRUE);
        break;
      }
    };
  }
}

void resize_callback(GLFWwindow* window, int width, int height) {
}

void cursor_callback(GLFWwindow* win, double x, double y) {
}

void button_callback(GLFWwindow* win, int bt, int action, int mods) {
}

void error_callback(int err, const char* desc) {
  printf("GLFW error: %s (%d)\n", desc, err);
}


/* ------------------------------------------------ */


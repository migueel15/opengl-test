#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glm/glm.hpp>
#include <iostream>

const float VELOCITY = 0.4f;

struct Vertex {
  float x;
  float y;
  glm::vec3 color;
  float type;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void check_exit(GLFWwindow *window);
void move(GLFWwindow *window, unsigned int VBO, Vertex *positions, int size);

unsigned int CreateProgramShader() {
  const char *vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec2 pos;\n"
                                   "layout (location = 1) in vec3 color;\n"
                                   "layout (location = 2) in float type;\n"
                                   "out vec2 fPos;\n"
                                   "out vec3 fColor;\n"
                                   "out float fType;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   gl_Position = vec4(pos,0.0,1.0);\n"
                                   "   fColor = color;\n"
                                   "   fPos = pos;\n"
                                   "   fType = type;\n"
                                   "}\0";

  const char *fragmentShaderSource = "#version 330 core\n"
                                     "in vec2 fPos;\n"
                                     "in vec3 fColor;\n"
                                     "in float fType;\n"
                                     "out vec4 FragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   float r = fPos.x;\n"
                                     "   float g = fPos.y;\n"
                                     "   float b = 1.0/fType;\n"
                                     "   FragColor = vec4(r,g,b, 0.0f);\n"
                                     "}\0";

  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // se crea el programa del shader y se vinculan
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // una vez vinculados al programa ya podemos borrarlos
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

int main(int argc, char *argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Juego", NULL, NULL);

  if (window == NULL) {
    std::cout << "Error al crear la ventana" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // vincula el viewport al size de la ventana
  glViewport(0, 0, 800, 600);
  // modifica el viewport cuando se cambia el size de la ventana
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Vertex a = {-0.1f, -0.1f, glm::vec3(1.0f, 0.0f, 0.0f), 1.0};
  Vertex b = {0.0f, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f), 1.0};
  Vertex c = {0.1f, -0.1f, glm::vec3(0.0f, 0.0f, 1.0f), 1.0};
  Vertex d = {-0.2f, -0.2f, glm::vec3(1.0f, 0.0f, 0.0f), 2.0};
  Vertex e = {0.1f, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), 2.0};
  Vertex f = {-0.4f, 0.3f, glm::vec3(0.0f, 0.0f, 1.0f), 2.0};

  Vertex positions[] = {a, b, c, d, e, f};

  unsigned int VBO, VAO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, positions, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, x));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, type));
  glEnableVertexAttribArray(2);

  unsigned int shaderProgram = CreateProgramShader();

  while (!glfwWindowShouldClose(window)) {
    check_exit(window);

    glClearColor(30.0 / 255.0, 30.0 / 255.0, 46.0 / 255.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    move(window, VBO, positions, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void check_exit(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void move(GLFWwindow *window, unsigned int VBO, Vertex *positions, int size) {
  float deltaTime = glfwGetTime();
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    for (int i = 0; i < size; i++) {
      positions[i].y += VELOCITY * deltaTime;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    for (int i = 0; i < size; i++) {
      positions[i].y -= VELOCITY * deltaTime;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    for (int i = 0; i < size; i++) {
      positions[i].x += VELOCITY * deltaTime;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    for (int i = 0; i < size; i++) {
      positions[i].x -= VELOCITY * deltaTime;
    }
  }
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(Vertex), positions);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glfwSetTime(0.0);
}

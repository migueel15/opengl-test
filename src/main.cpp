#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void check_exit(GLFWwindow *window);

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

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

  // se buildean y compilan los shaders
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

  // triangle (x,y,z) por cada vertice (posiciones normalizadas: -1 a 1)
  float vertices[] = {
      0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f, -0.5f, 0.5f,  0.0f,
      0.5f,  -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f,
      1.0f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f,
      -1.0f, -1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,  -1.0f, 0.0f,

  };

  unsigned int VBO, VAO;
  // se crea el buffer de vertices y se asigna
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // se crea el array de vertices y se asigna
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // se copian los vertices al buffer de vertices
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // definir como debe interpretar el array de vertices
  // 0: el atributo que queremos configurar
  // 3: tamaño del atributo
  // GL_FLOAT: tipo de dato
  // GL_FALSE: si queremos que se normalice (como ya esta normalizado los
  // valores de los vertices no es necesario)
  // 3 * sizeof(float): tamaño del stride (espacio entre atributos) (3
  // posiciones de float) (void*)0: offset del atributo (posicion del primer
  // atributo). Como empezamos en 0 y requiere un puntero, se pasa void* 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  // ya hemos definido como debe interpretar el array de vertices, ahora solo
  // hay que habilitarlo
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    check_exit(window);

    glClearColor(0.2f, 0.3f, 0.3f, 0.2f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));

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

#include <engine.hpp>



static GLuint
load_texture(const char* path, int type);



Engine::Engine(int windowWidth, int windowHeight) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  this->window = glfwCreateWindow(
    windowWidth, windowHeight, "konina", NULL, NULL
  );

  if(this->window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(1);
  }

  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(1);
  }

  glViewport(0, 0, windowWidth, windowHeight);
}

Engine::~Engine() {
  glfwTerminate();
}

void Engine::clear() {
  glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::flush() {
  glfwSwapBuffers(window);
}

bool Engine::running() {
  glfwPollEvents();
  
  return !glfwWindowShouldClose(this->window);
}

void Engine::add_glfw_key_callback(void(*callback)(GLFWwindow* window, int key, int scancode, int action, int mods)) {
  glfwSetKeyCallback(this->window, callback);
}

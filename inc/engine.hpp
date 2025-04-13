#pragma once

#include <iostream>
#include <vector>

#include <glad.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <stb_image_impl.hpp>

#include <shader.hpp>
#include <vertex.hpp>
#include <canvas.hpp>



class Engine {
  private:
    GLFWwindow* window;

  public:
    Engine(int windowWidth, int windowHeight);
    ~Engine();
    
    void clear();
    void flush();
    bool running();

    void add_glfw_key_callback(void(*callback)(GLFWwindow* window, int key, int scancode, int action, int mods));
};

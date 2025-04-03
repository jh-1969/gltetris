#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glad.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



class Shader {
  private:
    GLuint id;

  public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use();
    void set_mat4(const char* name, glm::mat4 &mat);
    void set_mat3(const char* name, glm::mat3 &mat);
    void set_mat3_arr(const char* name, std::vector<glm::mat3> &mat, int count);
    void set_vec3(const char* name, glm::vec3 &vec);
    void set_vec3_arr(const char* name, std::vector<glm::vec3> &vec, int count);
    void set_int(const char* name, int x);
    void set_uint_arr(const char* name, std::vector<GLuint> &vec, int count);
    void set_int_arr(const char* name, std::vector<GLint> &vec, int count);
};

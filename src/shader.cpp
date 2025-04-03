#include <shader.hpp>



static GLuint
read_and_compile_shader(const char* filename, int type);



Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  GLuint vertex = read_and_compile_shader(
      vertexPath, GL_VERTEX_SHADER);
  GLuint fragment = read_and_compile_shader(
      fragmentPath, GL_FRAGMENT_SHADER);

  this->id = glCreateProgram();
  glAttachShader(this->id, vertex);
  glAttachShader(this->id, fragment);
  glLinkProgram(this->id);
  
  int success = false;
  char infoLog[512];

  glGetProgramiv(this->id, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(this->id, 512, NULL, infoLog);
    std::cout << "Shader program error\n" << infoLog << std::endl;
    exit(1);
  }
    
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader() {
  glLinkProgram(0);
  glDeleteProgram(this->id);
}

void Shader::use() {
  glUseProgram(this->id);
}

void Shader::set_mat4(const char* name, glm::mat4 &mat) {
  this->use();
  int location = glGetUniformLocation(this->id, name);

  if(location == -1)
    std::cout << "No uniform in shader program named " << name << std::endl;
  else
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::set_mat3(const char* name, glm::mat3 &mat) {
  this->use();
  int location = glGetUniformLocation(this->id, name);

  if(location == -1)
    std::cout << "No uniform in shader program named " << name << std::endl;
  else
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::set_mat3_arr(const char* name, std::vector<glm::mat3> &mat, int count) {
  this->use();
  int location = glGetUniformLocation(this->id, name);

  if(location == -1)
    std::cout << "No uniform in shader program named " << name << std::endl;
  else
    glUniformMatrix3fv(location, count, GL_FALSE, glm::value_ptr(mat[0]));
}

void Shader::set_vec3(const char* name, glm::vec3 &vec) {
  this->use();
  int location = glGetUniformLocation(this->id, name);

  if(location == -1)
    std::cout << "No uniform in shader program named " << name << std::endl;
  else
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void Shader::set_vec3_arr(const char* name, std::vector<glm::vec3> &vec, int count) {
  this->use();
  int location = glGetUniformLocation(this->id, name);

  if(location == -1)
    std::cout << "No uniform in shader program named " << name << std::endl;
  else
    glUniform3fv(location, count, glm::value_ptr(vec[0]));
}

void Shader::set_int(const char* name, int x) {
  this->use();
  int location = glGetUniformLocation(this->id, name);

  if(location == -1)
    std::cout << "No uniform in shader program named " << name << std::endl;
  else
    glUniform1i(location, x);
}

void Shader::set_uint_arr(const char* name, std::vector<GLuint> &vec, int count) {
  this->use();
  int location = glGetUniformLocation(this->id, name);

  if(location == -1)
    std::cout << "No uniform in shader program named " << name << std::endl;
  else
    glUniform1uiv(location, count, &vec[0]);
}

void Shader::set_int_arr(const char* name, std::vector<GLint> &vec, int count) {
  this->use();
  int location = glGetUniformLocation(this->id, name);

  if(location == -1)
    std::cout << "No uniform in shader program named " << name << std::endl;
  else
    glUniform1iv(location, count, &vec[0]);
}



static GLuint
read_and_compile_shader(const char* filename, int type) {
  std::string code;
  std::ifstream file;
  
  file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  
  try {
    file.open(filename);
    std::stringstream stream;
    stream << file.rdbuf();

    file.close();
    code = stream.str();
  }
  catch(std::ifstream::failure e) {
    std::cout << "Shader file not found: " << filename << std::endl;
    exit(1);
  }
  
  const char* charCode = code.c_str();

  GLuint shader;
  int success;
  char infoLog[512];
     
  shader = glCreateShader(type);
  glShaderSource(shader, 1, &charCode, NULL);
  glCompileShader(shader);
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);

    switch(type) {
      case GL_VERTEX_SHADER:
      std::cout << "Vertex shader compilation error\n" << infoLog << std::endl;
      break;

      case GL_FRAGMENT_SHADER:
      std::cout << "Fragment shader compilation error\n" << infoLog << std::endl;
      break;
    }
    exit(1);
  };

  return shader;
}


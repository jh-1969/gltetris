#pragma once

#include <vector>
#include <iostream>

#include <glad.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <vertex.hpp>
#include <shader.hpp>
#include <texturelist.hpp>



class Canvas {
  private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    std::vector<glm::mat3> models;
    std::vector<glm::vec3> colors;
    std::vector<GLuint> texIndexes;

    Shader* shader; //not initilized by this class
    TextureList* texturelist; //not initilized by this class

    void create_buffer_objects();

  public:
    Canvas(Shader* shader, TextureList* texturelist);
    ~Canvas();
    
    void draw_sprite(int x, int y, int w, int h, float rot, glm::vec3 color, int texIndex);
    void draw();
};

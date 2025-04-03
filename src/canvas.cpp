#include <canvas.hpp>



const Vertex quadVertices[] = {{
    glm::vec2(-1, -1),
    glm::vec2(0, 0),
  },{
    glm::vec2(1, -1),
    glm::vec2(1, 0),
  },{
    glm::vec2(-1, 1),
    glm::vec2(0, 1),
  },{
    glm::vec2(1, 1),
    glm::vec2(1, 1),
}};

const GLuint quadIndices[] = {0, 1, 3, 0, 3, 2};



Canvas::Canvas(Shader* shader, TextureList* texturelist) {
  this->shader = shader;
  this->texturelist = texturelist;
  this->create_buffer_objects();

  if(this->shader)
    this->shader->use();
  else {
    std::cout << "Shader not initilized" << std::endl;
    exit(1);
  }

  if(!this->texturelist) {
    std::cout << "TextureList not initilized" << std::endl;
    exit(1);
  }
}

Canvas::~Canvas() {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDeleteVertexArrays(1, &this->vao);
  glDeleteBuffers(1, &this->vbo);
  glDeleteBuffers(1, &this->ebo);
}

void Canvas::draw_sprite(int x, int y, int w, int h, float rot, glm::vec3 color, int texIndex) {
  float fw = w / 800.0;
  float fh = h / 800.0;
  
  float fx = 2.0 * (x / 800.0) - 1.0;
  float fy = -2.0 * (y / 800.0) + 1.0;

  glm::mat3 transform = glm::mat3(1);
  transform = glm::translate(transform, glm::vec2(fx + fw, fy - fh));
  transform = glm::scale(transform, glm::vec2(fw, fh));
  transform = glm::rotate(transform, glm::radians(rot));
  
  this->models.push_back(transform);
  this->colors.push_back(color);
  this->texIndexes.push_back(texIndex);
}

void Canvas::draw() {
  this->shader->use();
  this->shader->set_mat3_arr("models", this->models, this->models.size());
  this->shader->set_vec3_arr("colors", this->colors, this->colors.size());
  this->shader->set_uint_arr("texIndexes", this->texIndexes, this->texIndexes.size());

  this->texturelist->bind();

  glBindVertexArray(this->vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, this->models.size());

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  this->texturelist->unbind();
  
  this->models.clear();
  this->colors.clear();
  this->texIndexes.clear();
}

void Canvas::create_buffer_objects() {
  glGenVertexArrays(1, &this->vao);
  glGenBuffers(1, &this->vbo);
  glGenBuffers(1, &this->ebo);

  glBindVertexArray(this->vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  
  glBufferData(GL_ARRAY_BUFFER,
    4 * sizeof(Vertex),
    &quadVertices,
    GL_STATIC_DRAW
  );

  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    6 * sizeof(GLuint),
    &quadIndices,
    GL_STATIC_DRAW
  );

  glEnableVertexAttribArray(0);	
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(1);	
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
  
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

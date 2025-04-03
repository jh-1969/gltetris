#pragma once

#include <iostream>
#include <vector>

#include <stb_image_impl.hpp>
#include <glad.hpp>



class TextureList {
  private:
    std::vector<GLuint> textures;
    std::vector<GLuint64> handles;

    GLuint ssbo;
  
  public:
    ~TextureList();

    GLuint add(const char* path, int type); //returns index of texture
                                            //used when drawing a sprite
    void bind();
    void unbind();
    void create_ssbo();
};

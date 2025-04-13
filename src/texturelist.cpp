#include <texturelist.hpp>

TextureList::~TextureList() {
  for(int i = 0; i < this->textures.size(); i++) {
    glDeleteTextures(1, &this->textures[i]);
  }
  glDeleteBuffers(1, &ssbo);
}

GLuint TextureList::add(const char* path, bool transparent) {
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
  
  if(!data) {
    std::cout << "failed to load texture" << std::endl;
    exit(1);
  }

  GLuint texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);
  glTextureStorage2D(texture, 1, transparent ? GL_RGBA8 : GL_RGB8, width, height);
  glTextureSubImage2D(
    texture, 
    0, 0, 0, width, height, 
    transparent ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, 
    (const void *)&data[0]);
  glGenerateTextureMipmap(texture);

  const GLuint64 handle = glGetTextureHandleARB(texture);
  if (handle == 0) {
      std::cerr << "Error! Handle returned null" << std::endl;
      exit(-1);
  }

  free(data);

  int index = this->textures.size();
  this->textures.push_back(texture);
  this->handles.push_back(handle);
 
  return index;
}

void TextureList::create_ssbo() {
  glCreateBuffers(1, &this->ssbo);
  glNamedBufferStorage(
    this->ssbo,
    sizeof(GLuint64) * this->handles.size(),
    (const void *)this->handles.data(),
    GL_DYNAMIC_STORAGE_BIT);
}

void TextureList::bind() {
  for(GLuint64 handle : this->handles) {
    glMakeTextureHandleResidentARB(handle);
  }
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->ssbo);
}

void TextureList::unbind() {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

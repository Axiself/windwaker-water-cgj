////////////////////////////////////////////////////////////////////////////////
//
// Textures
//
// Copyright (c) 2022-24 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include "./mglTexture.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"
#include "mglSampler.hpp"

namespace mgl {

//////////////////////////////////////////////////////////////////// TextureInfo

TextureInfo::TextureInfo(GLenum _unit, GLuint _index,
                         const std::string &_uniform, Texture *_texture,
                         Sampler *_sampler) {
  unit = _unit;
  uniform = _uniform;
  index = _index;
  texture = _texture;
  sampler = _sampler;
}

void TextureInfo::updateShader(ShaderProgram *shader) {
  glActiveTexture(unit);
  texture->bind();
  if (sampler)
    sampler->bind(index);
  glUniform1i(shader->Uniforms[uniform].index, index);
}

//////////////////////////////////////////////////////////////////////// Texture

Texture::Texture() : id(-1) {}

Texture::~Texture() { glDeleteTextures(1, &id); }

///////////////////////////////////////////////////////////////// TextureCubeMap

void TextureCubeMap::bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }

void TextureCubeMap::unbind() { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

struct CubemapParams {
  const GLenum name;
  const GLint value;
};
static const CubemapParams CUBEMAP_PARAMS[] = {
    {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
    {GL_TEXTURE_MIN_FILTER, GL_LINEAR},
    {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
    {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
    {GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE}};

struct CubemapData {
  const char *suffix;
  const GLenum target;
};
static const CubemapData CUBEMAP_TEXTURES[] = {
    {"right", GL_TEXTURE_CUBE_MAP_POSITIVE_X},
    {"left", GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
    {"top", GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
    {"bottom", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
    {"front", GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
    {"back", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z}};

void TextureCubeMap::loadCubeMap(const std::string &prefix,
                                 const std::string &suffix) {
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, id);

  for (const auto &p : CUBEMAP_PARAMS) {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, p.name, p.value);
  }

  for (const auto &t : CUBEMAP_TEXTURES) {
    std::stringstream ss;
    ss << prefix << t.suffix << suffix;
    std::string filename = ss.str();

    int width, height, channels;
    std::cout << "Loading cubemap file " << filename << "... ";
    unsigned char *image =
        stbi_load(filename.c_str(), &width, &height, &channels, 0);
    assert(channels == 4);
    if (image == nullptr) {
      std::cout << "error." << std::endl;
      exit(EXIT_FAILURE);
    } else {
      std::cout << "ok. " << std::endl;
    }
    glTexImage2D(t.target, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
  }
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

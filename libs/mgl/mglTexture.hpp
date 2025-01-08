////////////////////////////////////////////////////////////////////////////////
//
// Textures
//
// Copyright (c) 2022-24 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_TEXTURE_HPP
#define MGL_TEXTURE_HPP

#include <string>

#include "./mglSampler.hpp"
#include "./mglShader.hpp"

namespace mgl {

class Texture;
class TextureCubeMap;
struct TextureInfo;

//////////////////////////////////////////////////////////////////////// TEXTURE

class Texture {
protected:
  GLuint id;

public:
  Texture();
  virtual ~Texture();
  virtual void bind() = 0;
  virtual void unbind() = 0;
};

//////////////////////////////////////////////////////////////////// TextureInfo

struct TextureInfo {
  GLenum unit;                // GL_TEXTUREi
  GLuint index;               // sampler index in shader
  std::string uniform;        // uniform name in shader
  Texture *texture = nullptr; // Texture (engine object)
  Sampler *sampler = nullptr; // Sampler (engine object)

  TextureInfo(GLenum textureunit, GLuint index, const std::string &uniform,
              Texture *texture, Sampler *sampler);
  void updateShader(ShaderProgram *shader);
};

//////////////////////////////////////////////////////////////////////// CUBEMAP

class TextureCubeMap : public Texture {
public:
  void bind() override;
  void unbind() override;
  void loadCubeMap(const std::string &prefix, const std::string &suffix);
};

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_TEXTURE_HPP */

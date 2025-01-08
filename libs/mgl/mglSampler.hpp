////////////////////////////////////////////////////////////////////////////////
//
// Texture Sampler
//
// Copyright (c)2022-24 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SAMPLER_HPP
#define MGL_SAMPLER_HPP

#include <GL/glew.h>

namespace mgl {

class Sampler;
class NearestSampler;
class LinearSampler;
class NearestMimapNearestSampler;
class NearestMimapLinearSampler;
class LinearMimapNearestSampler;
class LinearMimapLinearSampler;
class LinearAnisotropicSampler;
class MaxAnisotropicSampler;

////////////////////////////////////////////////////////////////////////////////

class Sampler {
protected:
  GLuint _samplerId;

public:
  Sampler();
  virtual ~Sampler();
  virtual void create() = 0;
  void bind(GLuint unit);
  void unbind(GLuint unit);

  // Delete copy constructor and copy assignment operator
  Sampler(const Sampler &) = delete;
  Sampler &operator=(const Sampler &) = delete;
  // Allow move constructor and move assignment
  Sampler(Sampler &&other) noexcept;
  Sampler &operator=(Sampler &&other) noexcept;
};

class NearestSampler : public Sampler {
public:
  void create() override;
};

class LinearSampler : public Sampler {
public:
  void create() override;
};

class NearestMimapNearestSampler : public Sampler {
public:
  void create() override;
};

class NearestMimapLinearSampler : public Sampler {
public:
  void create() override;
};

class LinearMimapNearestSampler : public Sampler {
public:
  void create() override;
};

class LinearMimapLinearSampler : public Sampler {
public:
  void create() override;
};

class LinearAnisotropicSampler : public Sampler {
public:
  float Anisotropy = 4.0f;
  void create() override;
};

class MaxAnisotropicSampler : public Sampler {
public:
  void create() override;
};

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_SAMPLER_HPP */

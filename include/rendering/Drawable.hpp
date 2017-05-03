#ifndef RENDERING_DRAWABLE_HPP
#define RENDERING_DRAWABLE_HPP

#include <GL/glew.h>
#include "hummingbird/hum.hpp"
#include "ShaderProgram.hpp"

namespace rendering
{
class Drawable : public hum::Behavior
{
public:
    Drawable();
    virtual ~Drawable();

    virtual void init() override;
    virtual void onActivate() override;
    virtual void onDeactivate() override;
    virtual void onDestroy() override;

    /*!
      \brief Enable the drawable.

      A Drawable is enabled by default. If enabled it will be drawn.
    */
    void enable();

    /*!
      \brief Disable the drawable.

      A Drawable is enabled by default. If disabled it won't be drawn.
    */
    void disable();

    /*!
      \brief Query if the Drawable is enabled.

      \return Whether the Drawable is enabled or not.
    */
    bool isEnabled() const;

    /*!
      \brief Get a reference to the Drawable's hum::Transformation.

      This hum::Transformation is relative to the Drawable's hum::Actor.

      \return Drawable's hum::Transformation..
    */
    hum::Transformation& transform();

    /*!
      \brief Get a constant reference to the Drawable's hum::Transformation.

      This hum::Transformation is relative to the Drawable's hum::Actor.

      \return Drawable's hum::Transformation..
    */
    const hum::Transformation& transform() const;

    /*!
      \brief Set the ShaderProgram to use with the Drawable.

      The Drawable doesn't handle the given pointer and the pointed object must
      exists while the Drawable is using it.
    */
    virtual void setShaderProgram(ShaderProgram* shader_program);

    /*!
      \brief Get the ShaderProgram being used by the Drawable.
    */
    const ShaderProgram* shaderProgram() const;

    /*!
      \brief Get the ShaderProgram being used by the Drawable.
    */
    ShaderProgram* shaderProgram();

    /*!
      \brief Draw the Drawable to the active OpenGL context.

      This abstract method is to be implemented by derived classes to define
      how the Drawable must be drawn.

      By the point where this method is called, the ShaderProgram of the Drawable is being
      used and the _model_, _view_ and _projection_ uniform matrices are set.
     */
    virtual void draw() =0;

    /*!
      \brief Set the origin (center) of the Drawable.
     */
    void setOrigin(const hum::Vector3f& origin);

    /*!
      \brief Get the origin (center) of the Drawable.
     */
    const hum::Vector3f& getOrigin() const;

    static const char* behaviorName();

private:
    bool _is_enabled;
    hum::Transformation _transform;
    hum::Vector3f _origin;
    ShaderProgram* _shader_program;
};

/*!
  \class rendering::Drawable
  \brief Abstract class to implement a hum::Behavior that can be drawn.

  Drawable is a base class that implements the basic functionalities for drawing
  geometry into OpenGL. These functionalities include a hum::Transformation that
  id relative to its hum::Actor one; An origin for defining the center; shaderProgram
  setter and getters; and the ability to disable and enable it.

  Example:
  \code
  class MyDrawable : public mogl::Drawable
  {
  public:
    // ...

    // Overwrite setShaderProgram() to bind the vertex attribute
    void Rectangle::setShaderProgram(ShaderProgram* shader_program)
    {
      Drawable::setShaderProgram(shader_program);
      glBindVertexArray(s_VAO);
      glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
      p_position_loc = shaderProgram()->bindVertexAttribute("position", 2, 0, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

    // When this is called the shader is already in use and the model, view and
    // projection matrices are set.
    virtual void draw()
    {
      glBindVertexArray(s_VAO);
      shaderProgram()->setUniform4f("color",
              static_cast<float>(p_color.r)/255.0f,
              static_cast<float>(p_color.g)/255.0f,
              static_cast<float>(p_color.b)/255.0f,
              static_cast<float>(p_color.a)/255.0f);
      glEnableVertexAttribArray(p_position_loc);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }
  \endcode

  Shaders for Drawable%s must have the following uniforms defined:
  \li mat4 projection
  \li mat4 view
  \li mat4 model

  Which are the standard matrices to render.
*/
} /* rendering */
#endif /* RENDERING_DRAWABLE_HPP */

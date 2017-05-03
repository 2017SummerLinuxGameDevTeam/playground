#ifndef RENDERING_SHADER_PROGRAM_INCLUDE_HPP
#define RENDERING_SHADER_PROGRAM_INCLUDE_HPP

#include <GL/glew.h>
#include "glm.hpp"
#include "Shader.hpp"

namespace rendering
{
class ShaderProgram
{

public:
    //! Class constructor
    ShaderProgram();

    //! Class destructor
    ~ShaderProgram();

    /*!
      \brief Add a Shader to the ShaderProgram

      \return A pointer to itself.
     */
    ShaderProgram* addShader(const Shader& shader);

    /*!
      \brief Bind the output of the fragment shader

      \return A pointer to itself.
     */
    ShaderProgram* bindFragmentOutput(const std::string& output_name);

    /*!
      \brief Bind a vertex attribute to the ShaderProgram

      \return A pointer to itself.
     */
    GLint bindVertexAttribute(const std::string& attrib_name, GLint size, GLsizei stride, GLvoid* first_pointer);

    /*!
      \brief Link the various Shaders added into the ShaderProgram

      \return A pointer to itself.
     */
    ShaderProgram* link();

    /*!
      \brief Set the ShaderProgram as the one currently being used

      \return A pointer to itself.
     */
    ShaderProgram* use();

    /*!
      \brief Pass a `vec2` uniform wth name <uniform_name> to the associated shaders.

      \return A pointer to itself.
     */
    ShaderProgram* setUniform2f(const std::string& uniform_name, float v0, float v1);

    /*!
      \brief Pass a `vec3` uniform wth name <uniform_name> to the associated shaders.

      \return A pointer to itself.
     */
    ShaderProgram* setUniform3f(const std::string& uniform_name, float v0, float v1, float v2);

    /*!
      \brief Pass a `vec4` uniform wth name <uniform_name> to the associated shaders.

      \return A pointer to itself.
     */
    ShaderProgram* setUniform4f(const std::string& uniform_name, float v0, float v1, float v2, float v3);

    /*!
      \brief Pass a `mat4` uniform wth name <uniform_name> to the associated shaders.

      \return A pointer to itself.
     */
    ShaderProgram* setUniformMatrix4f(const std::string& uniform_name, const glm::mat4& mat);

    /*!
      \brief Get whether the ShaderProgram was able to link all the associated
      Shader%s.

      If this method returns `false` check log() to get the error.

      \return if the ShaderProgram linked.
     */
    bool isLinked();

    //! Get the error log of the shader code compilation.
    const std::string& log() const;

private:
    ShaderProgram(const ShaderProgram&) =delete;
    ShaderProgram& operator=(const ShaderProgram&) =delete;

    GLuint _program_id;
    bool _linked;
    std::string _error_log;

};

/*!
  \class rendering::ShaderProgram
  \brief Class to load, handle and work with shader programs.

  A ShaderProgram requires two Shader%s to be added (addShader()) to it before
  linking, one of Shader::Type VERTEX_SHADER and one of Shader::Type FRAGMENT_SHADER.

  Example:
  \code
  // Create shaders
  rendering::Shader vs, fs;

  // Load vertex shader from file
  vs.loadFromFile(Shader::Type::VERTEX_SHADER, "shader.vert");
  if(!vs.isCompiled())
  {
    hum::log_e("Vertex shader failed to compile: ", vs.log());
    return 1;
  }

  // Load fragment shader from file
  fs.loadFromFile(Shader::Type::FRAGMENT_SHADER, "shader.frag");
  if(!fs.isCompiled())
  {
    hum::log_e("Fragment shader failed to compile: ", fs.log());
    return 1;
  }

  // Create shader program
  rendering::ShaderProgram prog;

  // Add shaders and link
  prog.addShader(vs)
     ->addShader(fs)
     ->link();

  // Check for errors
  if (!prog.isLinked())
  {
    hum::log_e("Shader program failed to link: ", prog.log());
    return 1;
  }

  //...

  prog.use();
  prog.setUniform4f("color", 0.f, 1.f, 1.f, 1.f);
  //...
  \endcode

  Shaders for Drawable%s must have the following uniforms defined:
  \li mat4 projection
  \li mat4 view
  \li mat4 model

  Which are the standard matrices to render.
*/
}
#endif

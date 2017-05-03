#ifndef RENDERING_SHADER_INCLUDE_HPP
#define RENDERING_SHADER_INCLUDE_HPP

#include <string>
#include <GL/glew.h>

namespace rendering
{
class Shader
{
public:
    /*!
      \enum Type
      \brief Type of a Shader.
     */
    enum class Type { VERTEX_SHADER, FRAGMENT_SHADER };

    //! Class constructor
    Shader();

    //! Class destructor
    ~Shader();

    /*!
      \brief Load a shader of Shader::Type <type> from a `std::string` containing the source.

      This method must be called with an active OpenGL context.
     */
    void loadFromSource(const Type type, const std::string& source);

    /*!
      \brief Load a shader of Shader::Type <type> from the file <filename>

      This method must be called with an active OpenGL context.

      \return Whether there was and error reading the file.
     */
    bool loadFromFile(const Type type, const std::string& filename);

    //! Get the native handler of the shader.
    GLuint getId() const;

    /*!
      \brief Get whether the Shader code compiled after loading it.

      If this method returns `false` check log() to get the error.

      \return if the shader code compiled.
     */
    bool isCompiled() const;

    //! Get the error log of the shader code compilation.
    const std::string& log() const;

private:
    Shader(const Shader&) =delete;
    Shader& operator=(const Shader&) = delete;

    bool loadShaderSource(const std::string& filename, std::string& shader_source);

private:
    GLuint _shader_id;
    bool _compiled;
    std::string _error_log;

};

/*!
  \class rendering::Shader
  \brief Class for loading a Vertex Shader or a Fragment Shader.

  To be used with ShaderProgram.
*/
}
#endif

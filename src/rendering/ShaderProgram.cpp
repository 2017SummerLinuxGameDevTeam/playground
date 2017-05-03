#include <glm/gtc/type_ptr.hpp>
#include "rendering/ShaderProgram.hpp"

namespace rendering
{
ShaderProgram::ShaderProgram():
_program_id(glCreateProgram()),
_linked(false)
{}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(_program_id);
}

ShaderProgram* ShaderProgram::addShader(const Shader& shader)
{
    glAttachShader(_program_id, shader.getId());
    return this;
}

ShaderProgram* ShaderProgram::bindFragmentOutput(const std::string& output_name)
{
    glBindAttribLocation(_program_id, 0, output_name.c_str());
    return this;
}

GLint ShaderProgram::bindVertexAttribute(const std::string& attrib_name, GLint size, GLsizei stride, GLvoid* first_pointer)
{
    GLint attrib_pos;

    attrib_pos = glGetAttribLocation(_program_id, attrib_name.c_str());
    glVertexAttribPointer(attrib_pos, size, GL_FLOAT, GL_FALSE, stride, first_pointer);

    return attrib_pos;
}

ShaderProgram* ShaderProgram::link()
{
    GLint status;
    char buffer[512];

    glLinkProgram(_program_id);
    glGetProgramiv(_program_id, GL_LINK_STATUS,& status);
    _linked = (status == GL_TRUE);
    glGetProgramInfoLog(_program_id, 512, nullptr, buffer);
    _error_log.assign(buffer);
    return this;
}

ShaderProgram* ShaderProgram::use()
{
    glUseProgram(_program_id);
    return this;
}

bool ShaderProgram::isLinked()
{
    return _linked;
}

const std::string& ShaderProgram::log() const
{
    return _error_log;
}

ShaderProgram* ShaderProgram::setUniform2f(const std::string& uniform_name, float v0, float v1)
{
    GLint location = glGetUniformLocation(_program_id, uniform_name.c_str());

    if(location != -1)
    {
        glUniform2f(location, v0, v1);
    }
    return this;
}

ShaderProgram* ShaderProgram::setUniform3f(const std::string& uniform_name, float v0, float v1, float v2)
{
    GLint location = glGetUniformLocation(_program_id, uniform_name.c_str());

    if(location != -1)
    {
        glUniform3f(location, v0, v1, v2);
    }
    return this;
}

ShaderProgram* ShaderProgram::setUniform4f(const std::string& uniform_name, float v0, float v1, float v2, float v3)
{
    GLint location = glGetUniformLocation(_program_id, uniform_name.c_str());

    if(location != -1)
    {
        glUniform4f(location, v0, v1, v2, v3);
    }
    return this;
}

ShaderProgram* ShaderProgram::setUniformMatrix4f(const std::string& uniform_name, const glm::mat4& mat)
{
    GLint location = glGetUniformLocation(_program_id, uniform_name.c_str());

    if(location != -1)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
    return this;
}
}

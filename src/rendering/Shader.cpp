#include <fstream>
#include "rendering/Shader.hpp"

namespace rendering
{
Shader::Shader():
_shader_id(0),
_compiled(false)
{}

Shader::~Shader()
{
    glDeleteShader(_shader_id);
}

void Shader::loadFromSource(const Shader::Type type, const std::string& source)
{
    if(_shader_id != 0)
    {
        glDeleteShader(_shader_id);
    }
    const char *source_ptr = source.c_str();
    GLint status;
    char buffer[512];

    if (type == Type::VERTEX_SHADER)
    {
        _shader_id = glCreateShader(GL_VERTEX_SHADER);
    }
    else
    {
        _shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    }

    if(_shader_id == 0)
    {
        return;
    }
    glShaderSource(_shader_id, 1, &source_ptr, nullptr);
    glCompileShader(_shader_id);
    glGetShaderiv(_shader_id, GL_COMPILE_STATUS, &status);
    _compiled = (status == GL_TRUE);
    glGetShaderInfoLog(_shader_id, 512, nullptr, buffer);
    _error_log.assign(buffer);
}

bool Shader::loadFromFile(const Shader::Type type, const std::string& filename)
{
    if(_shader_id != 0)
    {
        glDeleteShader(_shader_id);
    }
    std::string shader_source;
    if(!loadShaderSource(filename, shader_source))
    {
        return false;
    }
    loadFromSource(type, shader_source);
    return true;
}

GLuint Shader::getId() const
{
    return _shader_id;
}

bool Shader::isCompiled() const
{
    return _compiled;
}

const std::string& Shader::log() const
{
    return _error_log;
}

bool Shader::loadShaderSource(const std::string& filename, std::string& shader_source)
{
    std::ifstream file;
    file.open(filename.c_str());
    if(!file.is_open())
    {
        return false;
    }
    shader_source.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    return true;
}
}

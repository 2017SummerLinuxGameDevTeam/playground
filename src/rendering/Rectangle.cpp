#include "rendering/Rectangle.hpp"
#include "rendering/Plugin.hpp"

namespace rendering
{
ShaderProgram* Rectangle::_shader_program = nullptr;
GLuint Rectangle::_VAO = 0;
GLuint Rectangle::_VBO = 0;

Rectangle::Rectangle (const Color& color):
_color(color)
{}

void Rectangle::init()
{
    if (_shader_program == nullptr)
    {
        Shader v_shader;
        v_shader.loadFromFile(Shader::Type::VERTEX_SHADER, "shaders/plain.vert");
        hum::assert_msg(v_shader.isCompiled(), "Error compiling plain.vert\n", v_shader.log());
        Shader f_shader;
        f_shader.loadFromFile(Shader::Type::FRAGMENT_SHADER, "shaders/plain.frag");
        hum::assert_msg(f_shader.isCompiled(), "Error compiling plain.frag\n", f_shader.log());
        _shader_program = new ShaderProgram();
        _shader_program
            ->addShader(v_shader)
            ->addShader(f_shader)
            ->link()
            ->bindFragmentOutput("out_color");
        if (!_shader_program->isLinked())
        {
            hum::log_d(_shader_program->log());
            delete _shader_program;
            _shader_program = nullptr;
        }
    }

    if (_VAO == 0)
    {
        float vert[12] = { 0. , 0. ,
                           1. , 0. ,
                           1. , 1. ,
                           0. , 0. ,
                           1. , 1. ,
                           0. , 1. };

        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);
        glGenBuffers(1, &_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vert, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    setShaderProgram(_shader_program);
    Drawable::init();
}
void Rectangle::onDestroy()
{
    Drawable::onDestroy();
}

void Rectangle::setShaderProgram(ShaderProgram* shader_program)
{
    Drawable::setShaderProgram(shader_program);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    _position_loc = shaderProgram()->bindVertexAttribute("position", 2, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Rectangle::setColor(const Color& color)
{
    _color = color;
}

const Color& Rectangle::getColor() const
{
    return _color;
}

void Rectangle::draw()
{
    glBindVertexArray(_VAO);
    shaderProgram()->setUniform4f("color",
            static_cast<float>(_color.r)/255.0f,
            static_cast<float>(_color.g)/255.0f,
            static_cast<float>(_color.b)/255.0f,
            static_cast<float>(_color.a)/255.0f);
    glEnableVertexAttribArray(_position_loc);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

const char* Rectangle::behaviorName()
{
    return "mogl::Rectangle";
}
}

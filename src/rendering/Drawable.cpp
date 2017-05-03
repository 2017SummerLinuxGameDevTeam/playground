#include "rendering/Drawable.hpp"
#include "rendering/Plugin.hpp"

namespace rendering
{
Drawable::Drawable():
_is_enabled(true),
_origin(0.0),
_shader_program(nullptr)
{}

Drawable::~Drawable()
{}

void Drawable::init()
{
    if (_is_enabled)
    {
        actor().game().getPlugin<Plugin>()->addDrawable(this);
    }
}

void Drawable::onActivate()
{
    enable();
}

void Drawable::onDeactivate()
{
    disable();
}

void Drawable::onDestroy()
{
    disable();
}

void Drawable::enable()
{
    if (!_is_enabled)
    {
        actor().game().getPlugin<Plugin>()->addDrawable(this);
        _is_enabled = true;
    }
}

void Drawable::disable()
{
    if (_is_enabled)
    {
        actor().game().getPlugin<Plugin>()->removeDrawable(this);
        _is_enabled = false;
    }
}

bool Drawable::isEnabled() const
{
    return _is_enabled;
}

hum::Transformation& Drawable::transform()
{
    return _transform;
}

const hum::Transformation& Drawable::transform() const
{
    return _transform;
}

void Drawable::setShaderProgram(ShaderProgram* shader_program)
{
    _shader_program = shader_program;
}

ShaderProgram* Drawable::shaderProgram()
{
    return _shader_program;
}

const ShaderProgram* Drawable::shaderProgram() const
{
    return _shader_program;
}

void Drawable::setOrigin(const hum::Vector3f& origin)
{
    _origin = origin;
}

const hum::Vector3f& Drawable::getOrigin() const
{
    return _origin;
}

const char* Drawable::behaviorName()
{
    return "rendering::Drawable";
}
} /* rendering */

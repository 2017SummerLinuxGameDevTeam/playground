#include "rendering/Plugin.hpp"

namespace rendering
{
void defaultSpaceTransform(const hum::Game& game, hum::Transformation& r)
{}


Plugin::Plugin():
_clear_color(0,0,0,1),
_game_started(false),
_space_transform(defaultSpaceTransform)
{}


void Plugin::gameStart()
{
    try {
        _sdl_plugin = game().getPlugin<SDLPlugin>();
    } catch(hum::exception::PluginNotFound exception) {
        hum::log_e("Plugin SDLPlugin not found. Required for rendering::Plugin.");
        throw exception;
    }
    _game_started = true;
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    setClearColor(_clear_color);
}


void Plugin::postUpdate()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (_camera.projectionChanged())
    {
        for (auto it : _shader_program_usage)
        {
            it.first->use();
            it.first->setUniformMatrix4f("projection", _camera.getProjection());
        }
    }
    if (_camera.viewChanged())
    {
        for (auto it : _shader_program_usage)
        {
            it.first->use();
            it.first->setUniformMatrix4f("view", _camera.getView());
        }
    }

    glm::vec3 camera_position = humToGlm(_camera.getPosition());
    glm::vec3 camera_normal = humToGlm(_camera.getCenter()) - camera_position;
    glm::vec4 camera_plane(camera_normal, -(glm::dot(camera_normal, camera_position)));

    std::vector<DrawOrder_t> draw_order;
    for (Drawable* drawable : _drawable_set)
    {
        hum::Transformation drawable_transform = drawable->transform();
        const hum::Kinematic* kinematic = _drawable_kinematic[drawable];
        hum::Transformation actor_transform;
        if (kinematic != nullptr)
        {
            actor_transform = kinematic->simulate(game().fixedUpdateLag());
        }
        else
        {
            actor_transform = drawable->actor().transform();
        }
        drawable_transform = drawable_transform.transform(actor_transform);

        _space_transform(game(), drawable_transform);

        double distance_from_camera = glm::dot(
                camera_plane,
                glm::vec4(
                    drawable_transform.position.x,
                    drawable_transform.position.y,
                    drawable_transform.position.z,
                    1.f)
                );
        if (distance_from_camera <= _camera.getZFar() && distance_from_camera >= _camera.getZNear())
        {
            draw_order.push_back(DrawOrder_t{distance_from_camera, drawable_transform, drawable});
        }
    }

    std::sort(draw_order.begin(), draw_order.end(), [](const DrawOrder_t& left, const DrawOrder_t& right) { return left.order > right.order; });

    for (DrawOrder_t& value : draw_order)
    {
        Drawable* drawable = value.drawable;
        hum::assert_msg(drawable != nullptr, "Found a drawable nullptr");
        hum::Transformation& transform = value.transform;
        hum::assert_msg(drawable->shaderProgram() != nullptr, "Found a drawable without a shader program");

        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(transform.position.x, transform.position.y, transform.position.z));
        model = glm::rotate(model, glm::radians(static_cast<float>(transform.rotation.x)), glm::vec3(1., 0., 0.));
        model = glm::rotate(model, glm::radians(static_cast<float>(transform.rotation.y)), glm::vec3(0., 1., 0.));
        model = glm::rotate(model, glm::radians(static_cast<float>(transform.rotation.z)), glm::vec3(0., 0., 1.));
        model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));
        glm::vec3 origin(drawable->getOrigin().x, drawable->getOrigin().y, drawable->getOrigin().z);
        model = glm::translate(model, -origin);
        drawable->shaderProgram()->use();
        drawable->shaderProgram()->setUniformMatrix4f("model", model);
        drawable->draw();
    }
    glBindVertexArray(0);
    SDL_GL_SwapWindow(_sdl_plugin->window());
}



void Plugin::setClearColor(const Color& color)
{
    _clear_color = color;
    if (_game_started)
    {
        glClearColor(
                static_cast<float>(color.r)/255.f,
                static_cast<float>(color.g)/255.f,
                static_cast<float>(color.b)/255.f,
                1);
    }
}


void Plugin::setCamera(const Camera& camera)
{
    _camera = camera;
}


const Camera& Plugin::getCamera() const
{
    return _camera;
}


Camera& Plugin::getCamera()
{
    return _camera;
}


void Plugin::addDrawable(Drawable* drawable)
{
    _drawable_set.insert(drawable);
    try
    {
        _drawable_kinematic[drawable] = drawable->actor().getBehavior<hum::Kinematic>();
    }
    catch (hum::exception::BehaviorNotFound e)
    {
        _drawable_kinematic[drawable] = nullptr;
    }

    if (drawable->shaderProgram())
    {
        if (_shader_program_usage.find(drawable->shaderProgram()) == _shader_program_usage.end())
        {
            _shader_program_usage.insert(std::make_pair(drawable->shaderProgram(), 0));
            drawable->shaderProgram()->use();
            drawable->shaderProgram()->setUniformMatrix4f("projection", _camera.getProjection());
            drawable->shaderProgram()->setUniformMatrix4f("view", _camera.getView());
        }

        _shader_program_usage[drawable->shaderProgram()] += 1;
    }
}


void Plugin::removeDrawable(Drawable* drawable)
{
    _drawable_set.erase(drawable);
    _drawable_kinematic.erase(drawable);

    if (drawable->shaderProgram())
    {
        _shader_program_usage[drawable->shaderProgram()] -= 1;

        if (_shader_program_usage[drawable->shaderProgram()] == 0)
        {
            _shader_program_usage.erase(drawable->shaderProgram());
        }
    }
}


void Plugin::setDrawSpaceTransform(const SpaceTransformation& space_transform)
{
    _space_transform = space_transform;
}
} /* rendering */

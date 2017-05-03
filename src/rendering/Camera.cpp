#include "rendering/Camera.hpp"

namespace rendering
{
Camera::Camera ():
_projection_changed(true),
_view_changed(true),
_z_near(0.1f),
_z_far(1000.f),
_param1(0),
_param2(-100),
_param3(100),
_param4(0),
_is_ortho(true),
_position(hum::Vector3f(0, 0, -1)),
_center(hum::Vector3f(0, 0, 1)),
_up(hum::Vector3f(0, 1, 0))
{}

void Camera::setPerspective(float fovy, float aspect)
{
    _param1 = fovy;
    _param2 = aspect;
    _is_ortho = false;
    _projection_changed = true;
}

void Camera::setOrthogonal(float left, float right, float bottom, float top)
{
    _param1 = left;
    _param2 = right;
    _param3 = bottom;
    _param4 = top;
    _is_ortho = true;
    _projection_changed = true;
}

const glm::mat4& Camera::getProjection()
{
    if (_projection_changed)
    {
        _projection_changed = false;
        if (_is_ortho)
        {
            _projection = glm::ortho(_param1, _param2, _param3, _param4, _z_near, _z_far);
        }
        else
        {
            _projection = glm::perspective(_param1, _param2, _z_near, _z_far);
        }
    }
    return _projection;
}

const glm::mat4& Camera::getView()
{
    if (_view_changed)
    {
        _view_changed = false;
        _view = glm::lookAt(humToGlm(_position), humToGlm(_center), humToGlm(_up));
    }
    return _view;
}

void Camera::setPosition(const hum::Vector3f& position)
{
    _view_changed = true;
    _position = position;
}

const hum::Vector3f& Camera::getPosition() const
{
    return _position;
}

void Camera::setCenter(const hum::Vector3f& center)
{
    _view_changed = true;
    _center = center;
}

const hum::Vector3f& Camera::getCenter() const
{
    return _center;
}

void Camera::setUp(const hum::Vector3f& up)
{
    _view_changed = true;
    _up = up;
}

const hum::Vector3f& Camera::getUp() const
{
    return _up;
}

void Camera::setZNear(float z_near)
{
    _projection_changed = true;
    _z_near = z_near;
}

float Camera::getZNear() const
{
    return _z_near;
}

void Camera::setZFar(float z_far)
{
    _projection_changed = true;
    _z_far = z_far;
}

float Camera::getZFar() const
{
    return _z_far;
}

bool Camera::projectionChanged() const
{
    return _projection_changed;
}

bool Camera::viewChanged() const
{
    return _view_changed;
}
}

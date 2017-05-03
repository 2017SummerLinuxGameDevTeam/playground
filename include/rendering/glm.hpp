#ifndef MOGL_GLM_HELPER_HPP
#define MOGL_GLM_HELPER_HPP
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include "hummingbird/Vector3.hpp"

inline glm::vec3 humToGlm(const hum::Vector3f& v)
{
    return glm::vec3(v.x, v.y, v.z);
}
#endif

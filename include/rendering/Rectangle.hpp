#ifndef MOGL_RECTANGLE_HPP
#define MOGL_RECTANGLE_HPP
#include "common.hpp"
#include "Drawable.hpp"

namespace rendering
{
class Rectangle : public Drawable
{
public:
    /*!
      \brief Class construtor with fill color
     */
    Rectangle (const Color&);

    void init() override;
    void onDestroy() override;

    void setShaderProgram(ShaderProgram* shader_program) override;

    /*!
      \brief Set fill color for the Rectangle
     */
    void setColor(const Color& color);

    /*!
      \brief Get fill color for the Rectangle
     */
    const Color& getColor() const;

    /*!
      \brief Draw the Rectangle
     */
    void draw() override;

    static const char* behaviorName();

private:
    static ShaderProgram* _shader_program;
    static GLuint _VAO, _VBO;
    GLuint _position_loc;
    Color _color;
};

/*!
  \class mogl::Rectangle
  \brief A rectangle-shaped 1x1 Drawable.

  For different sizes use the scale in either the hum::Actor hum::Transform of
  the Drawable's hum::Transform.
*/
}
#endif /* ifndef MOGL_RECTANGLE_HPP */

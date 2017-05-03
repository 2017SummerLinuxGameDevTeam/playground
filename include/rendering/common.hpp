#ifndef RENDERING_COMMON_HPP
#define RENDERING_COMMON_HPP
namespace rendering
{
class Color {
public:
    Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);
    unsigned int r, g, b, a;
};
}
#endif /* ifndef RENDERING_COMMON_HPP */

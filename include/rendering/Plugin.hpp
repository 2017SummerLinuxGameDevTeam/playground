#ifndef RENDERING_PLUGIN_HPP
#define RENDERING_PLUGIN_HPP

#include <unordered_map>
#include <algorithm>
#include <GL/glew.h>
#include "hummingbird/hum.hpp"
#include "SDLPlugin.hpp"
#include "rendering/common.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Drawable.hpp"

namespace rendering
{
/*!
  \brief Signature for a space transformation method. (see MultimediaOGL::setDrawSpaceTransform()).

  The first parameter is the instance of the hum::Game and the second is the
  hum::Transformation of the Drawable to be drawn, not the one relative to its
  hum::Actor, but the accumulated one.
 */
typedef std::function<void(const hum::Game&, hum::Transformation&)> SpaceTransformation;
void defaultSpaceTransform(const hum::Game&, hum::Transformation&);


class Plugin : public hum::Plugin
{
public:
    Plugin();
    void gameStart() override;
    void postUpdate() override;

    //! Set the clear color for OpenGL
    void setClearColor(const Color& color);

    /*!
      \brief Set a new Camera to use.

      Usage disencouraged. Better use getCamera() and set the desired configuration
      using its setters.
     */
    void setCamera(const Camera& camera);

    //! Get the Camera being used
    const Camera& getCamera() const;

    //! Get the Camera being used
    Camera& getCamera();

    //! Register a Drawable to be drawn (Internal use only).
    void addDrawable(Drawable* drawable);

    //! Register a Drawable to be drawn (Internal use only).
    void removeDrawable(Drawable* drawable);

    /*!
      \brief Set the space transformation between the game logic space and the
      draw space.

      This is useful when, for example you have a game with 2D logic but you want
      to render it on a irregular terrain (height map). You can use this method
      for decoupling the 2D logic from the 3D representation.

      Another example is to draw a 2D tile based (position is always aligned with
      the tiles) game as a 2D isometric game.

      Example:
      \code

      // Supose we have a hum::Plugin for managing terrains.
      void setTerrainHeight(const hum::Game& game, hum::Transformation& r)
      {
        r.position.z = game.getPlugin<Terrains>()->getCurrent().height(r.position.x, r.position.y);
      }

      //...

      game().getPlugin<mogl::MultimediaOGL>()->setDrawSpaceTransform(setTerrainHeight);
      \endcode
     */
    void setDrawSpaceTransform(const SpaceTransformation& space_transform);

private:
    struct DrawOrder_t {
        double order;
        hum::Transformation transform;
        Drawable* drawable;
    };

    SDLPlugin* _sdl_plugin;
    Color _clear_color;
    bool _game_started;
    Camera _camera;
    std::unordered_set<Drawable*> _drawable_set;
    std::unordered_map<ShaderProgram*, unsigned int> _shader_program_usage;
    std::unordered_map<Drawable*, const hum::Kinematic*> _drawable_kinematic;
    SpaceTransformation _space_transform;
};
}
#endif /* RENDERING_PLUGIN_HPP */

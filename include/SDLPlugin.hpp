#ifndef SDL_PLUGIN_HPP
#define SDL_PLUGIN_HPP

#include <string>
#include <SDL2/SDL.h>
#include "hummingbird/hum.hpp"

struct WindowConfig_t {
    std::string name;
    int x, y, w, h;
    Uint32 flags;
};

class SDLPlugin : public hum::Plugin
{
public:
    SDLPlugin(const WindowConfig_t& window_cfg);
    void gameStart() override;
    void postUpdate() override;
    void gameEnd() override;

    SDL_Window* window();

private:
    SDL_Window* _mainwindow; /* Our window handle */
    SDL_GLContext _maincontext; /* Our opengl context handle */
    WindowConfig_t _window_cfg;
};
#endif /* ifndef SDL_PLUGIN_HPP */

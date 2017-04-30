#include "SDLPlugin.hpp"

/* A simple function that prints a message, the error code returned by SDL,
 * and quits the application */
void sdldie(const char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}


void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
#endif
}


SDLPlugin::SDLPlugin(const WindowConfig_t& window_cfg):
_window_cfg(window_cfg)
{}


void SDLPlugin::gameStart()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        sdldie("Unable to initialize SDL"); /* Or die on error */

    /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Create our window centered at 512x512 resolution */
    _mainwindow = SDL_CreateWindow(
            _window_cfg.name.c_str(),
            _window_cfg.x, _window_cfg.y, _window_cfg.w, _window_cfg.h,
            _window_cfg.flags
            );
    if (!_mainwindow) /* Die if creation failed */
        sdldie("Unable to create window");

    checkSDLError(__LINE__);

    /* Create our opengl context and attach it to our window */
    _maincontext = SDL_GL_CreateContext(_mainwindow);
    checkSDLError(__LINE__);

    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);
}


void SDLPlugin::postUpdate()
{
    // TODO: here is where all the drawing should happen

    /* Swap our back buffer to the front */
    SDL_GL_SwapWindow(_mainwindow);
}


void SDLPlugin::gameEnd()
{
    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext(_maincontext);
    SDL_DestroyWindow(_mainwindow);
    SDL_Quit();
}

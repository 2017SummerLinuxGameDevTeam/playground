#include "hummingbird/hum.hpp"
#include "SDLPlugin.hpp"

// Custom behavior that we will add to the actors
// to see their positions
class PrintPosition : public hum::Behavior
{
public:
    void fixedUpdate() override
    {
        // all behaviors have a way to access the actor they are in
        hum::log_d("Actor ", actor().id(), " position: ", actor().transform().position);
    }
};


// Custom Plugin to demonstrate
// This ends the game after max_seconds seconds
class CloseGame : public hum::Plugin
{
public:
    CloseGame(float max_seconds):
    _max_seconds(max_seconds)
    {}

    void gameStart() override
    {
        _clk.reset();
    }

    void postUpdate() override
    {
        if (_clk.getTime().asSeconds() >= _max_seconds)
        {
            game().setRunning(false);
        }
    }

    void gameEnd() override
    {
        hum::log("Bye!");
    }

private:
    float _max_seconds;
    hum::Clock _clk;
};

int main(void)
{
    // Create a game instance
    hum::Game game;

    // Add a Plugin to the game instance.
    // In this case KinematicWorld takes care of moving Actors with Kinematic behaviors
    // The assignment is not needed here. I'm doing it to demonstrate how addPlugin works
    hum::KinematicWorld* kinematic_world = game.addPlugin<hum::KinematicWorld>();
    // Both addPlugin and addBehavior have as arguments whatever the class passed
    // to he template needs for its constructor.
    game.addPlugin<CloseGame>(3);
    game.addPlugin<SDLPlugin>(WindowConfig_t{"Playground", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN});

    // Create a new actor (GameObject). It is stored and managed by a ActorPool in game.
    hum::Actor* actor = game.actors().create();
    // Add a Kinematic behavior to the actor to give it some speed
    hum::Kinematic* kinematic = actor->addBehavior<hum::Kinematic>();
    kinematic->velocity().position.x = 10;
    // Add our custom behavior from before
    actor->addBehavior<PrintPosition>();

    game.run();

    return 0;
}

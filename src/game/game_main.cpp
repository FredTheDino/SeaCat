// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>

#include "text_zoom.cpp"

namespace Game {

void entity_registration() {}

enum class GameState {
    INTRO,
    GAME,
    TRANSITION,
    THERAPY
};

GameState game_state = GameState::GAME;

void setup() {
    Renderer::turn_on_camera(0);

    Input::add(K(SPACE), Input::Name::TEST);
}

// Main logic
void update(f32 delta) {

    if (Input::pressed(Input::Name::TEST)) {
        pick_up_compliment("Soot!", V2(0, 0));
    }

    // Determine which part of the game we are in.
    switch (game_state) {

        // Update intro.
        case GameState::INTRO:
            break;

        // Update game.
        case GameState::GAME:
            break;

        // Update transition.
        case GameState::TRANSITION:
            break;

        // Update thrapy session.
        case GameState::THERAPY:
            break;
    }
}

// Main draw
void draw() {
    const char *some_string = "SeaCow";

    Renderer::push_rectangle(0, V2(0, 0), V2(5, 5), V4(0.2, 0.2, 0.2, 1.0));

    // Determine which part of the game we are in.
    switch (game_state) {

        // Draw intro.
        case GameState::INTRO:
            break;

        // Draw game.
        case GameState::GAME:
            break;

        // Draw transition.
        case GameState::TRANSITION:
            break;

        // Draw thrapy session.
        case GameState::THERAPY:
            break;
    }
}

}  // namespace Game

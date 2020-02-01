// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>
#include "enemies.h"

namespace Game {

void entity_registration() {
    REGISTER_ENTITY(AggroEnemy);
}

enum class GameState {
    INTRO,
    GAME,
    TRANSITION,
    THERAPY
};

GameState game_state = GameState::GAME;


void setup() {
    enemy_shape = Physics::add_shape(LEN(enemy_shape_points), enemy_shape_points);
    AggroEnemy aggro_enemy;
    aggro_enemy_init(aggro_enemy);
    FloofEnemy floof_enemy;
    floof_enemy_init(floof_enemy, V2(0, 1));
    Logic::add_entity(aggro_enemy);
    Logic::add_entity(floof_enemy);

    Renderer::turn_on_camera(0);
}

// Main logic
void update(f32 delta) {
    static bool show_control_contols = true;
    if (Util::begin_tweak_section("target", &show_control_contols)) {
        Util::tweak("center", &target);
    }
    Util::end_tweak_section(&show_control_contols);
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

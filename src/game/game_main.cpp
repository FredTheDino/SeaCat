// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>
#include "enemies.h"

namespace Game {

void entity_registration() {}

enum class GameState {
    INTRO,
    GAME,
    TRANSITION,
    THERAPY
};

GameState game_state = GameState::GAME;

std::vector<Enemy*> enemies;

void setup() {
    enemy_shape = Physics::add_shape(LEN(enemy_shape_points), enemy_shape_points);
    enemies.push_back(new SmallEnemy(V2(0, 0)));

    Renderer::turn_on_camera(0);
}

// Main logic
void update(f32 delta) {

    // Determine which part of the game we are in.
    switch (game_state) {

        // Update intro.
        case GameState::INTRO:
            break;

        // Update game.
        case GameState::GAME:
            for (Enemy* enemy : enemies) {
                enemy->update(delta);
            }
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
            for (Enemy* enemy : enemies) {
                Renderer::push_rectangle(1, enemy->body.position, enemy->body.scale);
            }
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

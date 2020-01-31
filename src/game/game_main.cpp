// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>
#include "phase_one_player.cpp"

namespace Game {

phase_one_player player1;

void entity_registration() {}

void setup() {
    player1 = phase_one_player();
    Renderer::turn_on_camera(0);

    using namespace Input;
    add(K(a), Name::LEFT);
    add(K(d), Name::RIGHT);
    add(K(w), Name::UP);
    add(K(s), Name::DOWN);
}

// Main logic
void update(f32 delta) {
    player1.update(delta);
}

// Main draw
void draw() {
    const char *some_string = "SeaCow";
    Renderer::draw_text(some_string, 0, 0, 1.0, ASSET_MONACO_FONT, -0.5);
    Physics::debug_draw_body(player1.get_body());
}

}  // namespace Game

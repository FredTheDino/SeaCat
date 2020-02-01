// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>

Logic::LogicID update_id;
Logic::LogicID draw_id;

void (*current_exit)();

#include "game_state_phase1.cpp"

namespace Game {

void entity_registration() {}

void empty_func() {}

void setup() {
    Renderer::turn_on_camera(0);

    // Set inital state for gamestates
    {
        update_id = Logic::add_callback(Logic::PRE_UPDATE, empty_func,
                0.0, Logic::FOREVER);

        draw_id = Logic::add_callback(Logic::PRE_DRAW, empty_func,
                0.0, Logic::FOREVER);
        current_exit = empty_func;
        Phase1::enter();
    }
}

// Extra logic
void update(f32 delta) {}

// Extra draw
void draw() {}

}  // namespace Game

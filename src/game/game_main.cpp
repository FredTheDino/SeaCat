// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>

Logic::LogicID update_id;
Logic::LogicID draw_id;

void (*current_exit)();

#include "game_state_phase1.cpp"
#include "game_state_phase2.cpp"

namespace Game {

void entity_registration() {}

void empty_func() {}

void setup() {
    Renderer::turn_on_camera(0);

    using namespace Input;
    add(K(a), Name::LEFT);
    add(K(d), Name::RIGHT);
    add(K(w), Name::UP);
    add(K(s), Name::DOWN);

    add(A(LEFTX, Player::P1), Name::LEFT_RIGHT);
    add(A(LEFTY, Player::P1), Name::UP_DOWN);
    add(A(LEFTX, Player::P2), Name::LEFT_RIGHT);
    add(A(LEFTY, Player::P2), Name::UP_DOWN);

    {
        update_id = Logic::add_callback(Logic::PRE_UPDATE, empty_func,
                0.0, Logic::FOREVER);

        draw_id = Logic::add_callback(Logic::PRE_DRAW, empty_func,
                0.0, Logic::FOREVER);
        current_exit = empty_func;
        Phase2::enter();
    }
}

// Extra logic
void update(f32 delta) {}

// Extra draw
void draw() {}

}  // namespace Game

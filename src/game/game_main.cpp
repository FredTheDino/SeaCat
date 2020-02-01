// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>
#include "assets.cpp"
#include "enemies.h"

Spawner spawner;

Logic::LogicID update_id;
Logic::LogicID draw_id;
void (*current_exit)();

#include "game_state_phase1.cpp"
#include "game_state_phase2.cpp"
#include "game_state_intro.cpp"

namespace Game {

void entity_registration() {
    REGISTER_ENTITY(AggroEnemy);
}

void empty_func() {}


void setup() {
    Phase1::setup();
    Phase2::setup();

    enemy_shape = Physics::add_shape(LEN(enemy_shape_points), enemy_shape_points);

    Renderer::turn_on_camera(0);
	
    using namespace Input;
    add(K(a), Name::LEFT);
    add(K(d), Name::RIGHT);
    add(K(w), Name::UP);
    add(K(s), Name::DOWN);
    add(K(SPACE), Name::SHOOT);

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
        Intro::enter(0);
        Phase2::enter();
    }
}


// Extra logic
void update(f32 delta) {
    Renderer::debug_camera(0);
}

// Extra draw
void draw() {
    //for (u32 i = 0; i < (u32) Sprites::NUM_SPRITES; i++) {
    //    draw_sprite(0, V2(0, i), 0.5, 0.0, (Sprites) i);
    //}
}

}  // namespace Game

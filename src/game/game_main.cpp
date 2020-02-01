// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>
#include "assets.cpp"
#include "game_entities.h"

Spawner enemy_spawner;
Spawner cog_spawner;

Logic::LogicID update_id;
Logic::LogicID draw_id;
void (*current_exit)();

#include "game_state_phase1.cpp"
#include "game_state_phase2.cpp"
#include "game_state_intro.cpp"

namespace Game {

f32 INTRO_TIME = 12;
u32 CRITICAL_CONFIDENCE = 3;
u32 enteredCS = 0;
u32 confidence = 0;
u32 intro = 0;
u32 phase = 0;

void entity_registration() {
    REGISTER_ENTITY(AggroEnemy);
}

void empty_func() {}


void setup() {
    Phase1::setup();
    Phase2::setup();

    square_shape = Physics::add_shape(LEN(square_shape_points), square_shape_points);

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
        Intro::enter(intro);
        enteredCS = Logic::now();
    }
}


// Extra logic
void update(f32 delta) {
    Renderer::debug_camera(0);
    if (Logic::now() - enteredCS >= INTRO_TIME && phase < 1) {
        phase = 1;
        Intro::exit();
        Phase1::enter();
    }
    else if (confidence >= CRITICAL_CONFIDENCE && intro < 1) {
        intro = 1;
        Phase1::exit();
        Intro::enter(intro);
        enteredCS = Logic::now();
    }
    else if (Logic::now() - enteredCS >= 15 && phase < 2) {
        phase = 2;
        Intro::exit();
        Phase2::enter();
    }
    else if (confidence >= CRITICAL_CONFIDENCE * 2 && intro < 2) {
        intro = 2;
        Phase2::exit();
        Intro::enter(intro);
        enteredCS = Logic::now();
    }
    
    else if (Logic::now() - enteredCS >= INTRO_TIME && phase < 3) {
        phase = 3;
        Intro::exit();
        //Boss::enter();
    }
    
}


// Extra draw
void draw() {
    //for (u32 i = 0; i < (u32) Sprites::NUM_SPRITES; i++) {
    //    draw_sprite(0, V2(0, i), 0.5, 0.0, (Sprites) i);
    //}
}

}  // namespace Game

// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>
#include "enemies.h"

Logic::LogicID update_id;
Logic::LogicID draw_id;

void (*current_exit)();

#include "game_state_phase1.cpp"
#include "game_state_phase2.cpp"

namespace Game {

void entity_registration() {
    REGISTER_ENTITY(AggroEnemy);
}

void empty_func() {}


void setup() {
    enemy_shape = Physics::add_shape(LEN(enemy_shape_points), enemy_shape_points);

    AggroEnemy aggro_enemy;
    aggro_enemy_init(aggro_enemy);

    FloofEnemy floof_enemy;
    floof_enemy_init(floof_enemy, V2(0, 1));

    GloopEnemy gloop_enemy;
    gloop_enemy_init(gloop_enemy, V2(0,1));

    Logic::add_entity(aggro_enemy);
    Logic::add_entity(floof_enemy);
    Logic::add_entity(gloop_enemy);

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
        Phase2::enter();
    }
}


// Extra logic
void update(f32 delta) {
    static bool show_control_contols = true;
    if (Util::begin_tweak_section("target", &show_control_contols)) {
        Util::tweak("center", &target);
    }
    Util::end_tweak_section(&show_control_contols);
}

// Extra draw
void draw() {}

}  // namespace Game

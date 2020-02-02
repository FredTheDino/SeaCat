// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>

Vec2 square_shape_points[] = {V2(0, 0), V2(1, 0), V2(1, 1), V2(0, 1)};
Physics::ShapeID square_shape;

bool stars_spawned = false;

#include "assets.cpp"
#include "particles.h"
#include "player.h"
#include "player.cpp"
#include "game_entities.h"
#include "game_entities.cpp"
#include "text_zoom.cpp"

Spawner enemy_spawner;
Spawner cog_spawner;

Logic::LogicID update_id;
Logic::LogicID draw_id;
void (*current_exit)();

#include "game_state_cutscenes.h"
#include "game_state_phase1.cpp"
#include "game_state_phase2.cpp"
#include "game_state_cutscenes.cpp"

namespace Game {

u32 confidence = 0;
u32 intro = 0;
u32 phase = 0;

void entity_registration() { REGISTER_ENTITY(AggroEnemy); }

void empty_func() {}

void setup() {
    Phase1::setup();
    Phase2::setup();

    square_shape =
        Physics::add_shape(LEN(square_shape_points), square_shape_points);

    Renderer::turn_on_camera(0);

    using namespace Input;
    add(K(f), Name::FULLSCREEN);

    add(K(a), Name::LEFT);
    add(K(d), Name::RIGHT);
    add(K(w), Name::UP);
    add(K(s), Name::DOWN);
    add(K(SPACE), Name::SHOOT);

    add(A(LEFTX, Player::P1), Name::LEFT_RIGHT);
    add(A(LEFTY, Player::P1), Name::UP_DOWN);
    add(A(LEFTX, Player::P2), Name::LEFT_RIGHT);
    add(A(LEFTY, Player::P2), Name::UP_DOWN);

    add(B(B, Player::P1), Name::SHOOT);
    add(B(B, Player::P2), Name::SHOOT);

    init_laser_particles();
    init_star_system();

    {
        update_id = Logic::add_callback(Logic::PRE_UPDATE, empty_func, 0.0,
                                        Logic::FOREVER);

        draw_id = Logic::add_callback(Logic::PRE_DRAW, empty_func, 0.0,
                                      Logic::FOREVER);
        current_exit = empty_func;
        // Cutscene::enter(0);
        //Cutscene::enter(0);
        Phase1::enter();
    }
}

// Extra logic
void update(f32 delta) {
    if (!stars_spawned) {
        for (u32 i = 0; i < 50; i++) {
            LOG("spawning %d", i);
            star_system.spawn();
        }
        stars_spawned = true;
    }
    star_system.spawn();

    Renderer::debug_camera(0);
    using namespace Input;
    if (pressed(Name::FULLSCREEN)) {
        Renderer::toggle_fullscreen();
    }

    static bool camera_vignette = true;
    if (Util::begin_tweak_section("CAMERA VIGNETTE", &camera_vignette)) {
        Util::tweak("vin-radius", &Renderer::vignette_radius);
        Util::tweak("vin-strengt", &Renderer::vignette_strength);
    }
    Util::end_tweak_section(&camera_vignette);
}

// Extra draw
void draw() {}

}  // namespace Game

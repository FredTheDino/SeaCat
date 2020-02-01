// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>

namespace Game {

void entity_registration() {}

void setup() {
    Renderer::turn_on_camera(0);

    Renderer::get_camera(0)->zoom = 0.1;

    using namespace Input;
    add(K(a), Name::LEFT);
    add(K(d), Name::RIGHT);
    add(K(w), Name::UP);
    add(K(s), Name::DOWN);

    add(A(LEFTX, Player::P1), Name::LEFT_RIGHT);
    add(A(LEFTY, Player::P1), Name::UP_DOWN);
    add(A(LEFTX, Player::P2), Name::LEFT_RIGHT);
    add(A(LEFTY, Player::P2), Name::UP_DOWN);
}

// scroll
const f32 MAX_SPEED = 10;  // ship
f32 vel_rel = 0.4;
f32 floatiness = 4;
Vec2 vel = V2(0, 0);  // change per frame
Vec2 pos = V2(0, 0);  // current position
const Vec2 DIM = V2(1, 1);  // size of rect

// Main logic
void update(f32 delta) {
    static bool show_control_controls = true;
    if (Util::begin_tweak_section("controls", &show_control_controls)) {
        Util::tweak("floatiness", &floatiness);
        Util::tweak("vel_rel", &vel_rel);
    }
    Util::end_tweak_section(&show_control_controls);

    using namespace Input;
    Vec2 vel_target = V2(0, 0);
    if (down(Name::UP))
        vel_target.y += vel_rel;
    if (down(Name::DOWN))
        vel_target.y -= vel_rel;
    if (down(Name::LEFT))
        vel_target.x -= vel_rel;
    if (down(Name::RIGHT))
        vel_target.x += vel_rel;
    vel_target.y += vel_rel * value(Name::UP_DOWN);
    vel_target.x += vel_rel * value(Name::LEFT_RIGHT);

    vel += (vel_target - vel) * floatiness * delta;
    pos += vel;
}

// Main draw
void draw() {
    Renderer::push_rectangle(0, pos, DIM);
}

}  // namespace Game

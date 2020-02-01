namespace Phase2 {

void enter();
void update(f32 now, f32 delta);
void draw();
void exit();

f32 max_vel = 0.4;  // ship acceleration
f32 control = 4;  // ship-control, higher => better control
Vec2 vel = V2(0, 0);  // ship velocity
Vec2 pos = V2(0, 0);  // ship position
const Vec2 DIM = V2(1, 1);  // size of ship

void enter() {
    current_exit();
    Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
    Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
    current_exit = exit;

    Renderer::get_camera(0)->zoom = 0.1;
}

void update(f32 delta, f32 now) {
    using namespace Util;
    static bool show_ship_controls = true;
    if (begin_tweak_section("ship controls", &show_ship_controls)) {
        tweak("max velocity", &max_vel);
        tweak("control", &control);
    }
    end_tweak_section(&show_ship_controls);

    using namespace Input;
    Vec2 vel_target = V2(0, 0);
    if (down(Name::UP))
        vel_target.y += max_vel;
    if (down(Name::DOWN))
        vel_target.y -= max_vel;
    if (down(Name::LEFT))
        vel_target.x -= max_vel;
    if (down(Name::RIGHT))
        vel_target.x += max_vel;
    vel_target.y += max_vel * value(Name::UP_DOWN);
    vel_target.x += max_vel * value(Name::LEFT_RIGHT);

    vel += (vel_target - vel) * control * delta;
    pos += vel;
}

void draw() {
    Renderer::push_rectangle(0, pos, DIM);
}

void exit() {}

}

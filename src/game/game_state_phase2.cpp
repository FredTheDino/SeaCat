namespace Phase2 {

Vec2 player_shape_points[] = { V2(0, 0), V2(1, 0), V2(1, 1), V2(0, 1) };
Physics::ShapeID player_shape;

struct Player : public Logic::Entity {
    Physics::Body body;

    Vec2 position;
    Vec2 velocity = V2(0, 0);
    f32 max_velocity = 0.04;
    f32 control = 4;  // higher => better control
    const Vec2 DIMENSIONS = V2(0.1, 0.1);

    u32 wobble_speed = 10;
    f32 wobble_amp = 0.005;

    f32 shot_width = 0.05;
    f32 shot_held_for = 0;
    f32 shot_held_target = 0.5;
    f32 shot_current_shot_length = 0;
    f32 shot_length = 1;
    bool shot_shooting = false;

    void update(f32 delta) override {
        using namespace Util;
        static bool show_ship_controls = true;
        if (begin_tweak_section("ship controls", &show_ship_controls)) {
            tweak("max velocity", &max_velocity);
            tweak("control", &control);
            tweak("wobble speed", &wobble_speed);
            tweak("wobble amp", &wobble_amp);
            tweak("shot width", &shot_width);
            tweak("shot held for", &shot_held_for);
            tweak("shot held for - target", &shot_held_target);
            tweak("shot length", &shot_length);
            tweak("shot shooting", &shot_shooting);
        }
        end_tweak_section(&show_ship_controls);

        using namespace Input;
        Vec2 vel_target = V2(0, 0);
        if (down(Name::UP))
            vel_target.y += max_velocity;
        if (down(Name::DOWN))
            vel_target.y -= max_velocity;
        if (down(Name::LEFT))
            vel_target.x -= max_velocity;
        if (down(Name::RIGHT))
            vel_target.x += max_velocity;
        vel_target.y += max_velocity * value(Name::UP_DOWN);
        vel_target.x += max_velocity * value(Name::LEFT_RIGHT);

        velocity += (vel_target - velocity) * control * delta;
        position += velocity + V2(sin(Logic::now() * wobble_speed) * wobble_amp, 0);
        body.position = position;

        if (down(Input::Name::SHOOT)) {
            shot_held_for += delta;
        }

        if (released(Input::Name::SHOOT)) {
            if (shot_held_for >= shot_held_target) shot_shooting = true;
            shot_held_for = 0;
        }

        if (shot_shooting) {
            if (shot_current_shot_length > shot_length) {
                shot_shooting = false;
                shot_current_shot_length = 0;
            }
            shot_current_shot_length += delta;
        }
    }

    void draw() override {
        Renderer::push_rectangle(0, position, DIMENSIONS);

        if (shot_held_for >= shot_held_target) {
            //TODO(gu) particles
        }

        if (shot_shooting) {
            f32 max_height = Renderer::get_camera(0)->zoom;
            f32 shot_height = max_height - position.y;
            Renderer::push_rectangle(1, position + V2(0, shot_height/2), V2(shot_width, shot_height - 0.1), V4(0, 0.2, 1, 1));
        }
    }
};

void player_init(Player &player, Vec2 position=V2(0, 0)) {
    player.position = position;
    player.body = Physics::create_body(player_shape);
    player.body.scale = player.DIMENSIONS;
    player.body.position = player.position;
}

Player player;

void enter();
void update(f32 now, f32 delta);
void draw();
void exit();

void enter() {
    current_exit();
    Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
    Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
    current_exit = exit;

    player_shape = Physics::add_shape(LEN(player_shape_points), player_shape_points);
    player_init(player);
}

void update(f32 delta, f32 now) {
    player.update(delta);
}

void draw() {
    player.draw();
}

void exit() {}

}

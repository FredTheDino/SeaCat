f32 cos_acc_mod(Vec2 cur_velocity, f32 max_velocity, f32 min_acceleration) {
    Vec2 velocity_percentage = cur_velocity / max_velocity;
    if ((velocity_percentage.x + velocity_percentage.y) <= min_acceleration) {
        return min_acceleration;
    } else {
        return ((sin(velocity_percentage.x * (PI / 2)) +
                 sin(velocity_percentage.y * (PI / 2))) /
                2);
    }
}

void PlayerPhase1::update(f32 delta) {
    if (DEBUG) {
        using namespace Util;
        static bool show_control_controls = true;
        if (begin_tweak_section("player phase 1", &show_control_controls)) {
            tweak("max velocity", &max_velocity);
            tweak("max acceleration", &max_acceleration);
            tweak("acceleration steps", &acceleration_steps);
        }
        end_tweak_section(&show_control_controls);
    }

    using namespace Input;
    Vec2 acc = V2(0, 0);
    if (down(Name::UP)) acc.y += acceleration_steps;
    if (down(Name::DOWN)) acc.y -= acceleration_steps;
    if (down(Name::LEFT)) acc.x -= acceleration_steps;
    if (down(Name::RIGHT)) acc.x += acceleration_steps;
    acc.x = CLAMP(-max_acceleration, max_acceleration, acc.x);
    acc.y = CLAMP(-max_acceleration, max_acceleration, acc.y);
    body.acceleration = acc;

    body.velocity += body.acceleration * cos_acc_mod(body.velocity, max_velocity, min_acceleration);
    body.velocity.x = CLAMP(-max_velocity, max_velocity, body.velocity.x);
    body.velocity.y = CLAMP(-max_velocity, max_velocity, body.velocity.y);

    body.position += body.velocity;
}

void PlayerPhase1::draw() {
    draw_sprite(3, body.position, 0.3, 0, Sprites::CAT);
    f32 angle = sin(Logic::now()) * 0.2 + 0.2;
    draw_sprite(2, body.position - rotate(V2(0, -0.3), angle) * 0.2, 0.3, angle, Sprites::TAIL);
    Renderer::push_line(10, body.position, body.position + rotate(V2(0, -0.3), angle) * 0.2, V4(0, 1, 0, 1));
}

void PlayerPhase1::init() {
    body = Physics::create_body(square_shape, 0b110);  //TODO(gu) ????
    body.scale = V2(1, 1) * 0.2;
}

void PlayerPhase2::update(f32 delta) {
#ifdef DEBUG
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
            tweak("shot shooting", &shooting);
        }
        end_tweak_section(&show_ship_controls);
#endif

    using namespace Input;
    Vec2 vel_target = V2(0, 0);
    if (down(Name::UP)) vel_target.y += max_velocity;
    if (down(Name::DOWN)) vel_target.y -= max_velocity;
    if (down(Name::LEFT)) vel_target.x -= max_velocity;
    if (down(Name::RIGHT)) vel_target.x += max_velocity;
    vel_target.y += max_velocity * value(Name::UP_DOWN);
    vel_target.x += max_velocity * value(Name::LEFT_RIGHT);

    velocity += (vel_target - velocity) * control * delta;
    wobble_velocity = V2(sin(Logic::now() * wobble_speed) * wobble_amp, 0);
    position += velocity + wobble_velocity;
    ship_body.position = position;

    rightLaser.position = position;
    leftLaser.position = position;
    middleLaser.position = position;

    rightLaser.update(delta);
    leftLaser.update(delta);
    middleLaser.update(delta);

    if (down(Input::Name::SHOOT)) {
        shot_held_for += delta;
        rightLaser.spawn();
        rightLaser.spawn();
        rightLaser.spawn();
        leftLaser.spawn();
        leftLaser.spawn();
        leftLaser.spawn();
    }

    if (released(Input::Name::SHOOT)) {
        if (shot_held_for >= shot_held_target) shooting = true;
        shot_held_for = 0;
    }

    if (shooting) {
        if (shot_current_shot_length > shot_length) {
            shooting = false;
            shot_current_shot_length = 0;
        }
        shot_current_shot_length += delta;
        rightLaser.spawn();
        leftLaser.spawn();
        middleLaser.spawn();
        rightLaser.spawn();
        leftLaser.spawn();
        middleLaser.spawn();
        rightLaser.spawn();
        leftLaser.spawn();
        middleLaser.spawn();
    }

    f32 max_height = Renderer::get_camera()->zoom / Renderer::get_camera()->aspect_ratio;
    f32 shot_height = max_height/2 - position.y;
    shot_body.position = position + V2(0, shot_height / 2);
    shot_body.scale = V2(shot_width, shot_height);
}

void PlayerPhase2::draw() {
    const f32 MAX_ROT = PI/16;
    draw_sprite(2, ship_body.position, 0.22, CLAMP(-MAX_ROT, MAX_ROT, (velocity + wobble_velocity).x * 100), Sprites::SHIP);

    rightLaser.draw();
    leftLaser.draw();
    middleLaser.draw();

    Renderer::push_rectangle(0, position, DIMENSIONS);
    Physics::debug_draw_body(&ship_body);
    Physics::debug_draw_body(&shot_body);

    if (shooting) {
        Renderer::push_rectangle(1, shot_body.position, shot_body.scale,
                                 V4(0, 0.2, 1, 1));
    }
}

void PlayerPhase2::init(Vec2 pos) {
    position = pos;
    ship_body = Physics::create_body(square_shape);
    ship_body.scale = DIMENSIONS;
    ship_body.position = position;
    shot_body = Physics::create_body(square_shape);
    shot_body.position = pos;
}

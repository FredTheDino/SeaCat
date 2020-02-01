// Here is where the phase one hero is Implemented
f32 TOP_SPEED = 1.0;
f32 MIN_ACCELERATION = 0.01;
f32 TOP_ACCELERATION = 0.01;
f32 ACCELERATION_STEPS = 0.4;

#include <vector>
#include <iostream>
class PhaseOnePlayer {
   public:
    void init_PhaseOnePlayer();
    void update(f32 delta);
    void draw();

    Physics::Body player_body;

   private:
    void jerk(Vec2 *new_vector);
    void move();
    void accelerate();

    f32 cos_acc_mod(Vec2 *currSpeed);

    bool space_movement = true;

    // Vec2i current_pos = (0,0);
    // Vec2 acceleration_target = (0,0);
    // Vec2 current_speed = (0,0);
};

// Constuct.
void PhaseOnePlayer::init_PhaseOnePlayer() {
    Vec2 points[] = {V2(0, 0), V2(0, 0.5), V2(0.5, 0.5), V2(0.5, 0)};
    Physics::ShapeID my_shape = Physics::add_shape(LEN(points), points);
    player_body = Physics::create_body(my_shape, 0b110);
    player_body.scale = V2(1, 1) * 0.20;
}

// Update
void PhaseOnePlayer::update(f32 delta) {
    static bool show_control_controls = true;
    if (Util::begin_tweak_section("phase1_controls", &show_control_controls)) {
        Util::tweak("TOP_SPEED", &TOP_SPEED);
        Util::tweak("TOP_ACCELERATION", &TOP_ACCELERATION);
        Util::tweak("ACCELERATION_STEPS", &ACCELERATION_STEPS);
    }
    Util::end_tweak_section(&show_control_controls);

    using namespace Input;
    Vec2 vel_target = V2(0, 0);
    if (down(Name::UP)) vel_target.y += ACCELERATION_STEPS;
    if (down(Name::DOWN)) vel_target.y -= ACCELERATION_STEPS;
    if (down(Name::LEFT)) vel_target.x -= ACCELERATION_STEPS;
    if (down(Name::RIGHT)) vel_target.x += ACCELERATION_STEPS;

    jerk(&vel_target);
    accelerate();
    move();
}

// Increase the current acceleration Edward
void PhaseOnePlayer::jerk(Vec2 *new_acc) {
    Vec2 acc_temp = *new_acc;

    acc_temp.x = CLAMP(-TOP_ACCELERATION, TOP_ACCELERATION, acc_temp.x);
    acc_temp.y = CLAMP(-TOP_ACCELERATION, TOP_ACCELERATION, acc_temp.y);

    player_body.acceleration = acc_temp;
}

// Draw the Hero
void PhaseOnePlayer::draw() {
    Physics::debug_draw_body(&player_body);
    draw_sprite(2, player_body.position, 0.3, 0, Sprites::CAT);
}

// Calc and Change the current speed by the acc and cos(max_speed - curr_speed)
void PhaseOnePlayer::accelerate() {
    Vec2 vel_temp = player_body.velocity;
    vel_temp += player_body.acceleration * cos_acc_mod(&vel_temp);

    vel_temp.x = CLAMP(-TOP_SPEED, TOP_SPEED, vel_temp.x);
    vel_temp.y = CLAMP(-TOP_SPEED, TOP_SPEED, vel_temp.y);

    player_body.velocity = vel_temp;
}

// Move by the curr speed
void PhaseOnePlayer::move() {
    Vec2 current_vel = player_body.velocity;
    player_body.position = current_vel + player_body.position;
}

// Calc the cos(of max speed - current speed)
f32 PhaseOnePlayer::cos_acc_mod(Vec2 *currSpeed) {
    // What % of top speed is achived?
    f32 speed_procantage_x = (currSpeed->x / TOP_SPEED);
    f32 speed_procantage_y = (currSpeed->y / TOP_SPEED);
    if ((speed_procantage_x + speed_procantage_y) <= MIN_ACCELERATION) {
        return MIN_ACCELERATION;
    } else {
        return ((sin(speed_procantage_x * (PI / 2)) +
                 sin(speed_procantage_y * (PI / 2))) /
                2);
    }
}

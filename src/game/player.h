f32 cos_acc_mod(Vec2 *curr_speed);

struct PlayerPhase1 : public Logic::Entity {
    Physics::Body body;

    Vec2 position;
    f32 max_velocity = 1.0;
    f32 min_acceleration = 0.01;
    f32 max_acceleration = 0.01;
    f32 acceleration_steps = 0.4;

    void update(f32 delta) override;
    void draw() override;
    void init();
};

struct PlayerPhase2 : public Logic::Entity {
    Physics::Body ship_body;
    Physics::Body shot_body;

    Vec2 position;
    Vec2 velocity = V2(0, 0);
    f32 max_velocity = 0.12;
    f32 control = 4;  // higher => better control
    const Vec2 DIMENSIONS = V2(0.1, 0.1);

    u32 wobble_speed = 10;
    f32 wobble_amp = 0.015;

    f32 shot_width = 0.05;
    f32 shot_held_for = 0;
    f32 shot_held_target = 0.5;
    f32 shot_current_shot_length = 0;
    f32 shot_length = 1;
    bool shooting = false;

    void update(f32 delta) override;
    void draw() override;
    void init(Vec2 pos = V2(0, 0));
};

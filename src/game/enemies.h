Vec2 enemy_shape_points[] = { V2(0, 0), V2(1, 0), V2(1, 1), V2(0, 1) };
Physics::ShapeID enemy_shape;

enum class EnemyType {
    SMALL,
    BOSS
};

struct Enemy {
    Enemy(Vec2 position, int hp) :
        position(position),
        hp(hp),
        time(0),
        body(Physics::create_body(enemy_shape)) {}

    virtual void update(float delta) = 0;

    Vec2 position;
    int hp;
    float time;
    Physics::Body body;
};

struct SmallEnemy : Enemy {
    SmallEnemy(Vec2 position) : Enemy(position, 10) {
        body.scale = V2(0.25f, 0.25f);
    }

    void update(float delta) {
        time += delta;

        Vec2 dir;

        if (time < 10) {
            Vec2 goal = circling_center + V2(cos(time), sin(time)) * circling_radius;
            body.acceleration = normalize(goal - body.position) * acceleration;
        } else if (time >= 10 && charge_dir == V2(0, 0)) {
            charge_dir = normalize(circling_center - body.position) * charge_speed;
            body.acceleration = V2(0, 0);
            body.velocity = dir;
        } else if (time < 11) {
            // Charge
        } else {
            time = 0;
        }

        body.velocity += body.acceleration * delta;
        body.position += body.velocity * delta;
    }

    float circling_radius = 1;
    float acceleration = 1;
    float charge_speed = 2;
    Vec2 circling_center = V2(0, 0);
    Vec2 charge_dir = V2(0, 0);
};

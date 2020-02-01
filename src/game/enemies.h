Vec2 enemy_shape_points[] = { V2(0, 0), V2(1, 0), V2(1, 1), V2(0, 1) };
Physics::ShapeID enemy_shape;

Vec2 target = V2(0, 0);

enum class EnemyType {
    AGGRO,
    BOSS
};

struct Enemy : public Logic::Entity {
    Vec2 position;
    int hp;
    float time;
    Physics::Body body;
};

struct AggroEnemy : public Enemy {

    void update(float delta) {
        time += delta;

        if (time < 10) {
            Vec2 goal = circling_center + V2(cos(time), sin(time)) * circling_radius;
            body.velocity = goal - body.position;
            if (length(body.velocity) > 1)
                body.velocity = normalize(body.velocity);
            body.velocity *= speed;
        } else if (time >= 10 && !charging) {
            charging = true;
            body.velocity = normalize(circling_center - body.position) * charge_speed;
        } else if (time < 10.9) {
            // Charge
        } else {
            time = 0;
            charging = false;
        }

        body.position += body.velocity * delta;
    }

    void draw () {
        Renderer::push_rectangle(1, body.position, body.scale, V4(1, 0, 0, 1));
    }

    float circling_radius = 1;
    float speed = 2;
    float charge_speed = 2;
    Vec2& circling_center = target;
    bool charging = false;
    REGISTER_FIELDS(AGGRO_ENEMY, AggroEnemy, circling_radius, speed, charge_speed);
};

void aggro_enemy_init(AggroEnemy& aggro_enemy, Vec2 position=V2(0, 0)) {

    aggro_enemy.position = position;
    aggro_enemy.hp = 10;
    aggro_enemy.time = 0;
    aggro_enemy.body = Physics::create_body(enemy_shape);
    aggro_enemy.body.scale = V2(0.25, 0.25);
    aggro_enemy.body.position = position;
}

struct FloofEnemy : public Enemy {
    void update(float delta) {
        time += delta;

        body.velocity = V2(cos(time) * speed, -speed);
        body.position += body.velocity * delta;
    }

    void draw () {
        Renderer::push_rectangle(1, body.position, body.scale, V4(0, 0.7, 0.7, 1));
    }

    float speed = 0.2;
    Vec2& circling_center = target;
    bool charging = false;
    REGISTER_FIELDS(FLOOF_ENEMY, FloofEnemy, speed);
};

void floof_enemy_init(FloofEnemy& floof_enemy, Vec2 position=V2(0, 0)) {

    floof_enemy.position = position;
    floof_enemy.hp = 10;
    floof_enemy.time = 0;
    floof_enemy.body = Physics::create_body(enemy_shape);
    floof_enemy.body.scale = V2(0.25, 0.25);
    floof_enemy.body.position = position;
}

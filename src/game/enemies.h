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

        if (time < idle_time + time_offset) {
            // Circle around player
            Vec2 goal = circling_center + V2(cos(time), sin(time)) * circling_radius;
            Vec2 old_velocity = body.velocity;
            body.velocity = goal - body.position;
            if (length(body.velocity) > 1)
                body.velocity = normalize(body.velocity);
            body.velocity *= speed;
            body.velocity = LERP(old_velocity, 3*delta, body.velocity);
        } else if (time >= idle_time + time_offset && !charging) {
            // Prepare charge
            charging = true;
            body.velocity = normalize(circling_center - body.position) * speed;
        } else if (time < idle_time + time_offset + charge_time) {
            // Charge
        } else {
            // Reset cycle
            time = fmod(idle_time + time_offset + PI, 2*PI);
            time_offset = 2*PI*random_real();
            charging = false;
        }

        body.position += body.velocity * delta;
    }

    void draw () {
        Renderer::push_rectangle(1, body.position, body.scale, V4(0.7, 0, 0, 1));
    }

    bool charging = false;
    float charge_time = 1.0;
    float idle_time = 16;
    float time_offset = 2*PI*random_real();
    float circling_radius = 1;
    float speed = 2;
    Vec2& circling_center = target;
    REGISTER_FIELDS(AGGRO_ENEMY, AggroEnemy, circling_radius, speed, speed);
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

struct GloopEnemy : public Enemy {

    void update(float delta);
    void draw ();

    float speed = 0.2;
    Vec2& player_pos = target;
    REGISTER_FIELDS(GLOOP_ENEMY, GloopEnemy, speed);
};

struct GloopBullet : public Enemy {
    void update(float delta) {
        time += delta;
        body.position += body.velocity * delta;

        if (time > 10) {
            Logic::remove_entity(id);
        }
    }

    void draw () {
        Renderer::push_rectangle(1, body.position, body.scale, V4(0, 0.7, 0, 1));
    }

    float speed = 0.5;
    float rotation = 0;
    REGISTER_FIELDS(GLOOP_BULLET, GloopBullet, speed);
};

void gloop_bullet_init(GloopBullet& gloop_bullet, GloopEnemy& shooter) {

    gloop_bullet.hp = 10;
    gloop_bullet.time = 0;
    gloop_bullet.body = Physics::create_body(enemy_shape);
    gloop_bullet.body.scale = V2(0.10, 0.10);
    gloop_bullet.body.position = shooter.body.position;
    gloop_bullet.body.velocity = normalize(shooter.player_pos - shooter.body.position) * gloop_bullet.speed;
    gloop_bullet.body.rotation = atan2(gloop_bullet.body.velocity.y, gloop_bullet.body.velocity.x);
}

void GloopEnemy::update(float delta) {
    time += delta;

    body.velocity = V2(-sin(time) * speed, -speed);
    body.position += body.velocity * delta;

    if (time > 8) {
        time = 0;

        GloopBullet bullet;
        gloop_bullet_init(bullet, *this);
        Logic::add_entity(bullet);
    }
}

void GloopEnemy::draw () {
    Renderer::push_rectangle(1, body.position, body.scale, V4(0, 0.7, 0, 1));
}

void gloop_enemy_init(GloopEnemy& gloop_enemy, Vec2 position=V2(0, 0)) {

    gloop_enemy.position = position;
    gloop_enemy.hp = 10;
    gloop_enemy.time = 0;
    gloop_enemy.body = Physics::create_body(enemy_shape);
    gloop_enemy.body.scale = V2(0.25, 0.25);
    gloop_enemy.body.position = position;
}

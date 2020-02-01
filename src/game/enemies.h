Vec2 enemy_shape_points[] = { V2(0, 0), V2(1, 0), V2(1, 1), V2(0, 1) };
Physics::ShapeID enemy_shape;

Vec2 target = V2(0, 0);

enum EnemyType {
    AGGRO = 0,
    FLOOF = 1,
    GLOOP = 2,
    BOSS = 3
};

struct Enemy : public Logic::Entity {
    Vec2 position;
    float hp;
    float time;
    Physics::Body body;

    virtual bool is_dead() {
        return hp <= 0;
    };
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
        draw_sprite(1, body.position, 0.5, 0, Sprites::SPIKE);
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
        draw_sprite(1, body.position, 0.5, 0, Sprites::FLOOF);
    }

    float speed = 0.2;
    REGISTER_FIELDS(FLOOF_ENEMY, FloofEnemy, speed);
};

void floof_enemy_init(FloofEnemy& floof_enemy, Vec2 position=V2(0, 0)) {

    floof_enemy.position = position;
    floof_enemy.hp = 10;
    floof_enemy.time = 2*PI*random_real();
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
    }

    void draw () {
        draw_sprite(1, body.position, 1, 0, sprite);
    }

    bool is_dead() override {
        return hp <= 0 || time > 10;
    }

    float speed = 0.5;
    float rotation = 0;
    Sprites sprite = (random_bit() ? Sprites::GLOOP_PEW_1 : Sprites::GLOOP_PEW_2);
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
    draw_sprite(1, body.position, 0.5, 0, Sprites::GLOOP);
}

void gloop_enemy_init(GloopEnemy& gloop_enemy, Vec2 position=V2(0, 0)) {

    gloop_enemy.position = position;
    gloop_enemy.hp = 10;
    gloop_enemy.time = 0;
    gloop_enemy.body = Physics::create_body(enemy_shape);
    gloop_enemy.body.scale = V2(0.25, 0.25);
    gloop_enemy.body.position = position;
}

struct Spawner {

    void update(float delta) {

        if (paused) return;

        for (int i = enemies.size() - 1; i >= 0; i--) {
            Enemy *enemy = Logic::fetch_entity<Enemy>(enemies[i]);
            if (enemy->is_dead()) {
                Logic::remove_entity(enemies[i]);
                enemies.erase(enemies.begin()+i);
            }
        }

        time += delta;

        switch (phase) {
            case 1:
                if (time - last_spawn[EnemyType::FLOOF] > 8) {
                    last_spawn[EnemyType::FLOOF] = time;
                    spawn_floof();
                }
                break;
            case 2:
                if (time - last_spawn[EnemyType::AGGRO] > 20) {
                    last_spawn[EnemyType::AGGRO] = time;
                    spawn_aggro();
                }
                if (time - last_spawn[EnemyType::FLOOF] > 8) {
                    last_spawn[EnemyType::FLOOF] = time;
                    spawn_floof();
                }
                if (time - last_spawn[EnemyType::GLOOP] > 20) {
                    last_spawn[EnemyType::GLOOP] = time;
                    spawn_gloop();
                }
                break;
            default:
                LOG("Invalid spawner phase!!\n");
                break;
        }
    }

    void set_phase(int phase) {
        time = 0;
        this->phase = phase;
        if (phase == 1) {
            last_spawn[EnemyType::FLOOF] = 4;
        }
        if (phase == 2) {
            last_spawn[EnemyType::AGGRO] = -14;
            last_spawn[EnemyType::FLOOF] = 24;
            last_spawn[EnemyType::GLOOP] = -4;
        }
    }

    void set_paused(bool paused) {
        this->paused = paused;
    }

    void spawn_aggro() {
        AggroEnemy aggro_enemy;
        aggro_enemy_init(aggro_enemy, V2(2*random_real() - 1, 2));
        enemies.push_back(Logic::add_entity(aggro_enemy));
    }

    void spawn_floof() {
        FloofEnemy floof_enemy;
        floof_enemy_init(floof_enemy, V2(2*random_real() - 1, 2));
        enemies.push_back(Logic::add_entity(floof_enemy));
    }

    void spawn_gloop() {
        GloopEnemy gloop_enemy;
        gloop_enemy_init(gloop_enemy, V2(2*random_real() - 1, 2));
        enemies.push_back(Logic::add_entity(gloop_enemy));
    }

    std::vector<Logic::EntityID> enemies;
private:
    int phase = 0;
    bool paused = false;
    float time = 0;
    float last_spawn[EnemyType::BOSS];
};

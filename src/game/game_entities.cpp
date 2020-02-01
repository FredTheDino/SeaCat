void AggroEnemy::update(float delta) {
    time += delta;

    if (time < idle_time + time_offset) {
        // Circle around player
        Vec2 goal =
            circling_center + V2(cos(time), sin(time)) * circling_radius;
        Vec2 old_velocity = body.velocity;
        body.velocity = goal - body.position;
        if (length(body.velocity) > 1)
            body.velocity = normalize(body.velocity);
        body.velocity *= speed;
        body.velocity = LERP(old_velocity, 3 * delta, body.velocity);
    } else if (time >= idle_time + time_offset && !charging) {
        // Prepare charge
        charging = true;
        body.velocity = normalize(circling_center - body.position) * speed;
    } else if (time < idle_time + time_offset + charge_time) {
        // Charge
    } else {
        // Reset cycle
        time = fmod(idle_time + time_offset + PI, 2 * PI);
        time_offset = 2 * PI * random_real();
        charging = false;
    }

    body.position += body.velocity * delta;
}

void AggroEnemy::draw() {
    draw_sprite(1, body.position, 0.5, 0, Sprites::SPIKE);
}

void aggro_enemy_init(AggroEnemy& aggro_enemy, Vec2 position = V2(0, 0)) {
    aggro_enemy.position = position;
    aggro_enemy.hp = 10;
    aggro_enemy.time = 0;
    aggro_enemy.body = Physics::create_body(square_shape);
    aggro_enemy.body.scale = V2(0.25, 0.25);
    aggro_enemy.body.position = position;
}

void FloofEnemy::update(float delta) {
    time += delta;

    body.velocity = V2(cos(time) * speed, -speed);
    body.position += body.velocity * delta;
}

void FloofEnemy::draw() {
    draw_sprite(1, body.position, 0.5, 0, Sprites::FLOOF);
}

void floof_enemy_init(FloofEnemy& floof_enemy, Vec2 position = V2(0, 0)) {
    floof_enemy.position = position;
    floof_enemy.hp = 10;
    floof_enemy.time = 2 * PI * random_real();
    floof_enemy.body = Physics::create_body(square_shape);
    floof_enemy.body.scale = V2(0.25, 0.25);
    floof_enemy.body.position = position;
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

void GloopEnemy::draw() {
    draw_sprite(1, body.position, 0.5, 0, Sprites::GLOOP);
}

void gloop_enemy_init(GloopEnemy& gloop_enemy, Vec2 position = V2(0, 0)) {
    gloop_enemy.position = position;
    gloop_enemy.hp = 10;
    gloop_enemy.time = 0;
    gloop_enemy.body = Physics::create_body(square_shape);
    gloop_enemy.body.scale = V2(0.25, 0.25);
    gloop_enemy.body.position = position;
}

void GloopBullet::update(float delta) {
    time += delta;
    body.position += body.velocity * delta;
}

void GloopBullet::draw() { draw_sprite(1, body.position, 1, 0, sprite); }

bool GloopBullet::is_dead() { return hp <= 0 || time > 10; }

void gloop_bullet_init(GloopBullet& gloop_bullet, GloopEnemy& shooter) {
    gloop_bullet.hp = 10;
    gloop_bullet.time = 0;
    gloop_bullet.body = Physics::create_body(square_shape);
    gloop_bullet.body.scale = V2(0.10, 0.10);
    gloop_bullet.body.position = shooter.body.position;
    gloop_bullet.body.velocity =
        normalize(shooter.player_pos - shooter.body.position) *
        gloop_bullet.speed;
    gloop_bullet.body.rotation =
        atan2(gloop_bullet.body.velocity.y, gloop_bullet.body.velocity.x);
}

void Cog::update(f32 delta) {
    time += delta;
    // center_position += velocity * delta;

    position = center_position +
               V2(cos(time * rot_speed), sin(time * rot_speed)) * rot_amp;
    body.position = position;
}

void Cog::draw() {
    // Renderer::push_point(0, position, V4(1, 1, 1, 0.66), size);
    // Physics::debug_draw_body(&body);
    draw_sprite(2, position, size * 3.5, 0.0, Sprites::COG);
}

void cog_init(Cog& cog, Vec2 position = V2(0, 0)) {
    cog.center_position = position;
    cog.hp = 1;
    cog.time = 0;
    cog.body = Physics::create_body(square_shape);
    cog.body.scale = V2(cog.size, cog.size);
    cog.body.position = position;
}

void Spawner::update(float delta) {
    if (paused) return;

    for (int i = entities.size() - 1; i >= 0; i--) {
        GameEntity* entity = Logic::fetch_entity<GameEntity>(entities[i]);
        if (entity->is_dead()) {
            Logic::remove_entity(entities[i]);
            entities.erase(entities.begin() + i);
        }
    }

    time += delta;

    switch (phase) {
        case 1:
            if (time - last_spawn[EntityType::FLOOF] > 4) {
                last_spawn[EntityType::FLOOF] = time;
                spawn_floof_phase1();
            }
            break;
        case 2:
            if (time - last_spawn[EntityType::AGGRO] > 20) {
                last_spawn[EntityType::AGGRO] = time;
                spawn_aggro();
            }
            if (time - last_spawn[EntityType::FLOOF] > 8) {
                last_spawn[EntityType::FLOOF] = time;
                spawn_floof();
            }
            if (time - last_spawn[EntityType::GLOOP] > 20) {
                last_spawn[EntityType::GLOOP] = time;
                spawn_gloop();
            }
            break;
        // cogs
        case 11:
        case 12:
            if (time - last_spawn[EntityType::COG] > 2) {
                last_spawn[EntityType::COG] = time;
                spawn_cog();
            }
            break;
        default:
            LOG("Invalid spawner phase!!\n");
            break;
    }
}

void Spawner::set_phase(int phase) {
    time = 0;
    this->phase = phase;
    if (phase == 1) {
        last_spawn[EntityType::FLOOF] = 0;
    }
    if (phase == 2) {
        last_spawn[EntityType::AGGRO] = -14;
        last_spawn[EntityType::FLOOF] = 24;
        last_spawn[EntityType::GLOOP] = -4;
    }
}

void Spawner::set_paused(bool paused) { this->paused = paused; }

void Spawner::spawn_aggro() {
    AggroEnemy aggro_enemy;
    aggro_enemy_init(aggro_enemy, V2(2 * random_real() - 1, 2));
    entities.push_back(Logic::add_entity(aggro_enemy));
}

void Spawner::spawn_floof_phase1() {
    FloofEnemy floof_enemy;

    Vec2 camera_pos = Renderer::get_camera()->position;
    float camera_zoom = Renderer::get_camera()->zoom;
    float camera_ratio = Renderer::get_window_aspect_ratio();

    floof_enemy_init(floof_enemy,
                     camera_pos + V2(1, 1 / camera_ratio) / camera_zoom);
    floof_enemy.speed = 0;
    entities.push_back(Logic::add_entity(floof_enemy));
}

void Spawner::spawn_floof() {
    FloofEnemy floof_enemy;
    floof_enemy_init(floof_enemy, V2(2 * random_real() - 1, 2));
    entities.push_back(Logic::add_entity(floof_enemy));
}

void Spawner::spawn_gloop() {
    GloopEnemy gloop_enemy;
    gloop_enemy_init(gloop_enemy, V2(2 * random_real() - 1, 2));
    entities.push_back(Logic::add_entity(gloop_enemy));
}

void Spawner::spawn_cog() {
    Cog cog;
    cog_init(cog, V2(0, 0));
    entities.push_back(Logic::add_entity(cog));
}

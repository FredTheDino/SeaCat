#define ENTITY_DEBUG 0

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
#if ENTITY_DEBUG
    Physics::debug_draw_body(&body);
#endif
    draw_sprite(1, body.position, 0.25, 0, Sprites::SPIKE);
}

void aggro_enemy_init(AggroEnemy& aggro_enemy, Vec2 position = V2(0, 0)) {
    aggro_enemy.position = position;
    aggro_enemy.hp = 10;
    aggro_enemy.time = 0;
    aggro_enemy.body = Physics::create_body(square_shape);
    aggro_enemy.body.scale = V2(1, 1) * 0.06;
    aggro_enemy.body.position = position;
}

void sinus_strat(FloofEnemy *self, f32 delta) {
    self->body.velocity = V2(cos(self->time) * self->speed, -self->speed);
    self->body.position += self->body.velocity * delta;
}

void target_strat(FloofEnemy *self, f32 delta) {
    self->body.position += self->body.velocity * delta;
}

void FloofEnemy::update(float delta) {
    time += delta;
    strategy(this, delta);
    position = body.position;

    if (length(position - target) > 2/Renderer::get_camera(0)->aspect_ratio) {
        hp = 0;
    }
}

void FloofEnemy::draw() {
#if ENTITY_DEBUG
    Physics::debug_draw_body(&body);
#endif
    draw_sprite(1, body.position, 0.25, 0, Sprites::FLOOF);
}

void floof_enemy_init(FloofEnemy& floof_enemy, Vec2 position = V2(0, 0)) {
    floof_enemy.position = position;
    floof_enemy.hp = 10;
    floof_enemy.time = 2 * PI * random_real();
    floof_enemy.body = Physics::create_body(square_shape);
    floof_enemy.body.scale = V2(1, 1) * 0.08;
    floof_enemy.body.position = position;
    floof_enemy.strategy = sinus_strat;
}

void floof_enemy_init_linear(FloofEnemy& floof_enemy, Vec2 position, Vec2 target) {
    floof_enemy_init(floof_enemy, position);
    floof_enemy.strategy = target_strat;
    floof_enemy.body.velocity = normalize(target - position) * floof_enemy.speed;
}

void GloopEnemy::update(float delta) {
    time += delta;

    body.velocity = V2(-sin(time) * speed, -speed);
    body.position += body.velocity * delta;

    if (time > 8) {
        time = 0;

        enemy_spawner.spawn_gloop_bullet(*this);
    }
}

void GloopEnemy::draw() {
#if ENTITY_DEBUG
    Physics::debug_draw_body(&body);
#endif
    draw_sprite(1, body.position, 0.25, 0, Sprites::GLOOP);
}

void gloop_enemy_init(GloopEnemy& gloop_enemy, Vec2 position = V2(0, 0)) {
    gloop_enemy.position = position;
    gloop_enemy.hp = 10;
    gloop_enemy.time = 0;
    gloop_enemy.body = Physics::create_body(square_shape);
    gloop_enemy.body.scale = V2(1, 1) * 0.08;
    gloop_enemy.body.position = position;
}

void GloopBullet::update(float delta) {
    time += delta;
    body.position += body.velocity * delta;
    position = body.position;

    if (length(position - target) > 2/Renderer::get_camera(0)->aspect_ratio) {
        hp = 0;
    }
}

void GloopBullet::draw() {
#if ENTITY_DEBUG
    Physics::debug_draw_body(&body);
#endif
    draw_sprite(1, body.position, 0.50, 0, sprite);
}

bool GloopBullet::is_dead() { return hp <= 0 || time > 100; }

void gloop_bullet_init(GloopBullet& gloop_bullet, GloopEnemy& shooter) {
    gloop_bullet.hp = 1000;
    gloop_bullet.time = 0;
    gloop_bullet.body = Physics::create_body(square_shape);
    gloop_bullet.body.scale = V2(1, 1) * 0.03;
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

    rotation = 0.35 * ((u32)(((Logic::now())/0.5f) - init_rotations));
}

void Cog::draw() {
#if ENTITY_DEBUG
    Physics::debug_draw_body(&body);
#endif
    draw_sprite(2, position, size * 1.5, rotation, Sprites::COG);
}

void cog_init(Cog& cog, Vec2 position = V2(0, 0)) {
    cog.center_position = position;
    cog.hp = 1;
    cog.time = 0;
    cog.body = Physics::create_body(square_shape);
    cog.body.scale = V2(cog.size, cog.size) * 0.3;
    cog.body.position = position + V2(cog.rot_amp, 0);
}

// TODO: Cycles for shooting
void Boss::update(f32 delta) {
	time += delta;

    if (time > next_bullet) { // TODO: Randomize
        time = 0;
		next_bullet = random_real(0, 3);

        enemy_spawner.spawn_boss_bullet();
    }
}

void Boss::draw() {
#if ENTITY_DEBUG
	Physics::debug_draw_body(&body_left);
	Physics::debug_draw_body(&body);
	Physics::debug_draw_body(&body_right);
#endif
	draw_sprite(2, V2(x, y), size, 0, Sprites::BOSS);
}

// TEST
Logic::EntityID bossID;

void boss_init(Boss& boss) {
    boss.hp = 5;
	boss.size = 2;
	boss.x = 0;
	boss.y = 0.6;
	
    boss.body_left = Physics::create_body(triangle_shape);
    boss.body_left.scale = V2(boss.size, boss.size * 0.5);
    boss.body_left.position = V2(boss.x - 0.8, boss.y + 0.2);

    boss.body  = Physics::create_body(square_shape);
    boss.body.scale = V2(boss.size * 0.2, boss.size * 0.9);
    boss.body.position = V2(boss.x - boss.size * 0.05,  boss.y + boss.size * 0.13);

    boss.body_right = Physics::create_body(triangle_shape);
    boss.body_right.scale = V2(-boss.size, boss.size * 0.8);
    boss.body_right.position = V2(boss.x + 0.7, boss.y + 0.2);
}

void BossBullet::update(float delta) {
    time += delta;
    body.position += body.velocity * delta;
    position = body.position;

    if (length(position - target) > 2/Renderer::get_camera(0)->aspect_ratio) {
        hp = 0;
    }
}

void BossBullet::draw() {
#if ENTITY_DEBUG
    Physics::debug_draw_body(&body);
#endif
    draw_sprite(1, body.position, size, rotation, sprite);
}

bool BossBullet::is_dead() { return hp <= 0 || time > 100; }

void boss_bullet_init(BossBullet& bullet) {
    bullet.hp = 1000;
    bullet.time = 0;
	bullet.size = random_real(0.7, 1.5);
    bullet.body = Physics::create_body(square_shape);
    bullet.body.position = V2(random_real(-1, 1), 2);
    bullet.body.velocity = bullet.velocity; // Randomize?
    bullet.body.rotation = random_real(0, PI);
}

void Wall::update(f32 delta) {
}

void Wall::draw() {
#if ENTITY_DEBUG
	Physics::debug_draw_body(&body);
#endif
	Renderer::push_rectangle(3, position, size, V4(0, 0, 0, 1));
}

void wall_init(Wall& wall, Vec2 pos) { 
    wall.body  = Physics::create_body(square_shape);
	wall.position = pos;
	wall.size = V2(2, 4);
	wall.body.position = pos;
	wall.body.scale = wall.size;
}

void Spawner::update(float delta) {
    if (paused) return;

    for (int i = entities.size() - 1; i >= 0; i--) {
        GameEntity* entity = Logic::fetch_entity<GameEntity>(entities[i]);
        if (entity->is_dead()) {
            if (phase == 2) {
                hitEnemy.position = entity->body.position;
                for (u32 i = 0; i < 300; i++) hitEnemy.spawn();
                cog_spawner.spawn_cog(entity->body.position - V2(Cog().rot_amp, 0));
            }
            Logic::remove_entity(entities[i]);
            entities.erase(entities.begin() + i);
        }
    }

    time += delta;

    switch (phase) {
        case 1:
            if (time - last_spawn[EntityType::FLOOF] > 3) {
                last_spawn[EntityType::FLOOF] = time;
                spawn_floof_phase1();
            }
            break;
        case 2:
            if (time - last_spawn[EntityType::AGGRO] > 18) {
                last_spawn[EntityType::AGGRO] = time;
                spawn_aggro();
            }
            if (time - last_spawn[EntityType::FLOOF] > 8) {
                last_spawn[EntityType::FLOOF] = time;
                spawn_floof();
            }
            if (time - last_spawn[EntityType::GLOOP] > 18) {
                last_spawn[EntityType::GLOOP] = time;
                spawn_gloop();
            }
            break;
		case 3:
			if (!last_spawn[EntityType::BOSS]) {
				last_spawn[EntityType::BOSS] = 1;
				spawn_boss();
			}
			break;
        // cogs
        case 11:
        case 12:
            if (time - last_spawn[EntityType::COG] > 3) {
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
	if (phase == 3) {
		last_spawn[EntityType::BOSS] = 0;
	}
}

void Spawner::set_paused(bool paused) { this->paused = paused; }

void Spawner::clear() {
    entities.clear();
    Logic::clear_entitysystem();
}

void Spawner::spawn_aggro() {
    AggroEnemy aggro_enemy;
    aggro_enemy_init(aggro_enemy, V2(random_real(-0.8, 0.8), 1));
    entities.push_back(Logic::add_entity(aggro_enemy));
}

void Spawner::spawn_floof_phase1() {
    FloofEnemy floof_enemy;

    Vec2 camera_pos = Renderer::get_camera()->position;
    float radius = length(V2(1, (1.0 / Renderer::get_window_aspect_ratio())));
    Vec2 spawn_pos = random_unit_vec2() * radius - camera_pos;
    floof_enemy_init_linear(floof_enemy, spawn_pos,
                            Renderer::get_camera(0)->position);
    floof_enemy.speed = 2;
    entities.push_back(Logic::add_entity(floof_enemy));
}

void Spawner::spawn_floof() {
    FloofEnemy floof_enemy;
    floof_enemy_init(floof_enemy, V2(random_real(-0.8, 0.8), 1));
    entities.push_back(Logic::add_entity(floof_enemy));
}

void Spawner::spawn_gloop() {
    GloopEnemy gloop_enemy;
    gloop_enemy_init(gloop_enemy, V2(random_real(-0.8, 0.8), 1));
    entities.push_back(Logic::add_entity(gloop_enemy));
}

void Spawner::spawn_gloop_bullet(GloopEnemy& shooter) {
    GloopBullet bullet;
    gloop_bullet_init(bullet, shooter);
    entities.push_back(Logic::add_entity(bullet));
}

void Spawner::spawn_cog() {
    Vec2 camera_pos = Renderer::get_camera()->position;
    float radius = length(V2(1, 1.0 / Renderer::get_window_aspect_ratio()));
    Vec2 spawn_pos = random_unit_vec2() * radius - camera_pos;
    spawn_cog(spawn_pos);
}

void Spawner::spawn_cog(Vec2 spawn_pos) {
    Cog cog;
    cog_init(cog, spawn_pos);
    entities.push_back(Logic::add_entity(cog));
}

void Spawner::spawn_boss() {
    Boss boss;
    boss_init(boss);
	bossID = Logic::add_entity(boss);
	Wall left, right;
	wall_init(left, V2(-2, 0));
	wall_init(right, V2(1.95, 0));
	Logic::add_entity(left);
	Logic::add_entity(right);
}

void Spawner::spawn_boss_bullet() {
    BossBullet bullet;
    boss_bullet_init(bullet);
    entities.push_back(Logic::add_entity(bullet));
}

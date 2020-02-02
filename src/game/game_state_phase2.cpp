namespace Phase2 {

Logic::EntityID player_id;

const s32 progress_cap = 15;
s32 progress;

void setup();
void enter();
void update(f32 now, f32 delta);
void draw();
void exit();

void setup() {
    square_shape = Physics::add_shape(LEN(square_shape_points), square_shape_points);
}

Logic::LogicID leave_id;

void enter() {
    current_exit();
    Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
    Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
    current_exit = exit;

    Renderer::get_camera()->position = V2(0, 0);

    PlayerPhase2 player;
    player.init();
    player_id = Logic::add_entity(player);

    enemy_spawner.set_phase(2);
    enemy_spawner.set_paused(false);

    cog_spawner.set_phase(12);
    cog_spawner.set_paused(false);

    progress = 0;
    auto leave = []() {
        if (progress >= progress_cap) {
            Cutscene::enter(2);
        }
    };
    leave_id = Logic::add_callback(Logic::POST_DRAW, leave, 0.0, Logic::FOREVER);
    init_hit_particles();
}

void update(f32 delta, f32 now) {
    hitEnemy.update(delta);
    enemy_spawner.update(delta);
    cog_spawner.update(delta);

    PlayerPhase2 *player = Logic::fetch_entity<PlayerPhase2>(player_id);
    for (s32 i = enemy_spawner.entities.size() - 1; i >= 0; i--) {
        GameEntity *enemy =
            Logic::fetch_entity<GameEntity>(enemy_spawner.entities[i]);
        if (player->shooting && Physics::check_overlap(&enemy->body,
                    &player->shot_body)) {
            enemy->hp -= delta * 15;
        }

        Physics::Overlap overlap = Physics::check_overlap(&enemy->body, &player->ship_body);
        if (overlap) {
            hitEnemy.position = (player->ship_body.position + enemy->body.position)/2;
            for (int i = 0; i < 300; i++) {
                hitEnemy.spawn();
            }
            player->velocity = overlap.normal * 0.02;
        }
    }

    float player_constrain_acceleration = 0.2;
    if (player->position.x > 1) {
        player->velocity -= V2(delta * player_constrain_acceleration, 0);
    } else if (player->position.x < -1) {
        player->velocity += V2(delta * player_constrain_acceleration, 0);
    }
    if (player->position.y > Renderer::get_window_aspect_ratio()) {
        player->velocity -= V2(0, delta * player_constrain_acceleration);
    } else if (player->position.y < -Renderer::get_window_aspect_ratio()) {
        player->velocity += V2(0, delta * player_constrain_acceleration);
    }

    for (s32 i = cog_spawner.entities.size() - 1; i >= 0; i--) {
        GameEntity *cog =
            Logic::fetch_entity<GameEntity>(cog_spawner.entities[i]);
        if (Physics::check_overlap(&cog->body, &player->ship_body)) {
            cog->hp = 0;
            progress++;
        }
    }

    target = player->position;
}

void draw() {
    // Draw background
    draw_sprite(0, V2(0, 0), 2, 0, Sprites::BACKGROUND);
    hitEnemy.draw();
}

void exit() {
    enemy_spawner.clear();
    cog_spawner.clear();
}
}  // namespace Phase2

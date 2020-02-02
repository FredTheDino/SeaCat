namespace Phase3 {

Logic::EntityID player_id;
Logic::EntityID boss_id;

Mixer::AudioID music_id;

void setup();
void enter();
void update(f32 now, f32 delta);
void draw();
void exit();
void win();

void setup() {
    square_shape = Physics::add_shape(LEN(square_shape_points), square_shape_points);
}

void enter() {
    current_exit();
    Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
    Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
    current_exit = exit;

    PlayerPhase2 player;
    player.init();
    player_id = Logic::add_entity(player);

    enemy_spawner.set_phase(3);
    enemy_spawner.set_paused(false);
    boss_id = enemy_spawner.spawn_boss();

    music_id = Mixer::play_sound(3, ASSET_BOSS_SONG, 1.0, Mixer::AUDIO_DEFAULT_GAIN, 0, 0, true);
}

void update(f32 delta, f32 now) {
    enemy_spawner.update(delta);

    PlayerPhase2 *player = Logic::fetch_entity<PlayerPhase2>(player_id);
    for (int i = enemy_spawner.entities.size() - 1; i >= 0; i--) {
        GameEntity *enemy = Logic::fetch_entity<GameEntity>(enemy_spawner.entities[i]);
        if (Physics::check_overlap(&enemy->body, &player->ship_body)) {
            Cutscene::enter(2);
        }
    }
    Boss *boss = Logic::fetch_entity<Boss>(boss_id);
    if (Physics::check_overlap(&boss->body_left, &player->ship_body) ||
            Physics::check_overlap(&boss->body_right, &player->ship_body) ||
            Physics::check_overlap(&boss->body, &player->ship_body)) {
        Cutscene::enter(2);
    }

    if (player->shooting) {
        if (Physics::check_overlap(&boss->body_left, &player->shot_body) ||
                Physics::check_overlap(&boss->body_right, &player->shot_body) ||
                Physics::check_overlap(&boss->body, &player->shot_body)) {
            boss->hp -= delta * 1;
            if (boss->hp <= 0) {
                LOG("YOU WIN");
                Cutscene::enter(0);
            }
        }
    }

    float player_constrain_acceleration = 0.2;
    if (player->position.x > 0.3) {
        player->velocity -= V2(delta * player_constrain_acceleration, 0);
    } else if (player->position.x < -0.3) {
        player->velocity += V2(delta * player_constrain_acceleration, 0);
    }
    if (player->position.y > Renderer::get_window_aspect_ratio()) {
        player->velocity -= V2(0, delta * player_constrain_acceleration);
    } else if (player->position.y < -Renderer::get_window_aspect_ratio() * 3) {
        player->velocity += V2(0, delta * player_constrain_acceleration);
    }
}

void draw() {
    // Draw background
    draw_sprite(0, V2(0, 0), 2 / Renderer::get_camera()->zoom, 0,
            Sprites::BACKGROUND);
    Renderer::push_rectangle(1, V2(1, 0), V2(0.5, 2), V4(0, 0, 0, 1));
    Renderer::push_rectangle(1, V2(-1, 0), V2(0.5, 2), V4(0, 0, 0, 1));
}

void exit() {
    Logic::remove_entity(player_id);
    enemy_spawner.clear();
    Logic::update_callback(update_id, empty_func, 0.0, Logic::FOREVER);
    Logic::update_callback(draw_id, empty_func, 0.0, Logic::FOREVER);
    Mixer::stop_sound(music_id);
}
}

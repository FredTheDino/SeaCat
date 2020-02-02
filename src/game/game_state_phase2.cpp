namespace Phase2 {

Logic::EntityID player_id;

void setup();
void enter();
void update(f32 now, f32 delta);
void draw();
void exit();

void setup() {
    square_shape =
        Physics::add_shape(LEN(square_shape_points), square_shape_points);
}

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
}

void update(f32 delta, f32 now) {
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
            player->velocity = overlap.normal * 0.02;
        }
    }

    for (s32 i = cog_spawner.entities.size() - 1; i >= 0; i--) {
        GameEntity *cog =
            Logic::fetch_entity<GameEntity>(cog_spawner.entities[i]);
        if (Physics::check_overlap(&cog->body, &player->ship_body)) {
            cog->hp = 0;
        }
    }

    target = player->position;
}

void draw() {
    // Draw background
    draw_sprite(0, V2(0, 0), 2, 0, Sprites::BACKGROUND);
}

//TODO(gu) drop entities from entity system
void exit() {}

}  // namespace Phase2

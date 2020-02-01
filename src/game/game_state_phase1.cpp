namespace Phase1 {

Logic::EntityID player_id;
Physics::Body bordere_rect_container[4];

f32 progess = 0;

Vec4 START_COLOR = V4(0.1, 0.05, 0.05, 1.0);
Vec4 END_COLOR = V4(0.3, 0.3, 0.3, 1.0);

void setup();
void enter();
void update(f32 now, f32 delta);
void draw();
void exit();

void setup() {
    // for (int num_rec_i = 0; num_rec_i < len(temp_rect);num_rec_i){
    //     if (num_rec_i%2 = 0){
    //         Vec2 points[] = { V2(0, 0), V2(0, 1), V2(5, 1), V2(5, 0) };
    //     }else{
    //         Vec2 points[] = { V2(0, 0), V2(0, 1), V2(5, 1), V2(5, 0) };
    //     }

    // }
    // Vec2 points[] = { V2(0, 0), V2(0, 1), V2(5, 1), V2(5, 0) };
    // Physics::ShapeID temp_shape = Physics::add_shape(LEN(points), points);
    // temp_rect = Physics::create_body(temp_shape, 0.0, 0b110, 0.5);
    // temp_rect.position = V2(0, 3);
}

void enter() {
    current_exit();
    Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
    Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
    current_exit = exit;

    enemy_spawner.set_phase(1);
    enemy_spawner.set_paused(false);

    cog_spawner.set_phase(11);
    cog_spawner.set_paused(false);

    PlayerPhase1 player;
    player.init();
    player_id = Logic::add_entity(player);

}

void update(f32 delta, f32 now) {
    enemy_spawner.update(delta);
    cog_spawner.update(delta);

    PlayerPhase1 *player = (PlayerPhase1 *) Logic::fetch_entity(player_id);
    for (s32 i = cog_spawner.entities.size() - 1; i >= 0; i--) {
        GameEntity *cog =
            Logic::fetch_entity<GameEntity>(cog_spawner.entities[i]);
        if (Physics::check_overlap(&cog->body, &player->body)) {
            cog->hp = 0;
            pick_up_compliment(cog->position);
            progess = CLAMP(0, 1.0, progess + 0.1);
        }
    }
    Renderer::get_camera()->position = -player->body.position;
}

void draw() {
    // Draw background
    Vec4 tint = LERP(START_COLOR, progess, END_COLOR);
    draw_sprite(0, ((PlayerPhase1 *) Logic::fetch_entity(player_id))->body.position, 2, 0, Sprites::BACKGROUND, tint);

    // Physics::Overlap curr_overlap =
    // Physics::check_overlap(&player1.player_body, &temp_rect);
    // Physics::solve(curr_overlap);
    // Physics::debug_draw_body(&temp_rect);
}

void exit() { LOG("Do stuff for the exit"); }
};  // namespace Phase1

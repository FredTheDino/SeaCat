#include "phase1_player.cpp"

namespace Phase1 {

PhaseOnePlayer player1;
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
    player1.init_PhaseOnePlayer();
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

Logic::LogicID leave_id;

void enter() {
    current_exit();
    Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
    Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
    current_exit = exit;

    enemy_spawner.set_phase(1);
    enemy_spawner.set_paused(false);

    cog_spawner.set_phase(11);
    cog_spawner.set_paused(false);

    auto leave = [&leave_id]() {
        if (progess >= 1.0)
            Cutscene::enter(1);
    };
    leave_id = Logic::add_callback(Logic::POST_DRAW, leave, 0.0, Logic::FOREVER);
}

void update(f32 delta, f32 now) {
    player1.update(delta);
    enemy_spawner.update(delta);
    cog_spawner.update(delta);

    // TODO: Check for enemy collisions

    for (s32 i = cog_spawner.entities.size() - 1; i >= 0; i--) {
        GameEntity *cog =
            Logic::fetch_entity<GameEntity>(cog_spawner.entities[i]);
        if (Physics::check_overlap(&cog->body, &player1.player_body)) {
            cog->hp = 0;
            pick_up_compliment(cog->position);
            // TODO: Fix this later
            progess = CLAMP(0, 1.0, progess + 0.1);
        }
    }
    Renderer::get_camera()->position = -player1.player_body.position;
}

void draw() {
    player1.draw();

    // Draw background
    Vec4 tint = LERP(START_COLOR, progess, END_COLOR);
    draw_sprite(0, player1.player_body.position, 2, 0, Sprites::BACKGROUND, tint);

    // Physics::Overlap curr_overlap =
    // Physics::check_overlap(&player1.player_body, &temp_rect);
    // Physics::solve(curr_overlap);
    // Physics::debug_draw_body(&temp_rect);
}

void exit() {
    Logic::remove_callback(leave_id);
    enemy_spawner.clear();
    cog_spawner.clear();
}
};  // namespace Phase1

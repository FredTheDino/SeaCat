#include "PhaseOnePlayer.cpp"

namespace Phase1 {

    PhaseOnePlayer player1;
    Physics::Body temp_rect;

    void setup();
    void enter();
    void update(f32 now, f32 delta);
    void draw();
    void exit();

    void setup(){
	player1.init_PhaseOnePlayer();
	Vec2 points[] = { V2(0, 0), V2(0, 1), V2(5, 1), V2(5, 0) };
	Physics::ShapeID temp_shape = Physics::add_shape(LEN(points), points);
	temp_rect = Physics::create_body(temp_shape, 0b110);
	temp_rect.position = V2(0, 5);

    }
    
    void enter() {
        current_exit();
        Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
        Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
	setup();
        current_exit = exit;
    }

    void update(f32 now, f32 delta) {
        LOG("Updating!\n");

	player1.update(delta);
	Renderer::get_camera()->position = -player1.player_body.position;
    }

    void draw() {
        LOG("DRAWING\n");
	player1.draw();
	Physics::debug_draw_body(&temp_rect);
    }

    void exit() {
        LOG("Do stuff for the exit");
    }
};

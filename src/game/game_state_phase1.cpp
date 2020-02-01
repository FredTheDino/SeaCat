#include "PhaseOnePlayer.cpp"

namespace Phase1 {

    PhaseOnePlayer player1;
    
    void setup();
    void enter();
    void update(f32 now, f32 delta);
    void draw();
    void exit();

    void setup(){
	player1.init_PhaseOnePlayer();
    }
    
    void enter() {
        current_exit();
        Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
        Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
	setup();
        current_exit = exit;

        spawner.set_phase(1);
        spawner.set_paused(false);
    }

    void update(f32 delta, f32 now) {
        LOG("Updating!\n");
        player1.update(delta);
        spawner.update(delta);
    }

    void draw() {
        LOG("DRAWING\n");
	player1.draw();
    }

    void exit() {
        LOG("Do stuff for the exit");
    }
};

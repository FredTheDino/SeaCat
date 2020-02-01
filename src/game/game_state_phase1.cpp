namespace Phase1 {

    void enter();
    void update(f32 now, f32 delta);
    void draw();
    void exit();

    void enter() {
        current_exit();
        Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
        Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
        current_exit = exit;
    }

    void update(f32 now, f32 delta) {
        LOG("Updating!\n");
    }

    void draw() {
        LOG("DRAWING\n");
    }

    void exit() {
        LOG("Do stuff for the exit");
    }
};

namespace Intro {

    void enter();
    void update(f32 now, f32 delta);
    void draw();
    void exit();

    Logic::LogicID timer_id;
    void enter() {
        current_exit();
        Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
        Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
        current_exit = exit;

        auto timer = []() -> void {

        };
        Logic::add_callback(Logic::POST_UPDATE, timer, 2.0);
    }

    void update(f32 now, f32 delta) {}

    void draw() {
        Renderer::draw_text("Hello...", -0.9, 0.6, 1.0, ASSET_MONACO_FONT, 0);
        Renderer::draw_text("How are you doing?", 0.9, -0.6, 1.0, ASSET_MONACO_FONT, -1.0);
    }

    void exit() {
        LOG("Do stuff for the exit");
    }
};

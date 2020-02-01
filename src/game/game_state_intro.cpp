namespace Intro {

    void enter();
    void update(f32 now, f32 delta);
    void draw();
    void exit();
    void update_cat_text(f32 now, f32 started, char *phrase);
    void update_ther_text(f32 now, f32 started, char *phrase);

    char *catPhrase1 = "Hello...";
    char *catPhrase2 = "Hello";
    char *catPhrase3 = "Hi!";
    char *therPhrase = "How are you doing?";
    char phrase1[9];
    char phrase2[19];
    f32 DELAY1 = 0.36;
    f32 DELAY2 = 0.27;
    f32 STARTED = 0;

    void update_cat_text(f32 now, f32 started, char *phrase) {
        u32 charAmount= strlen(phrase);
        u32 charsWritten = strlen(phrase1);
        if ((now - started - (charsWritten * DELAY1)) >= DELAY1 && 
                charsWritten <= charAmount) {
            phrase1[charsWritten] = phrase[charsWritten];
        }
    }

    void update_ther_text(f32 now, f32 started, char *phrase) {
        u32 charAmount= strlen(phrase);
        u32 charsWritten = strlen(phrase2);
        if ((now - started - (charsWritten * DELAY2 + strlen(phrase1) * DELAY1)) >= DELAY2 && 
                charsWritten <= charAmount) {
            phrase2[charsWritten] = phrase[charsWritten];
        }
    }

    Logic::LogicID timer_id;
    void enter() {
        current_exit();
        Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
        Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
        current_exit = exit;

        auto timer = []() -> void {

        };
        Logic::add_callback(Logic::POST_UPDATE, timer, 2.0);
        STARTED = Logic::now();
        for (int i = 0; i < 9; i++) phrase1[i] = 0;
        for (int i = 0; i < 19; i++) phrase2[i] = 0;
    }

    void update(f32 delta, f32 now) {
        update_cat_text(now, STARTED, catPhrase1);
        if (strlen(catPhrase1) == strlen(phrase1)) {
            update_ther_text(now, STARTED, therPhrase);
        }
    }

    void draw() {
        Renderer::push_rectangle(5, V2(0, 0), V2(100, 100), V4(0, 0, 0, 1));
        Renderer::draw_text(phrase1, -0.9, 0.6, 1.0, ASSET_MONACO_FONT, 0, V4(0.41, 0.63, 1, 1));
        f32 textOffset = Renderer::messure_text(therPhrase, 1.0, ASSET_MONACO_FONT).x;
        Renderer::draw_text(phrase2, 0.9 - textOffset, -0.6, 1.0, ASSET_MONACO_FONT, 0);
    }

    void exit() {
        LOG("Do stuff for the exit");
    }
};

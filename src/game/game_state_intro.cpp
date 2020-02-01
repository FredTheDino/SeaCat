namespace Intro {

    void enter(u32 state);
    void update(f32 now, f32 delta);
    void draw();
    void exit();
    void update_cat_text(f32 now, f32 started, char *phrase);
    void update_ther_text(f32 now, f32 started, char *phrase);

    const char *catPhrase0 = "Hello...";
    const char *catPhrase1 = "Hello";
    const char *catPhrase2= "Hi!";
    const char *therPhrase = "How are you doing?";
    const char *catPhrase = "";
    char phrase1[9];
    char phrase2[19];
    f32 DELAY1 = 0.36;
    f32 DELAY2 = 0.27;
    f32 STARTED = 0;

    void play_cat_sound() {
        u64 assets[] = {ASSET_VOICE2_1, ASSET_VOICE2_2, ASSET_VOICE2_3, ASSET_VOICE2_4};
        u32 sound = random_int() % 4;
        Mixer::play_sound(0, assets[sound]);
    }

    void play_ther_sound() {
        u64 assets[] = {ASSET_VOICE1_1, ASSET_VOICE1_2, ASSET_VOICE1_3, ASSET_VOICE1_4};
        u32 sound = random_int() % 4;
        Mixer::play_sound(0, assets[sound]);
    }

    void update_cat_text(f32 now, f32 started, const char *phrase) {
        u32 charAmount= strlen(phrase);
        u32 charsWritten = strlen(phrase1);
        if ((now - started - (charsWritten * DELAY1)) >= DELAY1 && 
                charsWritten < charAmount) {
            phrase1[charsWritten] = phrase[charsWritten];
            play_cat_sound();
        }
    }

    void update_ther_text(f32 now, f32 started, const char *phrase) {
        u32 charAmount= strlen(phrase);
        u32 charsWritten = strlen(phrase2);
        if ((now - started - (charsWritten * DELAY2 + strlen(phrase1) * DELAY1)) >= DELAY2 && 
                charsWritten < charAmount) {
            phrase2[charsWritten] = phrase[charsWritten];
            play_ther_sound();
        }
    }

    Logic::LogicID timer_id;
    void enter(u32 state) {
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
        switch(state) {
            case 0:
                catPhrase = catPhrase0;
                break;
            case 1:
                catPhrase = catPhrase1;
                break;
            case 2:
                catPhrase = catPhrase2;
                break;
            default:
                ASSERT(false, "faulty intro state");
        }

    }

    void update(f32 delta, f32 now) {
        update_cat_text(now, STARTED, catPhrase);
        if (strlen(catPhrase) == strlen(phrase1)) {
            update_ther_text(now, STARTED, therPhrase);
        }
    }

    void draw() {
        Renderer::draw_text(phrase1, -0.8, 0.8 * Renderer::get_window_aspect_ratio(), 1.0, ASSET_MONACO_FONT, 0, V4(0.41, 0.63, 1, 1));
        Vec2 textOffset = Renderer::messure_text(therPhrase, 1.0, ASSET_MONACO_FONT);
        Renderer::draw_text(phrase2, 0.8 - textOffset.x, -0.8 * Renderer::get_window_aspect_ratio() + textOffset.y, 1.0, ASSET_MONACO_FONT, 0);
        draw_sprite(0, V2(0, 0), 2, 0, Sprites::BACKGROUND);
    }

    void exit() {
        LOG("Do stuff for the exit");
    }
};

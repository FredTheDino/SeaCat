namespace Cutscene {

const char *cat_phrases[] = {"Hello...", "Hello", "Hi!"};
const char *cat_phrase = "";
const char *their_phrase = "How are you doing?";
const u32 CAT_BUFFET_LEN = 9;
const u32 THEIR_BUFFER_LEN = 19;
char cat_buffer[CAT_BUFFET_LEN];
char their_buffer[THEIR_BUFFER_LEN];
f32 DELAY1 = 0.36;
f32 DELAY2 = 0.27;
f32 STARTED = 0;

Logic::LogicID id;

void play_cat_sound() {
    u64 assets[] = {ASSET_VOICE2_1, ASSET_VOICE2_2, ASSET_VOICE2_3,
                    ASSET_VOICE2_4};
    u32 sound = random_int() % 4;
    Mixer::play_sound(0, assets[sound]);
}

void play_their_sound() {
    u64 assets[] = {ASSET_VOICE1_1, ASSET_VOICE1_2, ASSET_VOICE1_3,
                    ASSET_VOICE1_4};
    u32 sound = random_int() % 4;
    Mixer::play_sound(0, assets[sound]);
}

void update_cat_phrase(f32 now, f32 started, const char *phrase) {
    u32 charAmount = strlen(phrase);
    u32 charsWritten = strlen(cat_buffer);
    if ((now - started - (charsWritten * DELAY1)) >= DELAY1 &&
        charsWritten < charAmount) {
        cat_buffer[charsWritten] = phrase[charsWritten];
        play_cat_sound();
    }
}

void update_their_phrase(f32 now, f32 started, const char *phrase) {
    u32 charAmount = strlen(phrase);
    u32 charsWritten = strlen(their_buffer);
    if ((now - started -
         (charsWritten * DELAY2 + strlen(cat_buffer) * DELAY1)) >= DELAY2 &&
        charsWritten < charAmount) {
        their_buffer[charsWritten] = phrase[charsWritten];
        play_their_sound();
    }
}

const f32 CUTSCENE_DURATION = 10;

void enter(u32 state) {
    current_exit();
    Logic::update_callback(update_id, update, 0.0, Logic::FOREVER);
    Logic::update_callback(draw_id, draw, 0.0, Logic::FOREVER);
    current_exit = leave;

    auto exit_func = [state]() {
        LOG("I'm here");
        switch (state) {
            case 0:
                Phase1::enter();
                break;
            case 1:
                Phase2::enter();
                break;
            case 2:
                // Boss::enter();
                break;
            default:
                UNREACHABLE;
        };
    };
    id = Logic::add_callback(Logic::POST_DRAW, exit_func, Logic::now() + CUTSCENE_DURATION);

    STARTED = Logic::now();
    for (u32 i = 0; i < CAT_BUFFET_LEN; i++) cat_buffer[i] = 0;
    for (u32 i = 0; i < THEIR_BUFFER_LEN; i++) their_buffer[i] = 0;
    ASSERT(state < CAT_BUFFET_LEN, "faulty intro state");
    cat_phrase = cat_phrases[state];
}

void update(f32 delta, f32 now) {
    update_cat_phrase(now, STARTED, cat_phrase);
    if (strlen(cat_phrase) == strlen(cat_buffer)) {
        update_their_phrase(now, STARTED, their_phrase);
    }
}

void draw() {
    f32 aspect_ratio = Renderer::get_window_aspect_ratio();
    Renderer::draw_text(cat_buffer, -0.8, 0.8 * aspect_ratio, 1.0,
                        ASSET_MONACO_FONT, 0, V4(0.41, 0.63, 1, 1));
    Vec2 text_offset =
        Renderer::messure_text(their_phrase, 1.0, ASSET_MONACO_FONT);
    Renderer::draw_text(their_buffer, 0.8 - text_offset.x,
                        -0.8 * aspect_ratio + text_offset.y, 1.0,
                        ASSET_MONACO_FONT, 0);
}

void leave() {
    Logic::remove_callback(id);
}
};  // namespace Cutscene

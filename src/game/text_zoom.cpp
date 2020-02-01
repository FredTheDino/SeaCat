
struct TextCompliment: public Logic::Entity {
    // Max lifetime in seconds
    const f32 MAX_LIFE = 1.0f;
    const f32 ZOOM_SCALE = 10.0f;

    const char *text;
    f32 life;
    Vec2 position;

    void update(f32 delta) {
        life += delta;
    }

    void draw() {
        // TODO(ed): What color here?
        f32 alpha = CLAMP(0, 1.0, (MAX_LIFE - life) / MAX_LIFE);
        f32 scale = life * life * life * ZOOM_SCALE;
        Renderer::draw_text(text, position.x, position.y + 0.5 * life * life * life,
                            scale, ASSET_MONACO_FONT, -0.5,
                            V4(0.7, 0.3, 1.0, alpha));
    }
};

void remove_text() {
    auto remover = []() {

    };
}

void pick_up_compliment(const char *text, Vec2 position) {
    TextCompliment compliment = {};
    compliment.text = text;
    compliment.position = position;
    Logic::add_entity(compliment);
}


// Tell the engine that this is loaded
#define FOG_GAME

#include <vector>

namespace Game {

void entity_registration() {}

void setup() {
    Renderer::turn_on_camera(0);
}

// Main logic
void update(f32 delta) {}

// Main draw
void draw() {
    const char *some_string = "SeaCow";
    Renderer::draw_text(some_string, 0, 0, 1.0, ASSET_MONACO_FONT, -0.5);
}

}  // namespace Game

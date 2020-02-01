namespace Cutscene {

void enter(u32 state);
void update(f32 now, f32 delta);
void draw();
void leave();
void update_cat_phrase(f32 now, f32 started, char *phrase);
void update_their_phrase(f32 now, f32 started, char *phrase);

};

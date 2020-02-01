enum class Sprites {
    SHIP,
    COG,
    FLOOF,
    SPIKE,
    GLOOP,
    GLOOP_PEW_1,
    GLOOP_PEW_2,
    CAT,
    TAIL,

    BACKGROUND,
    LAZER_BEAM,
    LAZER_BASE,
    SHIP_LAZER,

    BOSS,
    BOSS_PEW_1,
    BOSS_PEW_2,
    BOSS_LAZER,

    NUM_SPRITES,
};

struct SubSprite {
    AssetID sheet;
    Vec2 min, max;
};

#define SPRITE(n) ((u32) Sprites::n)
SubSprite _sprite_states[] = {
    [SPRITE(SHIP)] = {ASSET_SS1, V2(40, 21), V2(428, 485)},
    [SPRITE(COG)] = {ASSET_SS2, V2(263, 268), V2(248, 239)},
    [SPRITE(FLOOF)] = {ASSET_SS2, V2(255, 8), V2(250, 259)},
    [SPRITE(SPIKE)] = {ASSET_SS3, V2(280, 280), V2(230, 230)},
    [SPRITE(GLOOP)] = {ASSET_SS2, V2(6, 11), V2(245, 230)},
    [SPRITE(GLOOP_PEW_1)] = {ASSET_SS2, V2(14, 245), V2(55, 55)},
    [SPRITE(GLOOP_PEW_2)] = {ASSET_SS2, V2(78, 245), V2(80, 84)},
    [SPRITE(CAT)] = {ASSET_SS3, V2(18, 12), V2(271, 293)},
    [SPRITE(TAIL)] = {ASSET_SS3, V2(387, 12), V2(108, 193)},
    [SPRITE(BACKGROUND)] = {ASSET_SS4, V2(0, 0), V2(510, 361)},
    [SPRITE(LAZER_BEAM)] = {ASSET_SS5, V2(9, 11), V2(52, 139)},
    [SPRITE(LAZER_BASE)] = {ASSET_SS8, V2(108, 111), V2(300, 300)},
    [SPRITE(SHIP_LAZER)] = {ASSET_SS5, V2(179, 2), V2(327, 225)},
    [SPRITE(BOSS)] = {ASSET_SS6, V2(0, 0), V2(512, 512)},
    [SPRITE(BOSS_PEW_1)] = {ASSET_SS7, V2(110, 6), V2(90, 62)},
    [SPRITE(BOSS_PEW_2)] = {ASSET_SS7, V2(13, 12), V2(82, 53)},
    [SPRITE(BOSS_LAZER)] = {ASSET_SS7, V2(10, 329), V2(496, 174)},
};

void draw_sprite(u32 layer, Vec2 pos, f32 dim, f32 rotation, Sprites sprite) {
    SubSprite sub_sprite = _sprite_states[(u32) sprite];
    Vec2 size = sub_sprite.max / 512.0;
    size.y *= -1;
    Renderer::push_sprite(layer, pos, size * dim, rotation, sub_sprite.sheet,
                          sub_sprite.min, sub_sprite.max);
}

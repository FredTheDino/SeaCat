Vec2 square_shape_points[] = {V2(0, 0), V2(1, 0), V2(1, 1), V2(0, 1)};
Physics::ShapeID square_shape;

Vec2 target = V2(0, 0);

enum EntityType { AGGRO = 0, FLOOF = 1, GLOOP = 2, COG = 3, BOSS = 4 };

struct GameEntity : public Logic::Entity {
    Vec2 position;
    float hp;
    float time;
    Physics::Body body;

    virtual bool is_dead() { return hp <= 0; };
};

struct AggroEnemy : public GameEntity {
    void update(float delta) override;

    void draw() override;

    bool charging = false;
    float charge_time = 1.5;
    float idle_time = 16;
    float time_offset = 2 * PI * random_real();
    float circling_radius = 0.75;
    float speed = 0.4;
    Vec2& circling_center = target;
    REGISTER_FIELDS(AGGRO_ENEMY, AggroEnemy, circling_radius, speed, speed);
};

void aggro_enemy_init(AggroEnemy& aggro_enemy, Vec2 position);

struct FloofEnemy : public GameEntity {
    void update(float delta) override;

    void draw() override;

    float speed = 0.1;
    REGISTER_FIELDS(FLOOF_ENEMY, FloofEnemy, speed);
};

void floof_enemy_init(FloofEnemy& floof_enemy, Vec2 position);

struct GloopEnemy : public GameEntity {
    void update(float delta) override;
    void draw() override;

    float speed = 0.05;
    Vec2& player_pos = target;
    REGISTER_FIELDS(GLOOP_ENEMY, GloopEnemy, speed);
};

void gloop_enemy_init(GloopEnemy& gloop_enemy, Vec2 position);

struct GloopBullet : public GameEntity {
    void update(float delta) override;

    void draw() override;

    bool is_dead() override;

    float speed = 0.25;
    float rotation = 0;
    Sprites sprite =
        (random_bit() ? Sprites::GLOOP_PEW_1 : Sprites::GLOOP_PEW_2);
    REGISTER_FIELDS(GLOOP_BULLET, GloopBullet, speed);
};

void gloop_bullet_init(GloopBullet& gloop_bullet, GloopEnemy& shooter);

struct Cog : public GameEntity {
    Vec2 center_position;
    Vec2 velocity = V2(0.15, 0);
    f32 size = 0.15;
    f32 rot_speed = 0.2;
    f32 rot_amp = 0.2;

    void update(f32 delta) override;

    void draw() override;
};

void cog_init(Cog& cog, Vec2 position);

struct Spawner {
    void update(float delta);

    void set_phase(int phase);

    void set_paused(bool paused);

    void spawn_aggro();

    void spawn_floof_phase1();

    void spawn_floof();

    void spawn_gloop();

    void spawn_cog();

    std::vector<Logic::EntityID> entities;

   private:
    int phase = 0;
    bool paused = false;
    float time = 0;
    float last_spawn[EntityType::BOSS];
};

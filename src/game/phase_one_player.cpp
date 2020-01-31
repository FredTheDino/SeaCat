//Here is where the phase one hero is Implemented
static float TOP_SPEED = 2.0;
static float TOP_ACCELERATION = 0.5;

#include <vector>

class phase_one_player{
public:
    phase_one_player();
    void init_player();
    void draw();
    void move();
    void accelerate (Vec2 new_vector);

    Physics::Body *get_body();
    
private:
    
    bool space_movement = true;
    Vec2 ammount_of_space_mov = (1.1, 1.1);
    Physics::Body *player_body;

// Vec2i current_pos = (0,0);
// Vec2 acceleration_target = (0,0);
// Vec2 current_speed = (0,0);    
};

phase_one_player(){
    Vec2 points = { V2(0, 0), V2(1, 0), V2(1, 1), V2(1, 0) };
    ShapeID my_shape = add_shape(LEN(points), points);
    *player_body = *create_body(my_shape, 0b110); 
}

void init_player(){
}

//Draw the Hero
void draw(){
    Renderer::push_rectangle(V2(2,2), V2(2,2), V4(1,0,0,1));
}

void accelerate(Vec2 *new_acc){
    Vec2 accc_temp += player_body->acceleration + new_acc;
    
    acc_temp.x = CLAMP(0, TOP_ACCELERATION, accc_temp.x);
    acc_temp.y = CLAMP(0, TOP_ACCELERATION, accc_temp.y);

    player_body->acceleration = accc_temp;
}

Physics::Body *get_body(){
    return player_body;
}


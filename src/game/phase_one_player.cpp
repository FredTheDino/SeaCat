//Here is where the phase one hero is Implemented
f32 TOP_SPEED = 1.0;
f32 TOP_ACCELERATION = 0.5;
f32 ACCELERATION_STEPS = 0.4;


#include <vector>

class phase_one_player {
public:

    phase_one_player();
    void update(f32 delta);
    Physics::Body *get_body();
    
private:

    void jerk (Vec2 *new_vector);
    void draw();
    void move();
    void accelerate();

    f32 cos_acc_mod(Vec2 *currSpeed);
    
    bool space_movement = true;
    Physics::Body *player_body;
    

// Vec2i current_pos = (0,0);
// Vec2 acceleration_target = (0,0);
// Vec2 current_speed = (0,0);    
};


//Constuct.
phase_one_player::phase_one_player(){
    Vec2 points[] = { V2(0, 0), V2(1, 0), V2(1, 1), V2(1, 0) };
    Physics::ShapeID my_shape = Physics::add_shape(LEN(points), points);
    *player_body = Physics::create_body(my_shape, 0b110); 

}

//Update
void phase_one_player::update(f32 delta){

    static bool show_control_controls = true;
    if (Util::begin_tweak_section("phase1_controls", &show_control_controls)) {
        Util::tweak("TOP_SPEED", &TOP_SPEED);
        Util::tweak("TOP_ACCELERATION", &TOP_ACCELERATION);
	Util::tweak("ACCELERATION_STEPS", &ACCELERATION_STEPS);
    }
    Util::end_tweak_section(&show_control_controls);

    using namespace Input;
    Vec2 vel_target = V2(0, 0);
    if (down(Name::UP))
        vel_target.y += ACCELERATION_STEPS;
    if (down(Name::DOWN))
        vel_target.y -= ACCELERATION_STEPS;
    if (down(Name::LEFT))
        vel_target.x -= ACCELERATION_STEPS;
    if (down(Name::RIGHT))
        vel_target.x += ACCELERATION_STEPS;
    
    jerk(&vel_target);

}

//Getters/Setters
Physics::Body* phase_one_player::get_body(){
    return player_body;
}


//Increase the current acceleration
void phase_one_player::jerk(Vec2 *new_acc){
    Vec2 acc_temp = player_body->acceleration + *new_acc;
    
    acc_temp.x = CLAMP(0, TOP_ACCELERATION, acc_temp.x);
    acc_temp.y = CLAMP(0, TOP_ACCELERATION, acc_temp.y);

    player_body->acceleration = acc_temp;
}

//Draw the Hero
void phase_one_player::draw(){
    Renderer::push_rectangle(0,player_body->position, V2(2,2), V4(1,0,0,1));
}

//Calc and Change the current speed by the acc and cos(max_speed - curr_speed)
void phase_one_player::accelerate(){    

}

//Move by the curr speed
void phase_one_player::move(){
    
}

//Calc the cos(of max speed - current speed)
f32 phase_one_player::cos_acc_mod(Vec2 *currSpeed){
    return 0.0;
}




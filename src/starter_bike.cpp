#include "starter_bike.h"
#include <godot_cpp/core/class_db.hpp>
#include <iostream>


using namespace godot;

void StarterBike::_bind_methods() {
}

StarterBike::StarterBike(){
    //set_process_input(true);
}

StarterBike::~StarterBike(){

}

void StarterBike::_physics_process(double delta){
    Input* input = Input::get_singleton();
    double cur_acceleration = 0.0;
    Vector3 velocity = get_velocity();
    speed = Math::sqrt( velocity.x*velocity.x + velocity.z*velocity.z);

    // House keeping
    if (input->is_action_just_pressed("DebugToggle")){
        debug_toggle = !(debug_toggle);
        // $HUD.debugMode(debug_tog)
    }
    if (input->is_action_just_pressed("ShiftUp")){
        gear = std::min(gear + 1, 2);
        MAX_SPEED = GEARS[gear].second;
        effort_calculations();
    }
    if (input->is_action_just_pressed("ShiftDown")){
        gear = std::max(gear - 1, 0);
        MAX_SPEED = GEARS[gear].second;
        effort_calculations();
    }
    // Handle jump.
    if (input->is_action_just_pressed("Jump") && is_on_floor()){
        velocity.y = JUMP_VELOCITY;
        // set_velocity(velocity); unneeded
    }

    double direction = 0;
    double z_direction = 0;
    if (input->is_action_pressed("TurnLeft")){
        front_angle += STEERING_INTENSITY;
        //direction = 1;
    }
    if (input->is_action_pressed("TurnRight")){
        front_angle -= STEERING_INTENSITY;
        //direction = 1;
    }
    front_angle = std::max(-1.0 * MAX_STEER, std::min(MAX_STEER, front_angle));

    if (input->is_action_pressed("PitchUp")){
        //print(get_floor_angle())
        z_direction = 1;
    }
    if (input->is_action_pressed("PitchDown")){
        z_direction = -1;
    }
    
    if (z_direction == 0 && is_on_floor()){
        z_direction = -0.6;
    }

    //#rotation += Vector3(0, angular_speed * direction * delta, 0)
	//#var angular_change = angular_speed * direction * delta
	double angular_change = front_angle * delta;
	//#front_angle += angular_change
	front_angle_glob = angular_change * 0.1;
	rotate_y(angular_change);
	//# cancel mass rotation on Front wheel
	//print(angular_change)
	//print(front_angle_glob)
	//#$FrontWheel.rotate_y(-1*angular_change)

    // #$FrontWheel.rotate_y(angular_speed * direction * delta * 2)

    if (z_direction != 0){
        rotate_object_local(Vector3(-1,0,0), angular_speed * z_direction * delta);
    }

    Vector3 rotation = get_rotation();
    double max_pitch_radians = MAX_PITCH * M_PI / 180.0;
    if (rotation.x > -(get_floor_angle()) && is_on_floor()){
        rotation.x = -(get_floor_angle());
    }
    else if (rotation.x < max_pitch_radians && is_on_floor()){
        rotation.x = max_pitch_radians;
    }

    if (rotation.z != 0){
        rotation.z = 0;
    }
    set_rotation(rotation);

    // rotate_x(angular_speed * z_direction * delta)

    if (input->is_action_pressed("Pedal")) {
        //std::cout << "GREP" << speed << "\n";
        double pedal_acceleration = FRICTION + BASE_ACCELERATION  * effort_factor * get_acceleration_penalty(GEARS[gear].first, gear_penalty);
        if (pedal_acceleration <= FRICTION){
            // print("Pedal accel {} is less than friction {}".format([pedal_acceleration, FRICTION], "{}"))
        }
        cur_acceleration += pedal_acceleration;
    }
    if (input->is_action_pressed("Brake")) {
        cur_acceleration -= BASE_BRAKE;
    }
    if (input->is_action_just_released("Pedal")){
        acceleration = 0;
    }
    if (input->is_action_just_released("Brake")){
        acceleration = 0;
        front_angle = 0;
    }

    // effort check
    if (input->is_action_just_pressed("IncreaseEffort")){
        if (effort < MAX_EFFORT){
            effort += EFFORT_INTERVAL;
            effort_calculations();
        }
    }
    if (input->is_action_just_pressed("DecreaseEffort")){
        if (effort > 0){
            effort -= EFFORT_INTERVAL;
            effort_calculations();
        }
    }

    // was if speed > 0, can actually still just check if veloctiy is not equal to (0,0,0) ?
    // TODO Check this calculation in comparison to speed, it might make sense logically IDK
	if (velocity != Vector3(0,0,0)){
		cur_acceleration -= FRICTION;
    }
	
	acceleration_calcs(cur_acceleration);
    //std::cout << "GREPP" << speed << " " << acceleration << "\n";

	/*# https://physics.stackexchange.com/questions/182689/what-causes-a-cars-velocity-to-follow-the-front-wheels-direction
	
	var dx = speed - rear_speed
	var dxf = dx * cos(front_angle)
	var dyf = dx * sin(front_angle)*/

    // assuming cur_acceleration in the base case is going in the Z direction?
    // var trueRotation = ($FrontWheel.global_rotation.y + global_rotation.y) * 0.5
	// trueRotation = global_rotation.y + front_angle_glob
	// trueRotation = rotation.y + front_angle
	// print("main: {} front: {} frame: {}".format([trueRotation,$FrontWheel.global_rotation.y,global_rotation.y],"{}"))
	// velocity = Vector3(0,velocity.y,speed).rotated(Vector3.UP, trueRotation)
    // std::cout << "GREP" << speed << "\n";
    velocity = Vector3(0,velocity.y,speed).rotated(Vector3(0,1,0), rotation.y);

    //$FrontWheel.global_rotation.y = global_rotation.y
	//$FrontWheel.global_rotation.y += front_angle
	
	if (prev_rot != rotation.y){
		prev_rot = rotation.y;
		//print("GREP {}".format([$FrontWheel.rotation.y], "{}"))
    }
	//#speed = sqrt( velocity.x*velocity.x + velocity.z*velocity.z )
	
	//# gravity
	if (!is_on_floor()){
		//print("HELLO")
		velocity += get_gravity() * delta;
    }
    // last velocity modification, so set velocity
    set_velocity(velocity);
	
    // var rot = rotation_degrees
    move_and_slide();
    update_energy(input);


    // $FrontWheel.rotate_object_local(Vector3(1,0,0), speed * delta)
	// $RearWheel.rotate_object_local(Vector3(1,0,0), speed * delta)
	// $HUD.updateVelocity(speed)
	// $HUD.updateAcceleration(acceleration)
	// $HUD.updateSpeedCap(SPEED_CAP, is_cruising)
	// $HUD.updateEffort(effort)
	// $HUD.updateGear(gear + 1)
	// $HUD.updateNRG(energy)
	// $HUD.updateRotation(rotation_degrees.x, rotation_degrees.y, rotation_degrees.z)
	// $HUD.updateRotation($FrontWheel.rotation_degrees.x, $FrontWheel.rotation_degrees.y, $FrontWheel.rotation_degrees.z)
	// $HUD.updateFloorAngle(get_floor_angle())
	// #current_pos.emit(position.x, position.y, position.z, rotation.y)
}

void StarterBike::effort_calculations(){
    effort_factor = effort/MAX_EFFORT;
    prev_speed_cap = speed_cap;
    speed_cap = MAX_SPEED * effort_factor;
    // print(prev_speed_cap," ",speed_cap)
    need_flip = true;
}

// changed min_speed pass by value because I don't want to declare another variable.
double StarterBike::get_acceleration_penalty(double min_speed, std::pair<double,double>& bounds){
    if (speed >= min_speed) {
        return 1.0;
    }
    double ratio = ( min_speed - speed ) / min_speed;
    ratio = speed/min_speed; // ??? This is why I moved this to git to figure out why I do these things
    // 960 - 959
    double p = bounds.first - bounds.second;
    return (p * ratio) + bounds.first;
}

void StarterBike::update_energy(Input* input){
    if (acceleration < 0.0) {
        return;
    }
    else if (input->is_action_pressed("Pedal")){
        energy += (BASE_DEPLETION * effort_factor * std::max(acceleration, 0.0)) 
            + (BASE_DEPLETION * effort_factor * speed / MAX_SPEED);
    }
}

void StarterBike::acceleration_calcs(double& alpha){
    // base case we are adding the acceleration to the speed.
    acceleration = alpha;
    speed += acceleration;

    //std::cout << "GREP" << speed << " " << acceleration << " speed_cap " << speed_cap << " need_flip " << need_flip << "\n";
    if (speed >= speed_cap && !(need_flip)){ // had check of prev_speed_Cap
        speed = speed_cap;
        acceleration = 0;
    }
    else if (speed < speed_cap && need_flip){
        need_flip = false;
        prev_speed_cap = 0; // reset
        acceleration = std::max(acceleration, 0.0);
    }
    else if (speed >= speed_cap){ // Inversely means prev_speed_cap > speed_cap
        if (acceleration > (-1 * BASE_BRAKE)){
            speed -= acceleration; // undo previous acceleration
            acceleration -= alpha;
            acceleration = std::min(acceleration, 0.0);
            acceleration -= FRICTION;
            speed += acceleration;

        }
        if (speed <= speed_cap){
            prev_speed_cap = 0; // reset
            speed = speed_cap;
            acceleration = 0;
        }
    }

    if (speed <= 0.0){
        speed = 0;
        acceleration = 0;
    }
}
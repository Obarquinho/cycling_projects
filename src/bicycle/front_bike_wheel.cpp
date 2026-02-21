#include "front_bike_wheel.h"
#include <godot_cpp/core/class_db.hpp>
#include <iostream>

using namespace godot;

void FrontBikeWheel::_bind_methods() {
    // do things
}

FrontBikeWheel::FrontBikeWheel(){}

FrontBikeWheel::~FrontBikeWheel(){}

void FrontBikeWheel::updateSteerAngleConstant(double& dir){
    steer_angle = dir * MAX_STEERING_ANGLE;
}

void FrontBikeWheel::rotate_wheel(double& speed, double& delta){
    //this->transform.basis = this->basis; 
    
    //set_rotation(Vector3(rotation.x,rotation.y,0)); 
    // rotate_object_local(Vector3(0,1,0), steer_angle);
    // Vector3 globRot = global_rotation();
    
    // //rotate_x(steer_angle);
    // orthonormalize();
    BikeWheel::rotate_wheel(speed, delta);

    //Vector3 rotation = get_rotation();


    
    
}

void FrontBikeWheel::_physics_process(double delta){
    BikeWheel::_physics_process(delta);
}
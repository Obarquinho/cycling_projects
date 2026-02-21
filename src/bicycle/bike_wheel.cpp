#include "bike_wheel.h"
#include <godot_cpp/core/class_db.hpp>
#include <iostream>

using namespace godot;

void BikeWheel::_bind_methods() {
    // do things
}

BikeWheel::BikeWheel(){}

BikeWheel::~BikeWheel(){}

void BikeWheel::rotate_wheel(double& speed, double& delta){
    rotate_object_local(Vector3(1,0,0), speed * delta);
}

void BikeWheel::_physics_process(double delta){

}
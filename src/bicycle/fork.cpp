#include "fork.h"
#include <godot_cpp/core/class_db.hpp>
#include <iostream>

using namespace godot;

void Fork::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_steering_intensity", "intensity"), &Fork::set_steering_intensity);
    ClassDB::bind_method(D_METHOD("get_steering_intensity"), &Fork::get_steering_intensity);
    ClassDB::add_property("Fork", PropertyInfo(Variant::FLOAT, "Intensity"), "set_steering_intensity", "get_steering_intensity");
}

Fork::Fork(){
    m_steeringIntensity = STEERING_INTENSITY;
}

Fork::~Fork(){}

void Fork::set_steering_intensity(double intensity){
    m_steeringIntensity = intensity;
}

double Fork::get_steering_intensity(){
    return m_steeringIntensity;
}

void Fork::steer_handlebar(double& direction){
    Vector3 curRotDegrees = get_rotation_degrees();
    double steerAffect = stupidClamp(curRotDegrees.y + (m_steeringIntensity * direction),
                                  MIN_DEGREES,
                                  MAX_DEGREES);
    if (direction == 0 && steerAffect != 0){
        double steerAffectAffect = steerAffect > 0 ? m_steeringIntensity * REIHEN * -1 : m_steeringIntensity * REIHEN;
        double tempSA = steerAffect;
        steerAffect += steerAffectAffect;
        // make sure we don't approach the limit of 0
        if ((tempSA < 0 && steerAffect > 0) || (tempSA > 0 && steerAffect < 0)){
            steerAffect = 0;
        }
    }
                                  
    set_rotation_degrees(Vector3(curRotDegrees.x, steerAffect, curRotDegrees.z));

}

// make global util class or something
double Fork::stupidClamp(double value, const double& minimum, const double& maximum){
    if(value < minimum){
        value = minimum;
    } else if (value > maximum){
        value = maximum;
    }
    return value;
}

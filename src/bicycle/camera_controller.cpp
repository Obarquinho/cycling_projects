#include "camera_controller.h"
#include <godot_cpp/core/class_db.hpp>

#include <algorithm>
#include <iostream>

using namespace godot;

void CameraController::_bind_methods() {
    // do things
}

CameraController::CameraController(){}

CameraController::~CameraController(){}

void CameraController::_input(const Ref<InputEvent> &event){
    Ref<InputEventMouseMotion> mm = event;
    Input* input = Input::get_singleton();
    // UtilityFunctions::print("SEEN");
	if (mm.is_valid() && input->is_action_pressed("LMB")) {
        UtilityFunctions::print("DOING");
		rotate_camera(mm->get_screen_relative() * 0.001);
	}
}

void CameraController::_process(double delta){
    // Input* input = Input::get_singleton();
    
    // if(input == InputEvent::Mo)
}

void CameraController::rotate_camera(Vector2& move){
    rotate_y(-move.x);
    orthonormalize();
    Vector3 rotation = get_rotation();
    double possibleMovement = rotation.x + move.y;
    double newX = std::clamp(possibleMovement, CAMERA_X_ROT_MIN, CAMERA_X_ROT_MAX);
    rotation.x = newX;
    set_rotation(rotation);
}
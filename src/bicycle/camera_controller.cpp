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
        last_drag = 0;
        need_return = true;
	}
    if (input->is_action_pressed("LMB")){
        need_go_back = false;
    } else if (input->is_action_just_released("LMB")){
        last_drag = 0;
        need_go_back = true;
    }
}

void CameraController::_process(double delta){
    last_drag++;
    //Quaternion objective = get_quaternion();
    double speed = 0.05;
    // if (last_drag > WAIT_FRAMES && need_return && need_go_back){
    //     Quaternion q_cur = get_quaternion();//get_global_basis().get_rotation_quaternion();
    //     Quaternion q_desired = Quaternion(Vector3(1,0,0), 0);

        
    //     //Quaternion q_new = q_cur.slerp(q_desired, speed);
    //     Quaternion q_new = q_cur.slerp(parent_quaternion, speed);


        

    //     if (q_new.angle_to(q_desired) < 0.8){
    //         //set_global_basis(Quaternion(Vector3(1,0,0),0));
    //         UtilityFunctions::print("RETURNED");
    //         need_return = false;
    //     } else {
    //         set_quaternion(q_new);
    //     }

    // }

    
    set_global_position(parent_position);

    if(need_go_back){
        Quaternion q_c = get_quaternion();
        Quaternion q_n = q_c.slerp(parent_quaternion, speed*.1);
        //if (q_c.angle_to(q_n) >= 0.01){
            set_quaternion(q_n);
        //}
    }
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

void CameraController::update_parent_quaternion(Quaternion& quat){
    parent_quaternion = quat;
}

void CameraController::update_parent_position(Vector3& pos){
    parent_position = pos;
}
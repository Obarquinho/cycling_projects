#include "camera_controller.h"
#include <godot_cpp/core/class_db.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace godot;

void CameraController::_bind_methods() {
    ADD_SIGNAL(MethodInfo("camera_angle", PropertyInfo(Variant::FLOAT, "cam_ang")));
    ADD_SIGNAL(MethodInfo("parent_angle", PropertyInfo(Variant::FLOAT, "par_ang")));
    ADD_SIGNAL(MethodInfo("angle_limit", PropertyInfo(Variant::FLOAT, "angle_limit")));

    ClassDB::bind_method(D_METHOD("set_slow_angle", "_rads"), &CameraController::set_slow_angle);
    ClassDB::bind_method(D_METHOD("get_slow_angle"), &CameraController::get_slow_angle);
    
    ClassDB::bind_method(D_METHOD("set_fast_angle", "_rads"), &CameraController::set_fast_angle);
    ClassDB::bind_method(D_METHOD("get_fast_angle"), &CameraController::get_fast_angle);

    ClassDB::bind_method(D_METHOD("set_slerp_speed", "_spd"), &CameraController::set_slerp_speed);
    ClassDB::bind_method(D_METHOD("get_slerp_speed"), &CameraController::get_slerp_speed);


    ClassDB::add_property("CameraController", PropertyInfo(Variant::FLOAT, "Slow Angle", PROPERTY_HINT_NODE_TYPE, "Camera slow speed angle"), "set_slow_angle", "get_slow_angle");
    ClassDB::add_property("CameraController", PropertyInfo(Variant::FLOAT, "Fast Angle", PROPERTY_HINT_NODE_TYPE, "Camera fast speed angle"), "set_fast_angle", "get_fast_angle");
    ClassDB::add_property("CameraController", PropertyInfo(Variant::FLOAT, "Slerp Speed", PROPERTY_HINT_NODE_TYPE, "Camera slow speed angle"), "set_slerp_speed", "get_slerp_speed");
}

void CameraController::set_slow_angle(double _rads){
    slow_angle = _rads;
}

double CameraController::get_slow_angle(){
    return slow_angle;
}

void CameraController::set_fast_angle(double _rads){
    fast_angle = _rads;
}

double CameraController::get_fast_angle(){
    return fast_angle;
}

void CameraController::set_slerp_speed(double _spd){
    slerp_speed=_spd;
}

double CameraController::get_slerp_speed(){
    return slerp_speed;
}

CameraController::CameraController(){}

CameraController::~CameraController(){}

void CameraController::_input(const Ref<InputEvent> &event){
    Ref<InputEventMouseMotion> mm = event;
    Input* input = Input::get_singleton();
    bool l_held_l = false;
    bool l_held_r = false;
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

    if (input->is_action_pressed("TurnLeft")){
        held_left = (prev_lr_held == signals::hold_left || prev_lr_held == signals::reset);
        if(held_left) prev_lr_held = signals::hold_left;
        l_held_l = true;
        if(angle_signal != signals::lock_left && angle_signal != signals::hold_reset){
            angle_signal =  signals::reset;
        }
    }
    else
    {
        held_left = false;
        l_held_l = false;
    }

    if(angle_signal == signals::lock_left && input->is_action_just_released("TurnLeft")){
        angle_signal = signals::lock_mouse;
    }
    
    if (input->is_action_pressed("TurnRight")){
        UtilityFunctions::print("RIGHTPRES");
        held_right = (prev_lr_held == signals::hold_right || prev_lr_held == signals::reset);
        if(held_right) prev_lr_held = signals::hold_right;
        l_held_r = true;
        if(angle_signal != signals::lock_right && angle_signal != signals::hold_reset){
            angle_signal =  signals::reset;
        }
    }
    else
    {
        held_right = false;
        l_held_r = false;
    }

    if(angle_signal == signals::lock_right && input->is_action_just_released("TurnRight")){
        angle_signal = signals::lock_mouse;
    }

    if((!l_held_l && prev_lr_held == signals::hold_left) || (!l_held_r && prev_lr_held == signals::hold_right)){
        prev_lr_held = signals::reset;
    }

    // Keep at bottom until figure out better way to use this enum.
    if(!need_go_back){
        angle_signal = signals::lock_mouse;
    }
}

void CameraController::_process(double delta){
    // do not overflow this
    if(last_drag <= WAIT_FRAMES){
        last_drag++;
    }

    Input* input = Input::get_singleton();
    bool heldLeft = false;
    bool heldRight = false;
    if (input->is_action_pressed("TurnLeft") && input->is_action_pressed("TurnRight")) {
        heldLeft =false;
        heldRight = false;
    }
    else if (input->is_action_pressed("TurnLeft")){
        heldLeft = true;
    }
    else if (input->is_action_pressed("TurnRight")) {
        heldRight = true;
    }
    //Quaternion objective = get_quaternion();
    // was .025
    double speed = slerp_speed;
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

    /*
    
    IDEA:
    Speed-based angle limit. The faster you are going the smaller the angle you can have between the camera and parent.
    If you are at an angle greater than the limit, then we will slerp until we reach that angle
    For slerp, at end of each process, we cache the rotation between the camera and parent, so that on next process call, we can
    rotate our camera's quaternion back to that rotation, and then apply the slerp. Looks less jittery
    
    */

    
    set_global_position(parent_position);

    bool signal_locked = angle_signal == signals::lock_left || angle_signal == signals::lock_right;
    print_signal(angle_signal);

    double angle_limit = get_angle_limit(parent_speed);

    Quaternion q_d = get_quaternion();
    emit_signal("camera_angle", q_d.angle_to(Quaternion(0,1,0,0)));
    emit_signal("parent_angle", parent_quaternion.angle_to(Quaternion(0,1,0,0)) - q_d.angle_to(Quaternion(0,1,0,0)));

    emit_signal("camera_angle", q_d.get_euler().y);
    emit_signal("parent_angle", parent_quaternion.get_euler().y/* - q_d.get_euler().y*/);
    emit_signal("angle_limit", angle_limit);

    double controller_angle = q_d.get_euler().y;
    double parent_angle = parent_quaternion.get_euler().y;

    if(need_go_back && last_drag >= WAIT_FRAMES && !signal_locked){
        //Quaternion q_c = slerpRotation == Quaternion(0,0,0,1) ? get_quaternion() : slerpRotation * parent_quaternion;
        if(slerpRotation == Quaternion(0,0,0,1)) UtilityFunctions::print("empty quat");
        Quaternion q_c = get_quaternion();

        /*UtilityFunctions::print("---");
        UtilityFunctions::print(q_c.angle_to(parent_quaternion));
        UtilityFunctions::print(parent_quaternion.angle_to(q_c));
        UtilityFunctions::print(q_c.angle_to(Quaternion(0,1,0,0)), " ", q_c);
        UtilityFunctions::print(parent_quaternion.angle_to(Quaternion(0,1,0,0)), " ", parent_quaternion);*/


        
        //UtilityFunctions::print(angle_limit, q_c.angle_to(parent_quaternion), q_c.get_angle(), parent_quaternion.get_angle(), q_c.dot(parent_quaternion));
        //UtilityFunctions::print(std::format("q_c angle to parent {}, cam quat angle {}. parnet quat angle {}", q_c.angle_to(parent_quaternion), q_c.get_angle(), parent_quaternion.get_angle()));

        

        // was 1.2 not angle_limit
        if (angle_signal == signals::reset && q_c.angle_to(parent_quaternion) > angle_limit){
            goldDiff = q_c * parent_quaternion.inverse();

            int mult = 1;

            double l_cam_angle = q_c.get_euler().y;
            double l_par_angle = parent_quaternion.get_euler().y;
            double diff = l_cam_angle - l_par_angle;
            double crossDist = 0;
            if(diff > M_PI)
            { 
                diff -= M_PI;
                crossDist = M_PI - l_cam_angle + M_PI + l_par_angle;
            }
            if(diff < -M_PI) diff += M_PI;

            bool secondPart = crossDist != 0 ? diff < crossDist : true;
            bool l_to_right = diff > 0 && secondPart;

            UtilityFunctions::print("PARENT: ", l_par_angle, " CAM: ", l_cam_angle);
            UtilityFunctions::print("DIFF: ", diff, " L_TO_RIGHT ", l_to_right, " HELD_RIGHT ", heldRight, " HELD LEFT ", heldLeft);
            print_signal(prev_lr_held);

            
            
            if((/*held_left ||*/ heldLeft) && !l_to_right){
                mult = -1;
                angle_signal = signals::lock_left;
            }
            else if((/*held_right ||*/ heldRight) && l_to_right) /*if(held_right)*/{
                angle_signal = signals::lock_right;
            }
            goldDiff = Quaternion(Vector3(0,1,0), angle_limit * mult); // seeing if this would work?
            Quaternion q_b = parent_quaternion * goldDiff;
            set_quaternion(q_b);
            need_stutter = false;
        }

        //if(held_left || held_right) speed = 0.020;
        Quaternion q_target = parent_quaternion;
        if (angle_signal == signals::hold_reset){
            // If i'm going narrower, I want to slerp to parent
            // If i'm going wider, I want to slerp to angle limit (it is outer.)
            if (q_c.angle_to(parent_quaternion) <= angle_limit) { // hold_reset has fulfilled its purpose of going from big angle limit to small limit
                angle_signal = signals::reset;
            }
            else {
                q_c = slerpRotation * parent_quaternion;
            }

            if(prev_angle < angle_limit){
                int mult = 1;
                if(/*held_left*/heldLeft){
                //if(held_left){
                    mult = -1;
                }
                q_target = parent_quaternion * Quaternion(Vector3(0,1,0), mult * angle_limit);
            }
            
        }
        Quaternion q_n = q_c.slerp(q_target, speed);
        //if (q_c.angle_to(q_n) >= 0.01){
        slerpRotation = q_n * parent_quaternion.inverse();
        if(/*(!held_right && !held_left)*/(!heldLeft && !heldRight) || angle_signal == signals::hold_reset/* && prev_angle > angle_limit)*/){ // second clause on and was added later
            //angle_signal = angle_signal == signals::hold_reset ? signals::reset : angle_signal;
            set_quaternion(q_n);
        }
        //}
    }
    else if (signal_locked){
        //Quaternion q_n = parent_quaternion + goldDiff;
        Quaternion q_c = get_quaternion();
        double cur_angle = q_c.angle_to(parent_quaternion);
        // if(cur_angle < angle_limit && angle_limit != prev_angle) // prev angle check is not robust enough because will 
        // {
        //     UtilityFunctions::print(cur_angle, " lt ", angle_limit);
        //     angle_signal = signals::reset;
        // }
        //else if(angle_limit < prev_angle)
        if (angle_limit != prev_angle)
        {
            UtilityFunctions::print("---");
            angle_signal = signals::hold_reset; //was reset
            Quaternion q_n = goldDiff * parent_quaternion;

            int mult = 1;
            if(heldLeft /*held_left*/){
            //if(held_left){
                mult = -1;
            }
            Quaternion q_target = parent_quaternion * Quaternion(Vector3(0,1,0), mult * angle_limit);
            //q_n = q_target; //idk if set to actual limit or to lerp to it.
            q_n = q_n.slerp(q_target, speed);

            slerpRotation = q_n * parent_quaternion.inverse();

            set_quaternion(q_n);
        }
        else
        {
            UtilityFunctions::print("+++", "\n", cur_angle, " gt " , angle_limit);
            Quaternion q_rotate = Quaternion(Vector3(0,1,0), angle_limit); // set to limit (positive is to the right)
            
            Quaternion q_n = goldDiff * parent_quaternion;
            if( goldDiff.angle_to(parent_quaternion) > angle_limit && prev_angle != angle_limit) { //only wwould work for turning right.
                UtilityFunctions::print("SETTTING ++++++");
                //q_n = parent_quaternion * q_rotate;
            }
            set_quaternion(q_n);
        }
    }

    q_d = Quaternion(Vector3(0,1,0), -1);
    //set_quaternion(parent_quaternion * q_d);
    //set_quaternion(q_d * parent_quaternion);
    prev_angle = angle_limit;
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

void CameraController::update_parent_speed(double spd){
    parent_speed = spd;
}

double CameraController::get_angle_limit(double spd){
    double ratio = (fast_speed-spd)/(fast_speed-slow_speed);
    double p = slow_angle - fast_angle;
    return (p*ratio) + fast_angle;
}

void CameraController::print_signal(signals s){
    if (signal_map.find(s) != signal_map.end()){ // idk why this project is  not using >=cpp20
        UtilityFunctions::print(signal_map[s].c_str());
    }
    else {
        UtilityFunctions::print("UNRECOGNIZED SIGNAL");
    }
}
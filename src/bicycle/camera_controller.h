#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <unordered_map>

namespace godot {

    

class CameraController : public Node3D {
    GDCLASS(CameraController, Node3D)

    protected:
        enum class signals{
            reset,
            hold_left,
            hold_right,
            lock_left,
            lock_right,
            lock_mouse,
            hold_reset
        };

        std::unordered_map<signals, std::string> signal_map = {
            { signals::reset, "reset" },
            { signals::hold_left, "hold_left" },
            { signals::hold_right, "hold_right" },
            { signals::lock_left, "lock_left" },
            { signals::lock_right, "lock_right" },
            { signals::lock_mouse, "lock_mouse" },
            { signals::hold_reset, "hold_reset" }
        };

        static void _bind_methods();

        const double CAMERA_X_ROT_MIN = Math::deg_to_rad(-89.9);
        const double CAMERA_X_ROT_MAX = Math::deg_to_rad(70.0);
        const double WAIT_FRAMES = 120;

        double last_drag = 0;
        double need_return = false;
        Quaternion parent_quaternion;
        Vector3 parent_position;
        double parent_speed;
        Quaternion goldDiff;
        bool need_go_back =true;
        bool need_stutter = true;
        bool held_left = false;
        bool held_right = false;
        signals angle_signal = signals::reset;
        signals prev_lr_held = signals::reset;
        double prev_angle = 0;

        double slow_angle = 1.2;
        double fast_angle = 0.5;
        double slow_speed = 0;
        double fast_speed = 12; // Needs to be max speed!

        double slerp_speed = 0.5; // was 0.005

        Quaternion slerpRotation = Quaternion(0,0,0,1);
        

    public:
        CameraController();
        ~CameraController();

        virtual void _process(double delta) override;

        virtual void _input(const Ref<InputEvent> &event) override;

        void rotate_camera(Vector2& move);

        void update_parent_quaternion(Quaternion& quat);

        void update_parent_position(Vector3& pos);

        void update_parent_speed(double spd);

        void CameraController::set_slow_angle(double _rads);

        double CameraController::get_slow_angle();

        void CameraController::set_fast_angle(double _rads);
        
        double CameraController::get_fast_angle();

        void CameraController::set_slerp_speed(double _spd);

        double CameraController::get_slerp_speed();

        double CameraController::get_angle_limit(double spd);

        void print_signal(signals s);
    };

}
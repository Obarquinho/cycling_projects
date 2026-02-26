#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

namespace godot {

class CameraController : public Node3D {
    GDCLASS(CameraController, Node3D)

    protected:
        static void _bind_methods();

        const double CAMERA_X_ROT_MIN = Math::deg_to_rad(-89.9);
        const double CAMERA_X_ROT_MAX = Math::deg_to_rad(70.0);

    public:
        CameraController();
        ~CameraController();

        virtual void _process(double delta) override;

        virtual void _input(const Ref<InputEvent> &event) override;

        void rotate_camera(Vector2& move);
};

}
#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class BikeWheel : public Node3D {
    GDCLASS(BikeWheel, Node3D)

    protected:
        static void _bind_methods();

    public:
        BikeWheel();
        ~BikeWheel();

        virtual void _physics_process(double delta) override;
        virtual void rotate_wheel(double& speed, double& delta);
};

}
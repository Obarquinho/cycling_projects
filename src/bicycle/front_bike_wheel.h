#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <godot_cpp/classes/node3d.hpp>

#include "bike_wheel.h"

namespace godot {

class FrontBikeWheel : public BikeWheel {
    GDCLASS(FrontBikeWheel, BikeWheel)

    protected:
        static void _bind_methods();

        double steer_angle = 0.0;

        const double MAX_STEERING_ANGLE = 0.1;

    public:
        FrontBikeWheel();
        ~FrontBikeWheel();

        virtual void _physics_process(double delta) override;
        virtual void rotate_wheel(double& speed, double& delta) override;

        void updateSteerAngleConstant(double& dir);
};
}
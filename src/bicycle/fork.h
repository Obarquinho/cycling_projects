#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class Fork : public Node3D {
    GDCLASS(Fork, Node3D)

    protected:
        static void _bind_methods();
        const double MIN_DEGREES = -15.0; // denominates left
        const double MAX_DEGREES = 15.0; // denominates right
        const double STEERING_INTENSITY = 1;
        const double REIHEN = 4; // german for to straighten
        double m_steeringIntensity = 0.0;

        double stupidClamp(double value, const double& minimum, const double& maximum);
        

    public:
        Fork();
        ~Fork();

        void set_steering_intensity(double intensity);
        double get_steering_intensity();

        

        void steer_handlebar(double& direction);
};



}
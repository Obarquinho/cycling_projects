#pragma once

#define _USE_MATH_DEFINES

// Godot Deps
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input.hpp>

// std deps
#include <math.h>
#include <vector>

// my deps
#include "bike_wheel.h"
#include "front_bike_wheel.h"
#include "fork.h"

namespace godot {

class StarterBike : public CharacterBody3D {
    GDCLASS(StarterBike, CharacterBody3D)

// private:
protected:
    const std::vector<std::pair<int,int>> GEARS =  {{0, 5}, {3, 8}, {6, 12}};
    int gear = 0;
    const int MAX_GEAR = 2;
    std::pair<double, double> gear_penalty = {0.5, 1.0};

    double speed = 0;
    double angular_speed = M_PI;
    double acceleration = 0;
    int MAX_SPEED = GEARS[0].second; // is not const. dev note -> I set this to .first since I saw GEARS[0][1]. I thought I would have a problem with pairs member naming and I did :)
    int speed_cap = MAX_SPEED;
    int prev_speed_cap = 0;
    int target_speed = MAX_SPEED;
    const double FRICTION = 0.05;
    const double BASE_ACCELERATION = 0.3;
    const double BASE_BRAKE = 5.0;
    const double JUMP_VELOCITY = 4.5;
    const double STEERING_INTENSITY = 0.1;
    const double MAX_STEER = 1;


    //
    // Wheels
    double front_speed = 0;
    double front_angle = 0; // radians and relative to parallel axis of main body (i.e. cos is the z vector)
    double rear_speed = 0;
    double front_angle_glob = 0;

    // Effort variables
    const double MAX_EFFORT = 100.0;
    const double EFFORT_INTERVAL = 10.0;
    double effort = MAX_EFFORT;
    double effort_factor = 1.0;
    bool need_flip = false; // weird flag

    // Energy variables
    double energy = 100.0;
    const double BASE_DEPLETION = -0.005;

    // Misc
    bool debug_toggle = true;
    const double MAX_PITCH = -70.0;
    double prev_rot = 0.0; // debug, does not functionally change anything

    // pointers to chilidren
    BikeWheel* p_rearWheel = nullptr;
    FrontBikeWheel* p_frontWheel = nullptr;
    Fork* p_fork = nullptr;

protected:
    static void _bind_methods();

public:
    StarterBike();
    ~StarterBike();

    // for some reason, _physics_process takes a double, and not a float as they've defined it as _physics_process(float: delta) in
    // GDScript :) 
    virtual void _physics_process(double delta) override;

    // non GD overrides
    void effort_calculations();
    double get_acceleration_penalty(double min_speed, std::pair<double,double>& bounds);
    void update_energy(Input* input);
    void acceleration_calcs(double& alpha);

    // children
    void set_rear_wheel(BikeWheel* _rearWheel);
    BikeWheel* get_rear_wheel() const;

    void set_front_wheel(FrontBikeWheel* _frontWheel);
    FrontBikeWheel* get_front_wheel() const;

    void set_fork(Fork* _fork);
    Fork* get_fork() const;

    //virtual void input(const Ref<InputEvent> &p_event) override;
};

} // namespace godot
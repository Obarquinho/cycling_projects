#pragma once

#include <godot_cpp/classes/character_body3d.hpp>

namespace godot {

class StarterBike : public CharacterBody3D {
    GDCLASS(StarterBike, CharacterBody3D)

private:
    double temporary;

protected:
    static void _bind_methods();

public:
    StarterBike();
    ~StarterBike();

    // for some reason, _physics_process takes a double, and not a float as they've defined it as _physics_process(float: delta) in
    // GDScript :) 
    void _physics_process(double delta) override;
};

} // namespace godot
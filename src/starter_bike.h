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

    void _physics_process(double delta) override;
};

} // namespace godot
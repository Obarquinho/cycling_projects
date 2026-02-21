extends Node3D


const CAMERA_X_ROT_MIN: float = deg_to_rad(-89.9)
const CAMERA_X_ROT_MAX: float = deg_to_rad(70.0)

func _input(input_event:InputEvent) -> void:
	if input_event is InputEventMouseMotion and Input.is_action_pressed("LMB"):
		rotate_camera(input_event.screen_relative * 0.001)
		
func rotate_camera(move: Vector2) -> void:
	rotate_y(-move.x)
	orthonormalize()
	rotation.x = clampf(rotation.x + move.y, CAMERA_X_ROT_MIN, CAMERA_X_ROT_MAX)

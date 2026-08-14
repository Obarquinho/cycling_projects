extends Camera3D

#const CAMERA_X_ROT_MIN: float = deg_to_rad(-89.9)
#const CAMERA_X_ROT_MAX: float = deg_to_rad(70.0)
#
#func _input(input_event:InputEvent) -> void:
	#if input_event is InputEventMouseMotion:
		#rotate_camera(input_event.screen_relative * 0.001)
		#
#func rotate_camera(move: Vector2) -> void:
	#rotate_y(-move.x)
	#orthonormalize()
	#rotation.x = clampf(rotation.x + move.y, CAMERA_X_ROT_MIN, CAMERA_X_ROT_MAX)

func _on_starter_bike_bike_speed(spd: float) -> void:
	fov = 75 + spd

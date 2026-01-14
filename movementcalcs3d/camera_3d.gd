extends Camera3D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.



func _physics_process(delta: float) -> void:
	#var target := (get_parent() as Node3D).get_global_transform().origin
	#var pos := get_global_transform().origin
#
	#var difference := pos - target
	#
	#difference.y = clamp(difference.y, 0, 2)
	#
	## Apply lookat.
	#if difference.is_zero_approx():
		#difference = (pos - target).normalized() * 0.0001
#
	#pos = target + difference
#
	#look_at_from_position(pos, target, Vector3.UP)
	pass

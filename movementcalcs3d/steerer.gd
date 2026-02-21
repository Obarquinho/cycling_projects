extends Node3D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	var direction = 0;
	if Input.is_action_pressed("TurnLeft"):
		direction = 1;
	if Input.is_action_pressed("TurnRight"):
		direction = -1;
	rotate_object_local(Vector3(0,1,0), direction * 0.01)

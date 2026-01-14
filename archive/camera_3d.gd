extends Camera3D


@export var first = true
@export var prevPos = Vector3(0,0,0) # this is the position of the player, not camera

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
	


func _on_player_bike_current_pos(x: Variant, y: Variant, z: Variant, phi: Variant) -> void:
	#if first:
		#position.x = x
		#position.y = y + 3
		#position.z = z + 5
		#first=false
	#else:
		#var desiredPos = Vector3(x, y + 3, z + 5)
		##desiredPos -= prevPos
		#position = desiredPos.rotated(Vector3.UP, phi)
	position.x = x
	position.y = y + 3
	position.z = z + 5
	
	var velocity = Vector3(0,0,5).rotated(Vector3(0,1,0), phi)
	position += velocity
	var pos = Vector3(x,y,z)
	look_at(pos)
	prevPos = pos

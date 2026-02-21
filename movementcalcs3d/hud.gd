extends CanvasLayer

var DEBUG_MODE = true
func debugMode(flag):
	DEBUG_MODE = flag
	$DebugSpan.text = "Debug Mode: {}".format([DEBUG_MODE], "{}")
	
func updateVelocity(unit):
	$Velocity.text = "Speed: %0.3f" % unit

func updateAcceleration(unit):
	$Acceleration.text = "Accel: %0.3f" % unit

func updateSpeedCap(unit, shouldDisplay):
	if shouldDisplay:
		$SpeedCap.text = "SpeedCap: %0.0f" % unit
	else:
		$SpeedCap.text = "No SpeedCap"

func updateEffort(unit):
	$EffortLevel.text = "EffortLevel: %0.0f" % unit

func updateGear(unit):
	$Gear.text = "Gear: %d" % unit

func updateNRG(unit):
	$NRG.text = "NRG: %0.2f" % unit

func updateRotation(x, y, z):
	x = "%0.2f" % x
	y = "%0.2f" % y
	z = "%0.2f" % z
	if DEBUG_MODE:
		$Rotation.text = "x: {} y: {} z: {}".format([x,y,z],"{}")
	else:
		$Rotation.text = ""
		
func updateFloorAngle(unit):
	if DEBUG_MODE:
		$FloorAngle.text = "floor angle %0.3f" % unit
	else:
		$FloorAngle.text = ""


func _on_starter_bike_bike_speed(spd: float) -> void:
	updateVelocity(spd)

func _on_starter_bike_bike_acceleration(accel: float) -> void:
	updateAcceleration(accel);


func _on_starter_bike_bike_rotation(bike_rotation: Vector3) -> void:
	updateRotation(bike_rotation.x, bike_rotation.y, bike_rotation.z)


func _on_starter_bike_effort(effort: float) -> void:
	updateEffort(effort)


func _on_starter_bike_energy(nrg: float) -> void:
	updateNRG(nrg)


func _on_starter_bike_floor_angle(floor_angle: float) -> void:
	updateFloorAngle(floor_angle)


func _on_starter_bike_gear(gear: int) -> void:
	updateGear(gear)

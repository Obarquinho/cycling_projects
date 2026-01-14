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

extends CanvasLayer

func updateVelocity(unit):
	$Velocity.text = "Speed: %0.1f" % unit

func updateAcceleration(unit):
	$Acceleration.text = "Accel: %0.1f" % unit

func updateSpeedCap(unit, shouldDisplay):
	if shouldDisplay:
		$SpeedCap.text = "SpeedCap: %0.0f" % unit
	else:
		$SpeedCap.text = "No SpeedCap"

func updateEffort(unit):
	$EffortLevel.text = "EffortLevel: %0.0f" % unit

func updateDebug():
	$DEBUG.text = "ALERT"
	$DEBUG.show()
	await get_tree().create_timer(1.0).timeout
	$DEBUG.hide()

func updateGear(unit):
	$Gear.text = "Gear: %d" % unit

func updateNRG(unit):
	$NRG.text = "NRG: %0.2f" % unit

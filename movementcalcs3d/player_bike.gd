extends CharacterBody3D

#It took me 20 seconds to go from 0 to 20 mph, so if you were to llineary to do thatm 1mph a second acceleration
# Would put base acceleration at like 0.5
# Gearing
#var GEARS = [[0, 800], [200, 1000], [1000, 1200]]
#var GEARS = [[0,300], [200, 600], [400, 800]]
var GEARS = [[0, 5], [3, 8], [6, 12]] # These are in meters/sec
var gear = 0
var MAX_GEAR = 2
var gear_penalty = [0.5, 1.0] # 0.0001

# Movement
@export var speed = 0 # Our own tracking of velocity
@export var angular_speed = PI
#var velocity = Vector2.ZERO this is a property of 3dNode
var acceleration = 0
var screen_size # Size of game window
#var MAX_SPEED = 300 # was 800
var MAX_SPEED = GEARS[0][1]
var SPEED_CAP = MAX_SPEED # change to lower case this isn't a constant
var TEMP_SPEED_CAP = 0
var prev_speed_cap = 0
var target_speed = MAX_SPEED
var FRICTION = 0.05
#var BASE_ACCELERATION = 2.0
var BASE_ACCELERATION = 0.3
var BASE_BRAKE = 5
const JUMP_VELOCITY = 4.5
const STEERING_INTENSITY = 0.1
const MAX_STEER = 1

#wheels
var front_speed = 0
var front_angle = 0 # radians and relative to parallel axis of main body (i.e. cos is the z vector)
var rear_speed = 0
var front_angle_glob = 0

var is_cruising = false

# Effort vars
var MAX_EFFORT = 100.0
var EFFORT_INTERVAL = 10.0
var effort = MAX_EFFORT
var effort_factor = 1.0
var need_flip = false # Flag that I do not want in this code

# Energy vars
var energy = 100.0
var BASE_DEPLETION = -0.005

var debug_tog = true

var MAX_PITCH = -70

#debug no actual use
var prev_rot = 0

func _physics_process(delta: float) -> void:
	var cur_velocity = velocity # not used just stored
	var cur_acceleration = 0 
	speed = sqrt( velocity.x*velocity.x + velocity.z*velocity.z )
	front_speed = rear_speed / cos(front_angle)
	rear_speed =  speed
	
	# House keeping
	if Input.is_action_just_pressed("DebugToggle"):
		debug_tog = not debug_tog
		$HUD.debugMode(debug_tog)
		
	if Input.is_action_just_pressed("ShiftUp"):
		gear = min(gear + 1, 2)
		MAX_SPEED = GEARS[gear][1]
		effort_calculations()
		
	if Input.is_action_just_pressed("ShiftDown"):
		gear = max(gear - 1, 0)
		MAX_SPEED = GEARS[gear][1]
		effort_calculations()

	# Handle jump.
	if Input.is_action_just_pressed("Jump") and is_on_floor():
		velocity.y = JUMP_VELOCITY
	
	#speed = sqrt( velocity.x*velocity.x + velocity.z*velocity.z )
	var direction = 0
	var z_direction = 0
	if Input.is_action_pressed("TurnLeft"):
		front_angle += STEERING_INTENSITY
		#direction = 1
	if Input.is_action_pressed("TurnRight"):
		front_angle -= STEERING_INTENSITY
		#direction = -1
	front_angle = max(-1 * MAX_STEER, min(MAX_STEER, front_angle))
	if Input.is_action_pressed("PitchUp"):
		print(get_floor_angle())
		z_direction = 1
	if Input.is_action_pressed("PitchDown"):
		z_direction = -1
	
	if z_direction == 0 and is_on_floor():
		z_direction = -0.6
	
	#rotation += Vector3(0, angular_speed * direction * delta, 0)
	#var angular_change = angular_speed * direction * delta
	var angular_change = front_angle * delta
	#front_angle += angular_change
	front_angle_glob = angular_change * 0.1
	rotate_y(angular_change)
	# cancel mass rotation on Front wheel
	print(angular_change)
	print(front_angle_glob)
	#$FrontWheel.rotate_y(-1*angular_change)
	
	
	#$FrontWheel.rotate_y(angular_speed * direction * delta * 2)
	
	if z_direction != 0:
		rotate_object_local(Vector3(-1,0,0), angular_speed *z_direction * delta)
	
	if rotation.x > -(get_floor_angle()) and is_on_floor() :
		rotation.x = -(get_floor_angle())
	elif rotation_degrees.x < MAX_PITCH and is_on_floor():
		rotation_degrees.x = MAX_PITCH
	
	if rotation.z != 0:
		rotation.z = 0
	
	#rotate_x(angular_speed * z_direction * delta)
	
	if Input.is_action_pressed("Pedal"):
		var pedal_acceleration = FRICTION + BASE_ACCELERATION * effort_factor * get_penalty(GEARS[gear][0], gear_penalty)
		if pedal_acceleration <= FRICTION:
			print("Pedal accel {} is less than friction {}".format([pedal_acceleration, FRICTION], "{}"))
		cur_acceleration += pedal_acceleration
	if Input.is_action_pressed("Brake"):
		cur_acceleration -= BASE_BRAKE
	if Input.is_action_just_released("Pedal"):
		acceleration = 0
	if Input.is_action_just_released("Brake"):
		acceleration = 0
		front_angle = 0 
		
	# effort check
	if Input.is_action_just_pressed("IncreaseEffort"):
		if effort < MAX_EFFORT:
			effort += EFFORT_INTERVAL
			effort_calculations()
	if Input.is_action_just_pressed("DecreaseEffort"):
		if effort > 0:
			effort -= EFFORT_INTERVAL
			effort_calculations()
	
	# was if speed > 0, can actually still just check if veloctiy is not equal to (0,0,0) ?
	# TODO Check this calculation in comparison to speed, it might make sense logically IDK
	if velocity != Vector3.ZERO:
		cur_acceleration -= FRICTION
	
	acceleration_calcs(cur_acceleration)

	# https://physics.stackexchange.com/questions/182689/what-causes-a-cars-velocity-to-follow-the-front-wheels-direction
	
	var dx = speed - rear_speed
	var dxf = dx * cos(front_angle)
	var dyf = dx * sin(front_angle)
	
	# assuming cur_acceleration in the base case is going in the Z direction?
	var trueRotation = ($FrontWheel.global_rotation.y + global_rotation.y) * 0.5
	trueRotation = global_rotation.y + front_angle_glob
	trueRotation = rotation.y + front_angle
	print("main: {} front: {} frame: {}".format([trueRotation,$FrontWheel.global_rotation.y,global_rotation.y],"{}"))
	#velocity = Vector3(0,velocity.y,speed).rotated(Vector3.UP, rotation.y)
	velocity = Vector3(0,velocity.y,speed).rotated(Vector3.UP, trueRotation)
	print("HELLO")
	
	$FrontWheel.global_rotation.y = global_rotation.y
	$FrontWheel.global_rotation.y += front_angle
	
	if prev_rot != rotation.y:
		prev_rot = rotation.y
		print("GREP {}".format([$FrontWheel.rotation.y], "{}"))
	#speed = sqrt( velocity.x*velocity.x + velocity.z*velocity.z )
	
	# gravity
	if not is_on_floor():
		print("HELLO")
		velocity += get_gravity() * delta

	var rot = rotation_degrees
	move_and_slide()
	update_energy()
	$FrontWheel.rotate_object_local(Vector3(1,0,0), speed * delta)
	$RearWheel.rotate_object_local(Vector3(1,0,0), speed * delta)
	$HUD.updateVelocity(speed)
	$HUD.updateAcceleration(acceleration)
	$HUD.updateSpeedCap(SPEED_CAP, is_cruising)
	$HUD.updateEffort(effort)
	$HUD.updateGear(gear + 1)
	$HUD.updateNRG(energy)
	$HUD.updateRotation(rotation_degrees.x, rotation_degrees.y, rotation_degrees.z)
	$HUD.updateRotation($FrontWheel.rotation_degrees.x, $FrontWheel.rotation_degrees.y, $FrontWheel.rotation_degrees.z)
	$HUD.updateFloorAngle(get_floor_angle())
	#current_pos.emit(position.x, position.y, position.z, rotation.y)

func effort_calculations():
	effort_factor = effort/MAX_EFFORT
	prev_speed_cap = SPEED_CAP
	SPEED_CAP = MAX_SPEED * effort_factor
	print(prev_speed_cap," ",SPEED_CAP)
	need_flip = true

func get_penalty(min_speed, bounds):
	if speed >= min_speed:
		return 1.0
	var ratio = (min_speed - speed)/min_speed
	ratio = speed/min_speed
	# 960 - 959
	var p = bounds[1] - bounds[0]
	return (p * ratio) + bounds[0]

func update_energy():
	if acceleration < 0:
		return
	if Input.is_action_pressed("Pedal"):
		energy += (BASE_DEPLETION * effort_factor * max(acceleration,0)) + (BASE_DEPLETION * effort_factor * speed/MAX_SPEED)

func acceleration_calcs(alpha):
	# base case we are adding the acceleration to the speed. 
	acceleration = alpha
	speed += acceleration
	
	if speed >= SPEED_CAP and not need_flip: # had check of prev_speed_cap
		speed = SPEED_CAP
		acceleration = 0
	elif speed < SPEED_CAP and need_flip:
		need_flip = false
		prev_speed_cap = 0 # reset
		acceleration = max(acceleration, 0)
	elif speed >= SPEED_CAP: # Inversely mean prev_speed_cap > SPEED_CAP
		if acceleration > (-1 * BASE_BRAKE):
			speed -= acceleration # undo previous acceleration
			acceleration -= alpha
			acceleration = min(0, acceleration)
			acceleration -= FRICTION
			speed += acceleration
		if speed <= SPEED_CAP:
			# $HUD.updateDebug()
			prev_speed_cap = 0 # reset
			speed = SPEED_CAP
			acceleration = 0
	
	if speed <= 0:
		speed = 0
		acceleration = 0

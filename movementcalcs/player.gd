extends Area2D

# Gearing
#var GEARS = [[0, 800], [200, 1000], [1000, 1200]]
var GEARS = [[0,300], [200, 600], [400, 800]]
var gear = 0
var MAX_GEAR = 2
var gear_penalty = [0.5, 1.0] # 0.0001

# Movement
@export var speed = 0 # How fast the player will move (pixels/sec). was 400
@export var angular_speed = PI
var velocity = Vector2.ZERO
var acceleration = 0
var screen_size # Size of game window
#var MAX_SPEED = 300 # was 800
var MAX_SPEED = GEARS[0][1]
var SPEED_CAP = MAX_SPEED # change to lower case this isn't a constant
var TEMP_SPEED_CAP = 0
var prev_speed_cap = 0
var target_speed = MAX_SPEED
var FRICTION = 0.05
var BASE_ACCELERATION = 2.0
var BASE_BRAKE = 5

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


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	screen_size = get_viewport_rect().size
	
	start(Vector2(screen_size.x / 2.0, screen_size.y / 2.0))
	#hide()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	var cur_velocity = Vector2.ZERO # The player's movement vector.
	var cur_acceleration = 0
	
	if Input.is_action_just_pressed("ShiftUp"):
		gear = min(gear + 1, 2)
		MAX_SPEED = GEARS[gear][1]
		effort_calcuations()
		
	if Input.is_action_just_pressed("ShiftDown"):
		gear = max(gear - 1, 0)
		MAX_SPEED = GEARS[gear][1]
		effort_calcuations()
	
	# Deprecate this, is buggy
	if Input.is_action_just_pressed("ToggleCruise"):
		if is_cruising:
			SPEED_CAP = TEMP_SPEED_CAP
			is_cruising = false
		else:
			TEMP_SPEED_CAP = SPEED_CAP
			SPEED_CAP = speed
			is_cruising = true
	
	var direction = 0
	if Input.is_action_pressed("TurnLeft"):
		direction = -1
	if Input.is_action_pressed("TurnRight"):
		direction = 1
	
	rotation += angular_speed * direction * delta
	
	#if velocity.length() > 0:
		#velocity = velocity.normalized() * speed
		#$AnimatedSprite2D.play()
	#else:
		#$AnimatedSprite2D.stop()
		
	if Input.is_action_pressed("Pedal"):
		cur_acceleration += BASE_ACCELERATION * effort_factor * get_penalty(GEARS[gear][0], gear_penalty)
	if Input.is_action_pressed("Brake"):
		#cur_acceleration -= 0.5
		cur_acceleration -= BASE_BRAKE
	if Input.is_action_just_released("Pedal"):
		acceleration = 0
	if Input.is_action_just_released("Brake"):
		acceleration = 0
	if Input.is_action_just_pressed("IncreaseEffort"):
		if effort < MAX_EFFORT:
			effort += EFFORT_INTERVAL
			effort_calcuations()
	if Input.is_action_just_pressed("DecreaseEffort"):
		if effort > 0:
			effort -= EFFORT_INTERVAL
			effort_calcuations()
	
	if speed > 0:
		cur_acceleration -= FRICTION
		
	#acceleration += cur_acceleration
	acceleration = cur_acceleration
	speed += acceleration
	
	if speed >= SPEED_CAP and not need_flip: # had check of prev_speed_cap
		speed = SPEED_CAP
		acceleration = 0
	elif speed < SPEED_CAP and need_flip:
		need_flip = false
		prev_speed_cap = 0 # reset
		acceleration = max(acceleration, 0)
	elif speed >= SPEED_CAP: # inversely means prev_speed_cap > SPEED_CAP
		if acceleration > (-1 * BASE_BRAKE):
			speed -= acceleration # undo previous acceleration
			acceleration -= cur_acceleration
			acceleration = min(0, acceleration)
			acceleration -= FRICTION
			speed += acceleration
		if(speed <= SPEED_CAP):
			$HUD.updateDebug()
			prev_speed_cap = 0 # reset
			speed = SPEED_CAP
			acceleration = 0
	
	if speed <= 0:
		speed = 0
		acceleration = 0
	
	#speed *= effort / MAX_EFFORT
	velocity = Vector2.UP.rotated(rotation) * speed
	
	position += velocity * delta
	position = position.clamp(Vector2.ZERO, screen_size)
	update_energy()
	$HUD.updateVelocity(speed)
	$HUD.updateAcceleration(acceleration)
	$HUD.updateSpeedCap(SPEED_CAP, is_cruising)
	$HUD.updateEffort(effort)
	$HUD.updateGear(gear + 1)
	$HUD.updateNRG(energy)
	
func start(pos):
	position = pos
	show()
	$CollisionShape2D.disabled = false
	
func effort_calcuations():
	effort_factor = effort/MAX_EFFORT
	prev_speed_cap = SPEED_CAP
	SPEED_CAP = MAX_SPEED * effort_factor
	print(prev_speed_cap, SPEED_CAP)
	need_flip = true

func simple_speed_calc():
	if speed >= SPEED_CAP:
		speed = SPEED_CAP
		acceleration = 0

# This does bad behavior when LERPING + Pedaling + Braking
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

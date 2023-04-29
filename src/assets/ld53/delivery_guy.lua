SPEED = 100

MIN_Y = -180
MAX_Y = 180
MIN_X = -305
MAX_X = 305

velocity_x = 0
velocity_y = 0

INITIAL_ROTATION = 0
GOAL_ROTATION = 0
START_SCALE_X = 1.5
START_SCALE_Y = 1.5

function Initialize(self)
    INITIAL_ROTATION = _GetRotation(self)
    _SetScale(self, START_SCALE_X, SCALE_SCALE_Y)
end

function BeginPlay(self)
end

function ProcessInput(self, keyboard_state)
    if(keyboard_state.a_held) then
        GOAL_ROTATION = INITIAL_ROTATION - 0.25
        velocity_x = -SPEED
    end

    if(keyboard_state.d_held) then
        GOAL_ROTATION = INITIAL_ROTATION + 0.25
        velocity_x = SPEED
    end

    if(keyboard_state.w_held) then
        velocity_y = SPEED
    end

    if(keyboard_state.s_held) then
        velocity_y = -SPEED
    end
end

function Update(self, dt)
    -- /////////////////////////////////////////////////
    -- blocking velocity if we are out of boundaries
    x_pos = _GetPositionX(self)
    y_pos = _GetPositionY(self)

    if(x_pos > MAX_X and velocity_x > 0) then
        velocity_x = 0
    end

    if(x_pos < MIN_X and velocity_x < 0) then
        velocity_x = 0
    end

    if(y_pos > MAX_Y and velocity_y > 0) then
        velocity_y = 0
    end

    if(y_pos < MIN_Y and velocity_y < 0) then
        velocity_y = 0
    end


    -- /////////////////////////////////////////////////
    -- Effectively Moving
    _Move(self, velocity_x * dt, velocity_y * dt)
    

    -- // //////////////////////////////////////////////
    -- // Doing weird stuff with the scale
    scale_x = START_SCALE_X + (math.random() / 4) -- make it in between 0 and 0.1
    scale_y = START_SCALE_Y + (math.random() / 4) -- make it in between 0 and 0.1
    _SetScale(self, scale_x, scale_y)
    
    -- // //////////////////////////////////////////////
    -- // Rotating
    current_rotation = _GetRotation(self)
    current_rotation = current_rotation + ((GOAL_ROTATION - current_rotation) * 10*dt)
    _SetRotation(self, current_rotation)

    if(velocity_x < 10 and velocity_x > -10) then
        GOAL_ROTATION = 0
    end

    -- /////////////////////////////////////////////////
    -- adding some sort of acceleration simulation...
    if(velocity_x < 0) then
        velocity_x = velocity_x + (SPEED * dt)
    end

    if(velocity_x > 0) then
        velocity_x = velocity_x - (SPEED * dt)
    end

    if(velocity_y > 0) then
        velocity_y = velocity_y - (SPEED * dt)
    end

    if(velocity_y < 0) then
        velocity_y = velocity_y + (SPEED * dt)
    end
end

function Destroy(self)
end
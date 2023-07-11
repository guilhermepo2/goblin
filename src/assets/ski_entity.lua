g_internal_rotation = 0
SPEED = 100

MIN_Y = -155
MAX_Y = 156
MIN_X = -80
MAX_X = 57

velocity_x = 0
velocity_y = 0

function Initialize(self)
end

function BeginPlay(self)
end

function ProcessInput(self, keyboard_state)
    if(keyboard_state.a_held) then
        velocity_x = -SPEED
    end

    if(keyboard_state.d_held) then
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
    -- rot = _GetRotation(self)
    -- _SetRotation(self, rot + (1 * dt))

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
g_internal_rotation = 0

function Initialize(self)
end

function BeginPlay(self)
end

function ProcessInput(self)
end

function Update(self, dt)
    _Move(self, 0, -0.25)
    -- rot = _GetRotation(self)
    -- _SetRotation(self, rot + (1 * dt))
end

function Destroy(self)
end
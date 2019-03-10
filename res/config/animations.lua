local IMGDIR = "res/textures/combat/animations/"
local SNDDIR = "res/sounds/combat/"

COMBAT_ANIMATION {
    name = "unarmed_right",
    width = 16,
    height = 16,
    frames = { IMGDIR.."unarmed_right.png" },
    type = "at_target",
    speed = 1.0,
    duration = 0.6,
    sound = SNDDIR.."unarmed.wav"
}

COMBAT_ANIMATION {
    name = "unarmed_left",
    width = 16,
    height = 16,
    frames = { IMGDIR.."unarmed_left.png" },
    type = "at_target",
    speed = 1.0,
    duration = 0.6,
    sound = SNDDIR.."unarmed.wav"
}

COMBAT_ANIMATION {
    name = "basic_attack",
    width = 16,
    height = 16,
    frames = { IMGDIR.."basic_attack.png" },
    type = "at_target",
    speed = 1.0,
    duration = 0.6,
    sound = SNDDIR.."basic_attack.wav"
}

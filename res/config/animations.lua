local ROOT_DIR = "res/textures/combat/animations/"

COMBAT_ANIMATION {
    name = "unarmed_right",
    width = 16,
    height = 16,
    frames = { ROOT_DIR.."unarmed_right.png" },
    type = "at_target",
    speed = 1.0,
    duration = 0.6
}

COMBAT_ANIMATION {
    name = "unarmed_left",
    width = 16,
    height = 16,
    frames = { ROOT_DIR.."unarmed_left.png" },
    type = "at_target",
    speed = 1.0,
    duration = 0.6
}

COMBAT_ANIMATION {
    name = "basic_attack",
    width = 16,
    height = 16,
    frames = { ROOT_DIR.."basic_attack.png" },
    type = "at_target",
    speed = 1.0,
    duration = 0.6
}

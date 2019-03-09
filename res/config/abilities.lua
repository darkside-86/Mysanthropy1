
-- PLAYER ABILITIES -----------------------------------------------------------
-------------------------------------------------------------------------------

ABILITY {
    name = "unarmed_right",
    minRange = 0,
    maxRange = 32,
    offensive = true,
    cooldown = 1.0,
    onGCD = false,
    castType = "instant",
    castTime = 0,
    formula = "!<(0.25,0.5)MAP,Physical;" -- todo: add fist weapon damage
}

ABILITY {
    name = "unarmed_left",
    minRange = 0,
    maxRange = 32,
    offensive = true,
    cooldown = 1.1,
    onGCD = false,
    castType = "instant",
    castTime = 0,
    formula = "!<(0.2,0.4)MAP+1NIL,Physical;"
}

-- PLAYER CONFIGURATIONS ------------------------------------------------------
-------------------------------------------------------------------------------

ABILITY_TABLE {
    name = "player_survivalist",
    attacks = { "unarmed_right", "unarmed_left" }
}

-- NPC ABILITIES --------------------------------------------------------------
-------------------------------------------------------------------------------
ABILITY {
    name = "basic_attack",
    minRange = 0,
    maxRange = 32,
    offensive = true,
    cooldown = 1.0,
    onGCD = false,
    castType = "instant",
    castTime = 0,
    formula = "!<(0.1,0.5)MAP+1NIL,Physical;"
}

-- NPC CONFIGURATIONS ---------------------------------------------------------
-------------------------------------------------------------------------------

ABILITY_TABLE {
    name = "redcrab",
    attacks = { "basic_attack" }
}
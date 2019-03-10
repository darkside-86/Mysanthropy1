
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
    formula = "!<(0.25,0.5)MAP+0.5LVL,Physical;", -- todo: add fist weapon damage
    level = 1
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
    formula = "!<(0.2,0.4)MAP+0.5LVL,Physical;",
    level = 1
}

ABILITY {
    name = "throw_stone",
    minRange = 48,
    maxRange = 96,
    offensive = true,
    cooldown = 5.0,
    onGCD = true,
    castType = "instant",
    castTime = 0,
    formula = "!<(0.2)RAP,Earth[1stone];!?0.5SPD,Earth:8;",
    level = 5
}

-- PLAYER CONFIGURATIONS ------------------------------------------------------
-------------------------------------------------------------------------------

ABILITY_TABLE {
    name = "player_survivalist",
    attacks = { "unarmed_right", "unarmed_left", "throw_stone" }
}

-- NPC ABILITIES --------------------------------------------------------------
-------------------------------------------------------------------------------
ABILITY {
    name = "basic_attack",
    minRange = 0,
    maxRange = 32,
    offensive = true,
    cooldown = 1.0,
    onGCD = true,
    castType = "instant",
    castTime = 0,
    formula = "!<(0.2,0.4)MAP+0.5LVL,Physical;",
    level = 1
}

-- NPC CONFIGURATIONS ---------------------------------------------------------
-------------------------------------------------------------------------------

ABILITY_TABLE {
    name = "redcrab",
    attacks = { "basic_attack" }
}

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
    targetType = "enemy",
    rangeType = "target",
    weaponsRequired = { "unarmed" },
    formula = "<(0.25,0.5)MAP+0.5LVL+1WPN,Physical;",
    level = 1,
    animation = "unarmed_right"
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
    targetType = "enemy",
    rangeType = "target",
    weaponsRequired = { "unarmed" },
    formula = "<(0.2,0.4)MAP+0.5LVL+1WPN,Physical;",
    level = 1,
    animation = "unarmed_left"
}

ABILITY {
    name = "sword_right",
    minRange = 0,
    maxRange = 35,
    offensive = true,
    cooldown = 1.4,
    onGCD = false,
    castType = "instant",
    castTime = 0,
    targetType = "enemy",
    rangeType = "target",
    weaponsRequired = { "sword1h" },
    formula = "<(0.35,0.7)MAP+0.5LVL+1WPN,Physical;",
    level = 1,
    animation = "sword_right"
}

ABILITY {
    name = "sword_left",
    minRange = 0,
    maxRange = 35,
    offensive = true,
    cooldown = 1.5,
    onGCD = false,
    castType = "instant",
    castTime = 0,
    targetType = "enemy",
    rangeType = "target",
    weaponsRequired = { "sword1h" },
    formula = "<(0.28,0.56)MAP+0.5LVL+1WPN,Physical;", 
    level = 1,
    animation = "sword_left"
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
    targetType = "enemy",
    rangeType = "target",
    itemCost = { 1, "stone" },
    formula = "<(0.1)RAP,Earth;/Slow4.5;",
    level = 5
    -- TODO: animation
}

ABILITY {
    name = "first_aid",
    minRange = 0,
    maxRange = 32, -- meaningless because it's self cast
    offensive = false,
    cooldown = 15,
    onGCD = true,
    castType = "channeled",
    castTime = 5,
    targetType = "friendly",
    rangeType = "target",
    formula = ">(1,2)LVL,Physical;",
    tickRate = 1,
    level = 6
}

ABILITY {
    name = "gauze",
    minRange = 0,
    maxRange = 32,
    offensive = false,
    cooldown = 30,
    onGCD = true,
    castType = "instant",
    castTime = 0,
    targetType = "friendly",
    rangeType = "target",
    -- no formula, this is just a simple dispel
    dispels = { "Physical" }, -- only dispels one physical debuff. Could put physical twice e.g.
    level = 7
}

ABILITY {
    name = "basic_attack",
    minRange = 0,
    maxRange = 32,
    offensive = true,
    cooldown = 0,
    onGCD = true,
    castType = "instant",
    castTime = 0,
    targetType = "enemy",
    rangeType = "target",
    formula = "<(0.1,0.9)MAP;",
    level = 1
}



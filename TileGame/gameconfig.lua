-- The base movement speed in any direction of player (without speed buffs)
PLAYER_SPEED = 50.0
-- The tile on which player spawns (on island map) for new games
SPAWN_POINT = { 23, 234 }
-- The "Boy" LG sprite name
BOY_SURV = "man1"
-- The "Girl" LG sprite name
GIRL_SURV = "wmn2"
-- The amount of experience required per level increase
EXPERIENCE_SCALE = 1.33
-- The amount of core stat increase per level (which stats varies with class)
CORESTAT_SCALE = 1.11
-- The amount of non-core stat increase per level (which stats varies with class)
OTHERSTAT_SCALE = 1.001 
-- The increase in mob stats (all of its stats) per level
MOB_STAT_SCALE = 1.13
-- base level (level=1) for core stats (which stats varies according to class)
CORE_STATS = 10
-- base level (level=1) for non-core stats (which vary according to class)
OTHERSTAT_STATS = 5
-- the amount of exp to increase from level 1 to level 2
BASE_EXP = 250
-- the base experience value of a non-elite level 1 mob
BASE_MOB_EXP = 25
-- the amount of experience per mob's level given by killing a non-elite mob
BASE_MOB_EXP_SCALE = 1.25
-- the experience penalty for killing lower level mobs per level difference
MOB_EXP_PENALTY = 0.9
-- Entries for all possible inventory items
ITEM_ENTRY {
    name = "exp",
    hidden = true,
    texture = "res/textures/items/exp.png"
}

ITEM_ENTRY {
    name = "foodstuff",
    hidden = false, -- for now
    texture = "res/textures/items/foodstuff.png"
}

ITEM_ENTRY {
    name = "wood",
    texture = "res/textures/items/wood.png"
}

ITEM_ENTRY {
    name = "grass",
    texture = "res/textures/items/grass.png"
}

ITEM_ENTRY {
    name = "stone",
    texture = "res/textures/items/stone.png"
}

ITEM_ENTRY {
    name = "coconut",
    texture = "res/textures/items/coconut.png"
}

ITEM_ENTRY {
    name = "banana",
    texture = "res/textures/items/banana.png"
}

ITEM_ENTRY {
    name = "vine",
    texture = "res/textures/items/vine.png"
}

ITEM_ENTRY {
    name = "shellfish",
    texture = "res/textures/items/shellfish.png"
}

ITEM_ENTRY {
    name = "carnivora",
    texture = "res/textures/items/carnivora.png"
}

ITEM_ENTRY {
    name = "carnivora_seed",
    texture = "res/textures/items/carnivora_seed.png"
}

ITEM_ENTRY {
    name = "water",
    texture = "res/textures/items/water.png"
}
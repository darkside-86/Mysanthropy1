PLAYER_SPEED = 50.0
SPAWN_POINT = { 23, 234 }
BOY_SURV = "man1"
GIRL_SURV = "wmn2"
-- logarithmic scales
EXPERIENCE_SCALE = 1.33 -- experience requirement multiplier
CORESTAT_SCALE = 1.11 -- increase in class core stats per level
OTHERSTAT_SCALE = 1.001 -- increase in non-core class stats per level
MOB_DIFFICULTY_SCALE = 1.12 -- increase in mob difficulty per level
-- stat levels
CORE_STATS = 10 -- level 1 stats (varies with class)
OTHERSTAT_STATS = 5 -- level 1 non-core stats (varies with class)
-- level 1 experience max
BASE_EXP = 500
BASE_MOB_EXP = 5
BASE_MOB_EXP_SCALE = 1.30

ITEM_ENTRY {
    name = "exp",
    hidden = true,
    texture = "res/textures/items/exp.png"
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
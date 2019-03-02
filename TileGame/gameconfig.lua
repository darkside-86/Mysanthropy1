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
BASE_EXP = 250
BASE_MOB_EXP = 25
BASE_MOB_EXP_SCALE = 1.30

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
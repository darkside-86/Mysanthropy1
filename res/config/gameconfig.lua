-- The base movement speed in any direction of player (without speed buffs)
PLAYER_SPEED = 50.0
-- The tile on which player spawns (on island map) for new games
SPAWN_POINT = { 23, 234 }
-- The "Boy" LG sprite name
BOY_SURV = "man1"
-- The "Girl" LG sprite name
GIRL_SURV = "wmn2"

-- Chance (0-100) per second of time between saves a fully destroyed object has to regenerate
ENT_REGEN_CHANCE = 0.00005

-- The amount of experience required per level increase
EXPERIENCE_SCALE = 1.33
-- the amount of exp to increase from level 1 to level 2
BASE_EXP = 250
-- the base experience value of a non-elite level 1 mob
BASE_MOB_EXP = 10
-- the amount of experience per mob's level given by killing a non-elite mob
BASE_MOB_EXP_SCALE = 1.25
-- the experience penalty for killing lower level mobs per level difference
MOB_EXP_PENALTY = 0.8

-- The amount of core stat increase per level (which stats varies with class)
CORESTAT_SCALE = 1.11
-- The amount of non-core stat increase per level (which stats varies with class)
OTHERSTAT_SCALE = 1.01 

-- The increase in mob stats (all of its stats) per level
MOB_STAT_SCALE = 1.13
-- base level (level=1) for core stats (which stats varies according to class)
CORE_STATS = 5
-- base level (level=1) for non-core stats (which vary according to class)
OTHER_STATS = 4

-- lucky survivalist gets 2 AP stats as well as health
SURVIVALIST_CORE_STATS = {
    "Strength", "Agility", "Vitality"
}


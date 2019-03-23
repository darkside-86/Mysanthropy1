-- The base movement speed in any direction of player (without speed buffs)
PLAYER_SPEED = 50.0
-- Starting map to load
STARTING_MAP = "res/tilemaps/island.bin"
-- The tile on which player spawns (on island map) for new games
SPAWN_POINT = { 23, 234 }
-- The "Boy" LG sprite name
BOY_SURV = "man1"
-- The "Girl" LG sprite name
GIRL_SURV = "wmn2"
-- TODO: full LG sprite configuration done from Lua rather than hardcoded in C++
--   ...
-- Sound to play when a non-spell cast is being performed
ACTION_SOUND = "res/sounds/harvest.wav"

-- Chance (0-100) per second of time between saves a fully destroyed object has to regenerate
ENT_REGEN_CHANCE = 0.00015

-- The amount of experience required per level increase
EXPERIENCE_SCALE = 1.33
-- the amount of exp to increase from level 1 to level 2
BASE_EXP = 250
-- the base experience value of a non-elite level 1 mob
BASE_MOB_EXP = 10
-- the amount of experience per mob's level given by killing a non-elite mob
BASE_MOB_EXP_SCALE = 1.2
-- the experience penalty for killing lower level mobs per level difference
MOB_EXP_PENALTY = 0.75

-- all base stats start as 5 and multipled by (configured_multipler)^(LVL-1)

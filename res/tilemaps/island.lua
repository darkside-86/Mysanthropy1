-- USEFUL CONSTANTS -----------------------------------------------------------
-------------------------------------------------------------------------------

-- number of seconds required to get one harvest out of item
local defaultClickTime = 6.0
-- time values are in seconds so use these for conversion
local minutes = 60.0
local hours = 60.0 * minutes

-- TILE ID CONFIGURATION ------------------------------------------------------
-------------------------------------------------------------------------------
-- when a tile causes the sprite to "swim"
LIQUIDS ( 
   {2,0} -- the full water tile
) 
-- when a tile can have a building placed on it
BUILDABLE (
   {0,0}, {1,0},                
   {0,1}, {1,1}, {2,1}, 
   {0,2}, {1,2}, {2,2},     {4,2}, 
   {0,3}, {1,3}, {2,3}, 
   {0,4}, {1,4}, {2,4}, 
   {0,5}, {1,5}, {2,5}, 
   {0,6}, {1,6}, {2,6}
) 
-- when the upper layer is empty it is filled with
EMPTY ( 5, 0 )

SWIMMING ( -- indicates what tile to paint for sprites that are swimming
   "res/textures/swimming.png"
)

-- ENTITY LIST ----------------------------------------------------------------
-------------------------------------------------------------------------------

-- 0 : palm1 ------------------------------------------------------------------
BEGIN_ENTITY "palm1"
 USE_TEXTURE "res/textures/objects/palm1.png"
 WIDTH (91)
 HEIGHT (128)
 COLLISION_BOX (25, 115, 35, 125)
 MAX_CLICKS (70)
 ON_INTERACT ( 
    100.0, 1, "wood",
    100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 10, "grass",
    100.0, 10, "wood",
    100.0, 10, "exp"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY ()
-- 1 : palm2 ------------------------------------------------------------------
BEGIN_ENTITY "palm2"
 USE_TEXTURE "res/textures/objects/palm2.png"
 WIDTH (114)
 HEIGHT (96)
 COLLISION_BOX (95, 75, 110, 90)
 MAX_CLICKS (70)
 ON_INTERACT ( 
    100.0, 1, "wood",
    100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 10, "grass",
    100.0, 10, "wood",
    100.0, 10, "exp"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY ()
-- 2 : palm3 ------------------------------------------------------------------
BEGIN_ENTITY "palm3"
 USE_TEXTURE "res/textures/objects/palm3.png"
 WIDTH (95)
 HEIGHT (128)
 COLLISION_BOX(55, 110, 70, 125)
 MAX_CLICKS (70)
 ON_INTERACT (
    100.0, 1, "wood",
    100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 10, "grass",
    100.0, 10, "wood",
    100.0, 10, "exp"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY ()
-- 3 : palm4 ------------------------------------------------------------------
BEGIN_ENTITY "palm4"
 USE_TEXTURE "res/textures/objects/palm4.png"
 WIDTH (99)
 HEIGHT (96)
 COLLISION_BOX(0,80,15,90)
 MAX_CLICKS (70)
 ON_INTERACT (
     100.0, 1, "wood",
     100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 10, "grass",
    100.0, 10, "wood",
    100.0, 10, "exp"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY ()
-- 4 : shrub1 -----------------------------------------------------------------
BEGIN_ENTITY "shrub1"
 USE_TEXTURE "res/textures/objects/shrub1.png"
 WIDTH (39)
 HEIGHT (32)
 COLLISION_BOX (-1, 0, 0, 0) -- no collision
 MAX_CLICKS (10)
 ON_INTERACT (
     50.0, 1, "grass",
     50.0, 1, "wood",
     100.0, 1, "exp"
 )
 ON_DESTROY (
     100.0, 5, "grass",
     100.0, 5, "exp"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY()
-- 5 : shrub2 -----------------------------------------------------------------
BEGIN_ENTITY "shrub2"
 USE_TEXTURE "res/textures/objects/shrub2.png"
 WIDTH (51)
 HEIGHT (32)
 COLLISION_BOX (-1, 0, 0, 0)
 MAX_CLICKS (25)
 ON_INTERACT (
     50.0, 1, "grass",
     50.0, 1, "wood",
     100.0, 1, "exp"
 )
 ON_DESTROY (
     100.0, 10, "grass",
     100.0, 10, "exp"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY()
-- 6 : grass1 -----------------------------------------------------------------
BEGIN_ENTITY "grass1"
 USE_TEXTURE "res/textures/objects/grass1.png"
 WIDTH (32)
 HEIGHT (26)
 COLLISION_BOX (-1, 0, 0, 0)
 MAX_CLICKS (15)
 ON_INTERACT (
    100.0, 1, "grass",
    100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 10, "grass",
    100.0, 10, "exp"
 )
 CLICK_TIME(defaultClickTime/2)
END_ENTITY()
-- 7 : coconut_palm -----------------------------------------------------------
BEGIN_ENTITY "coconut_palm"
 USE_TEXTURE "res/textures/objects/coconut_palm.png"
 WIDTH (91)
 HEIGHT (128)
 COLLISION_BOX (25, 115, 35, 125)
 MAX_CLICKS (80)
 ON_INTERACT (
    100.0, 1, "wood",
    100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 10, "grass",
    100.0, 10, "exp"
 )
 CLICK_TIME(defaultClickTime)
 FARMABLE (
    100.0, 2, "coconut", 8 * hours, "res/textures/objects/palm1.png"
 )
END_ENTITY()
-- 8 : banana_tree ------------------------------------------------------------
BEGIN_ENTITY "banana_tree"
 USE_TEXTURE "res/textures/objects/banana_tree.png"
 WIDTH (59)
 HEIGHT (64)
 COLLISION_BOX (25, 50, 35, 60)
 MAX_CLICKS (20)
 ON_INTERACT (
    100.0, 1, "wood",
    100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 10, "grass",
    100.0, 10, "exp"
 )
 CLICK_TIME(defaultClickTime)
 FARMABLE (
    100.0, 2, "banana", 6 * hours, "res/textures/objects/banana_tree_pending.png"
 )
END_ENTITY()
-- 9 : palm_with_vine1 --------------------------------------------------------
BEGIN_ENTITY "palm_with_vine1"
 USE_TEXTURE "res/textures/objects/palm_with_vine1.png"
 WIDTH (114)
 HEIGHT (96)
 COLLISION_BOX (95, 75, 110, 90)
 MAX_CLICKS ( 60 )
 ON_INTERACT (
    100.0, 1, "wood",
    100.0, 1, "exp",
    5.0, 1, "vine"
 )
 ON_DESTROY (
    100.0, 10, "grass",
    100.0, 10, "exp",
    5.0, 10, "vine"
 )
 CLICK_TIME(defaultClickTime)
 FARMABLE (
    100.0, 2, "vine", 4 * hours, "res/textures/objects/palm2.png"
 )
END_ENTITY()
-- 10 : palm_with_vine2 -------------------------------------------------------
BEGIN_ENTITY "palm_with_vine2"
 USE_TEXTURE "res/textures/objects/palm_with_vine2.png"
 WIDTH (95)
 HEIGHT (128)
 COLLISION_BOX(55, 110, 70, 125)
 MAX_CLICKS ( 50 )
 ON_INTERACT (
    100.0, 1, "wood",
    100.0, 1, "exp",
    5.0, 1, "vine"
 )
 ON_DESTROY (
    100.0, 10, "grass",
    100.0, 10, "exp",
    5.0, 10, "vine"
 )
 CLICK_TIME(defaultClickTime)
 FARMABLE (
    100.0, 2, "vine", 4 * hours, "res/textures/objects/palm3.png"
 )
END_ENTITY()
-- 11 : starfish --------------------------------------------------------------
BEGIN_ENTITY "starfish"
 USE_TEXTURE "res/textures/objects/starfish.png"
 WIDTH (16)
 HEIGHT (16)
 COLLISION_BOX (-1, 0, 0, 0)
 MAX_CLICKS (3)
 ON_INTERACT (
    100.0, 1, "shellfish",
    100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 3, "exp"
 )
 CLICK_TIME(defaultClickTime / 2)
END_ENTITY()
-- 12 : stone1 ----------------------------------------------------------------
BEGIN_ENTITY "stone1"
 USE_TEXTURE "res/textures/objects/stone1.png"
 WIDTH (66)
 HEIGHT (46)
 COLLISION_BOX (5,15,60,40)
 MAX_CLICKS (100)
 ON_INTERACT (
    100.0, 1, "stone",
    3.0, 1, "shellfish",
    100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 20, "stone",
    100.0, 20, "exp",
    3.0, 10, "shellfish"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY()
-- 13 : spring ----------------------------------------------------------------
BEGIN_ENTITY "spring"
 USE_TEXTURE "res/textures/objects/spring.png"
 WIDTH (64)
 HEIGHT (39)
 COLLISION_BOX (5, 9, 55, 26)
 MAX_CLICKS( -1 )
 CLICK_TIME(defaultClickTime)
 FARMABLE ( 100.0, 5, "water", 4 * hours, "res/textures/objects/spring_dry.png")
END_ENTITY ()
-- 14 : stone2 ----------------------------------------------------------------
BEGIN_ENTITY "stone2"
 USE_TEXTURE "res/textures/objects/stone2.png"
 WIDTH (64)
 HEIGHT (42)
 COLLISION_BOX (9, 20, 58, 32)
 MAX_CLICKS (120)
 ON_INTERACT (
    100.0, 1, "stone",
    100.0, 1, "exp"
 )
 ON_DESTROY (
    100.0, 20, "stone",
    100.0, 20, "exp"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY()
-- 15 : stone3 ----------------------------------------------------------------
BEGIN_ENTITY "stone3"
 USE_TEXTURE "res/textures/objects/stone3.png"
 WIDTH (59)
 HEIGHT (49)
 COLLISION_BOX(7, 12, 52, 38)
 MAX_CLICKS (140)
 ON_INTERACT (
   100.0, 1, "stone",
   100.0, 1, "exp",
   3.0, 1, "shellfish"
 )
 ON_DESTROY (
    100.0, 20, "stone",
    100.0, 20, "exp",
    3.0, 5, "shellfish"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY()
-- 16 : planewing -------------------------------------------------------------
BEGIN_ENTITY "planewing"
 USE_TEXTURE "res/textures/objects/planewing.png"
 WIDTH ( 99 )
 HEIGHT ( 64 )
 COLLISION_BOX ( 0, 0, 91, 56 )
 MAX_CLICKS ( 300 )
 ON_INTERACT (
    15.0, 1, "ironingot",
    20.0, 1, "nail",
    100.0, 1, "exp"
 )
 CLICK_TIME(defaultClickTime)
END_ENTITY()

-- Mob spawner types ----------------------------------------------------------
-------------------------------------------------------------------------------

-- 0 : redcrab ----------------------------------------------------------------
-- Begins a mob type entry. Argument is the name of the mob used for indexing and display
BEGIN_MOB_TYPE "redcrab"
 MOB_CLASS "basic_mob"
 -- lower and upper inclusive range the levels of the mobs that spawn
 LEVEL_RANGE (1, 5)
 -- default "anim0" texture. See next note.
 DEFAULT_ANIMATION "lf1.png"
 -- Front facing animation. textures are loaded as res/textures/sprites/mobs/%nameofmob%_%animtexturelistarg%
 --   Can be any number of frames not necessarily 6.
 FR_ANIM_TEXTURE_LIST (
  "lf1.png", "lf2.png", "lf3.png",
  "lf4.png", "lf5.png", "lf6.png"
 )
 -- Back facing animation. see above
 BK_ANIM_TEXTURE_LIST (
  "rt1.png", "rt2.png", "rt3.png",
  "rt4.png", "rt5.png", "rt6.png"
 )
 -- Right facing animation. see above.
 RT_ANIM_TEXTURE_LIST (
  "rt1.png", "rt2.png", "rt3.png",
  "rt4.png", "rt5.png", "rt6.png"
 )
-- Left facing animation. see above.
 LF_ANIM_TEXTURE_LIST (
  "lf1.png", "lf2.png", "lf3.png",
  "lf4.png", "lf5.png", "lf6.png"
 )
 -- The amount of seconds between each animation frame
 ANIM_SPEED ( 0.1 )
 -- The width in logical pixels of the mob
 MOB_WIDTH ( 20 )
 -- The height in logical pixels of the mob
 MOB_HEIGHT ( 20 )
 -- The default speed (unaffected by temp buffs) the mob can travel. TODO: handle water. But these mobs
 --   can swim so no swim speed modifier.
 MOB_SPEED ( 20 )
 -- The amount in logical pixels the mob is willing to travel away from its spawn origin point before
 --   returning. Prevents linear "kiting"
 MOB_LEASH ( 200 )
 -- The collision information
 MOB_COLLISION_BOX ( 4, 4, 16, 16 )
 -- "neutral" means the mobs only attack when the player attacks them first. "hostile" for aggressive mobs
 MOB_AGGRO_TYPE ( "neutral" )
 -- 25% chance to drop 1 shellfish. Arguments to LOOT_TABLE should be multiples of 3
 LOOT_TABLE ( "shellfish", 1, 25.0 )
-- Ends the mob entry and adds it to the list
END_MOB_TYPE()
-- 1 : shark ------------------------------------------------------------------
BEGIN_MOB_TYPE "shark"
 MOB_CLASS "basic_mob"
 LEVEL_RANGE(6, 8)
 DEFAULT_ANIMATION "lf1.png"
 FR_ANIM_TEXTURE_LIST (
  "lf1.png"
 )
 BK_ANIM_TEXTURE_LIST (
  "rt1.png"
 )
 RT_ANIM_TEXTURE_LIST (
  "rt1.png"
 )
 LF_ANIM_TEXTURE_LIST (
  "lf1.png"
 )
 ANIM_SPEED ( 0.1 )
 MOB_WIDTH ( 118 )
 MOB_HEIGHT ( 32 )
 MOB_SPEED ( 30 )
 MOB_LEASH ( 400 )
 MOB_COLLISION_BOX ( 4, 4, 112, 112 )
 MOB_AGGRO_TYPE ( "hostile" )
 MOB_AGGRO_RADIUS ( 320 ) -- only meaningful if hostile
 BIOME "water" -- "both" by default, other options are "land", restricts movement
END_MOB_TYPE()
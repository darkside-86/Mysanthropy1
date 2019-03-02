-- number of seconds required to get one harvest out of item
local defaultClickTime = 8.0
-- time values are in seconds so use these for conversion
local minutes = 60.0
local hours = 60.0 * minutes

-- 0 : palm1
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
-- 1 : palm2
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
-- 2 : palm3
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
-- 3 : palm4
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
-- 4 : shrub1
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
-- 5 : shrub2
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
-- 6 : grass1
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
-- 7 : coconut_palm
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
-- 8 : banana_tree
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
-- 9 : palm_with_vine1
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
-- 10 : palm_with_vine2
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
-- 11 : starfish
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
-- 12 : stone1
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
-- 13 : spring
BEGIN_ENTITY "spring"
 USE_TEXTURE "res/textures/objects/spring.png"
 WIDTH (64)
 HEIGHT (39)
 COLLISION_BOX (5, 9, 55, 26)
 MAX_CLICKS( -1 )
 CLICK_TIME(defaultClickTime)
 FARMABLE ( 100.0, 5, "water", 4 * hours, "res/textures/objects/spring_dry.png")
END_ENTITY ()
-- 14 : stone2
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
-- 15 : stone3
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

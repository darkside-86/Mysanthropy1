print("Loading island.lua")
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
 CLICK_TIME(15.0)
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
 CLICK_TIME(15.0)
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
 CLICK_TIME(15.0)
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
 CLICK_TIME(15.0)
END_ENTITY ()
-- 4 : shrub1
BEGIN_ENTITY "shrub1"
 USE_TEXTURE "res/textures/objects/shrub1.png"
 WIDTH (39)
 HEIGHT (32)
 COLLISION_BOX (-1, 0, 0, 0) -- no collision
 MAX_CLICKS (5)
 ON_INTERACT (
     100.0, 1, "grass",
     100.0, 1, "exp"
 )
 ON_DESTROY (
     100.0, 5, "grass",
     100.0, 5, "exp"
 )
 CLICK_TIME(0.5)
END_ENTITY()
-- 5 : shrub2
BEGIN_ENTITY "shrub2"
 USE_TEXTURE "res/textures/objects/shrub2.png"
 WIDTH (51)
 HEIGHT (32)
 COLLISION_BOX (-1, 0, 0, 0)
 MAX_CLICKS (25)
 ON_INTERACT (
     100.0, 1, "grass",
     100.0, 1, "exp"
 )
 ON_DESTROY (
     100.0, 10, "grass",
     100.0, 10, "exp"
 )
 CLICK_TIME(15.0)
END_ENTITY()

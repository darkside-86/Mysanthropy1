-- All items either crafted or required to craft should be in the inventory ---
-- database -------------------------------------------------------------------
-------------------------------------------------------------------------------

CRAFTABLE {
    name = "rope",
    yield = 1, -- optional, 1 is default for number of items created
    requires = {
        5, "grass",
        2, "vine"
    },
    time = 15, -- time required to craft the item in seconds.
    level = 1 -- required minimum level (optional, defaults to 1)
    -- optional fields such as building
    -- e.g. building = "campfire", or building = "workshop"
    -- to indicate need to be within a proximity to building (maybe buildingwidth * 1.1 radius?)
}
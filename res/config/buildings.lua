-- database of all buildings

BUILDING {
    name = "campfire",
    title = "Campfire",
    tooltip = "Allows you to cook delicious food",
    texture = "res/textures/buildings/campfire.png",
    hidden = false, -- optional, hidden is false by default
    width = 32,
    height = 32,
    required = {
        {"foodstuff", 25}, {"wood", 25}
    },
    collision = { 0, 0, 32, 32 },
    time = 15, -- N seconds to build
    level = 1
    -- TODO torndown fields for item drops when destroyed,
    -- harvestable... max, drops, etc. (e.g. a well with 100/100 clicks for water)
    -- farming... time, numtimes, leftover (e.g. fruit trees)
    -- craftable... time, yield, reqs, etc. (e.g. a smithy)
}

BUILDING {
    name = "well",
    title = "Well",
    tooltip = "Allows you to extract fresh water from underground",
    texture = "res/textures/buildings/well.png",
    width = 32,
    height = 45,
    required = {
        {"foodstuff", 100}, {"stone", 100}
    },
    collision = {0, 13, 32, 32},
    time = 30, -- time in seconds to build
    level = 3, -- min level required to build the structure
    harvesting = {
        maxClicks = 100, -- can be used to get water up to 100 times
        time = 5, -- 5 seconds to harvest
        drops = {
            {"water", 1, 100.0}, -- 100% chance to drop 1 water per harvest
            {"exp", 1, 100.0}
        },
        completed = "drywell" -- what the building turns into once fully harvested
    },
    removing = { -- when player selects "Tear down" from the building interface
        time = 25, -- how many seconds it takes to complete action
        drops = { -- 25% chance to get 25 stones--better to just extract the well than destroy it
            {"stone", 25, 25.0}
        }
    }   
}

BUILDING {
    name = "drywell",
    title = "Dried up well",
    tooltip = "This well has dried up and should be torn down",
    texture = "res/textures/buildings/drywell.png",
    hidden = true, -- can't select a dry well to build.
    width = 32,
    height = 45,
    required = {
        -- This should NEVER be directly built anyway
    },
    collision = {0, 13, 32, 32},
    time = 9999, -- player never builds these directly
    level = 3,
    harvesting = {
        maxClicks = 10,
        time = 3,
        drops = {
            {"stone", 1, 100.0},
            {"exp", 1, 100.0}
        },
    }
}
--[[
BUILDING {
    name = "farmland",
    title = "Farm land",
    tooltip = "Tilled ground for planting crops",
    texture = "farmland.png",
    width = 32,
    height = 32,
    required = {
        {"foodstuff", 500}
    },
    collision = { -1, 0, 0, 0},
    time = 60,
    level = 6,
    crafting = {
        {
            title = "Grass",
            sources = { {"grass_seed", 1} },
            time=4*60*60,
            pending = "grass_growing.png", -- drawn on top of 'building'
            completed = "grass_grown.png",  
            results = {
                {"grass", 4, 100.0},
                {"grass_seed", 1, 100.0},
                {"grass_seed", 2, 50.0}
            }
        },
        {
            title = "Batata",
            sources = { {"batata_seed", 1} },
            time = 6 * 60 * 60,
            pending = "batata_growing.png",
            completed = "batata_grown.png",
            results = {
                {"batata", 4, 100.0},
                {"batata_seed", 1, 100.0},
                {"batata_seed", 2, 50.0}
            }
        }
    }
}

BUILDING {
    name = "apple_tree",
    title = "Apple Tree",
    tooltip = "Provides a finite source of apples",
    width = 48,
    height = 96,
    texture = "res/textures/buildings/apple_tree.png"
    required = {
        {"foodstuff", 15000}, {"apple_tree_sapling", 1}
    },
    collision = {10, 38, 16, 96},
    time = 30,
    level = 10,
    harvesting = {
        maxClicks = 20,
        time = 5,
        drops = {
            {"wood", 1, 100.0}
        }
        -- no completed field means just disappear the "building" upon harvests  being spent.
        --  it would be dumb to chop down an active apple tree for wood and destroy it but it's 
        --  an option.
    },
    farming = {
        maxFarms = 10, -- how many times it can be farmed for apples. -1 for infinite times
        time = 10, -- how long it takes to grab apples from the tree
        frequency = 10 * 60 * 60, -- 10 hours between apples being available to pluck. interaction
                                  --  during this wait equate to a "harvest" action. When a building
                                  --  is first placed it is not ready to farm until this amount of
                                  --  time has passed.
        pending = "apple_tree_pending.png", -- texture to use when apples aren't ready yet
        drops = {
            {"apple", 4, 100.0}
        },
        completed = "dead_apple_tree" -- once farming is done 10 times it turns into a dead tree that
                                      --  that can be torn down for a small amount of wood. If this 
                                      --  field is left blank, the building disappears. (For infinite
                                      --  farming one should set maxFarms to -1)
    }
    -- no removing field, tree can be instantly destroyed but gives nothing if done so
}

BUILDING { -- probably better to use "hay_bale" for the name given implementation
    name = "grass_bale",
    title = "Grass bale",
    toolip = "Turns into hay",
    width = 42, height = 42,
    texture = "res/textures/buildings/hay_bale.png" -- notice the farm completion texture
    required = {
        {"rope", 10}, {"grass", 10}
    },
    collision = {0, 0, 42, 42},
    time = 4,
    level = 8,
    farming = {
        maxFarms = 1, -- a grass bale turns into exactly one hay bale
        time = 2, -- only 2 seconds to pick up the hay
        frequency = 6 * 60 * 60, -- 6 hours to turn into hay
        pending = "res/textures/buildings/grass_bale.png", -- notice use of pending texture
        drops = {
            {"hay", 5, 100.0} -- super itchy
        }
        -- no completed field--just disappear when picked up
    }
}
]]
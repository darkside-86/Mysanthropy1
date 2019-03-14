-- database of all buildings

BUILDING {
    name = "campfire",
    title = "Campfire",
    tooltip = "Allows you to cook delicious food",
    width = 32,
    height = 32,
    required = {
        {"foodstuff", 25}, {"wood", 25}
    },
    collision = { 0, 0, 32, 32 },
    time = 30, -- 30 seconds to build
    level = 1
    -- TODO torndown fields for item drops when destroyed,
    -- harvestable... max, drops, etc. (e.g. a well with 100/100 clicks for water)
    -- farming... time, numtimes, leftover (e.g. fruit trees)
    -- craftable... time, yield, reqs, etc. (e.g. a smithy)
}
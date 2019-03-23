STATUS_EFFECT {
    name = "bleed",
    title = "Bleed",
    tooltip = "Bleeding",
    icon = "res/textures/statuseffects/bleed.png",
    tickRate = 2, -- every two seconds
    offensive = true,
    group = "bleeds",
    groupUnique = false,
    harmful = true,
    maxStacks = 5,
    formula = "<(0.1,0.2)WPN+(1)LVL,Physical;",
    -- not a buff so no adds or mults field
    outputType = "Physical",
    dispel = false
}

STATUS_EFFECT {
    name = "blessfoo",
    title = "Blessing of Foo",
    tooltip = "Increases blah blah",
    icon = "res/textures/statuseffects/blessfoo.png",
    tickRate = 0, -- not dot/hot
    offensive = false,
    group = "enhT1",
    groupUnique = true,
    harmful = false,
    maxStacks = 1,
    -- no formula, see above
    adds = { {4,5}, "ARM", {1,2.5}, "MAP" }, -- adds 4 to 5 armor and 1 to 2.5 melee attack power
    mults = { {1.05,1.05}, "VIT" }, -- adds 5% vitality
    outputType = "Holy",
    dispel = true
}

STATUS_EFFECT {
    name = "wellfed_clambroth",
    title = "Well Fed",
    tooltip = "Increases vitality and blah blah",
    icon = "res/textures/statuseffects/wellfed.png",
    tickRate = 0,
    offensive = false,
    group = "wellfed",
    groupUnique = true,
    harmful = false,
    maxStacks = 1,
    adds = { {1,2}, "VIT", {1}, "STR"},
    -- no mults
    outputType = "Physical",
    dispel = false
}
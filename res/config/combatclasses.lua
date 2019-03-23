COMBAT_CLASS {
    name = "survivalist",
    title = "Survivalist",
    tooltip = "Survives by gathering natural resources and crafting",
    playable = true,
    strength = 1.085,
    agility = 1.09,
    intellect = 1.05,
    dexterity = 1.05,
    wisdom = 1.05,
    knowledge = 1.05,
    vitality = 1.075,
    abilities = { "unarmed_left", "unarmed_right", "sword_left", "sword_right" }
}

COMBAT_CLASS {
    name = "basic_mob",
    title = "Basic mob",
    tooltip = "...", -- shouldn't be shown in UI anyway
    playable = false,
    strength = 1.11,
    agility = 1.11,
    intellect = 1.1,
    dexterity = 1.075,
    wisdom = 1.075,
    knowledge = 1.075,
    vitality = 1.1,
    abilities = { "basic_attack" }
}
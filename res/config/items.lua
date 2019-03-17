-- Entries for all possible inventory items -----------------------------------
-------------------------------------------------------------------------------
-- Fields:
--  name : string -- required
--  title : string -- optional
--  tooltip : string -- optional
--  icon : string -- required
--  hidden : boolean -- optional
--  unique : boolean -- optional
--  foodstuff : integer -- optional
--  type : string -- optional*
-- If type is "consumable", consumable : table field is required
-- If type is "equipment", equipment : table field is required
-- If type is "food", food : table field is required


ITEM_ENTRY {
    name = "foodstuff",
    hidden = true, -- this is a special currency item
    icon = "res/textures/items/foodstuff.png"
}

ITEM_ENTRY {
    name = "wood",
    -- title is blank -- so it will default to "Wood"
    tooltip = "Used for construction, making planks, crafting traps, simple weapons, and more",
    icon = "res/textures/items/wood.png",
    -- foodstuff is blank -- can't eat wood
    -- type is blank -- this is just a simple crafting item
}

ITEM_ENTRY {
    name = "grass",
    -- title is blank -- so it will default to "Grass"
    tooltip = "Used for crafting ropes and more",
    icon = "res/textures/items/grass.png",
    -- foodstuff is blank -- we can't eat that!
    -- type is blank -- it's just for crafting
}

ITEM_ENTRY {
    name = "stone",
    -- title is blank -- will default to "Stone"
    tooltip = "Used for crafting structures such as wells and buildings and more. Can also be thrown at bad guys!",
    icon = "res/textures/items/stone.png",
    -- foodstuff is blank -- would break teeth if eaten
    -- type is blank -- although can be thrown at mobs otherwise has no special info
}

ITEM_ENTRY {
    name = "coconut",
    -- title is blank -- defaults to "Coconut"
    tooltip = "Can be stored as food or made into delicious recipes",
    icon = "res/textures/items/coconut.png",
    foodstuff = 5
    -- type is blank -- just a crafting item
}

ITEM_ENTRY {
    name = "banana",
    -- title is blank -- defaults to "Banana"
    tooltip = "Can be stored as food or made into delicious recipes",
    icon = "res/textures/items/banana.png",
    foodstuff = 5
    -- type is blank -- just for crafting
}

ITEM_ENTRY {
    name = "vine",
    -- title is blank -- defaults to "Vine"
    tooltip = "Needed for crafting ropes and other items",
    icon = "res/textures/items/vine.png",
    -- foodstuff is 0 -- can't eat vines obviously
    -- type is blank -- just a crafting item
}

ITEM_ENTRY {
    name = "shellfish",
    -- title is blank -- "Shellfish"
    tooltip = "Can be stored as food or made into delicious soups and broths",
    icon = "res/textures/items/shellfish.png",
    foodstuff = 5
    -- type is blank -- just a crafting item
}

ITEM_ENTRY {
    name = "water",
    -- title is blank -- defaults to "Water"
    tooltip = "Needed for cooking. You can also drink water to restore magic points",
    icon = "res/textures/items/water.png",
    -- foodstuff is 0 -- although it can be consumed it's not a source of calories or energy
    type = "food",
    food = {} -- no information to put here yet
}

ITEM_ENTRY {
    name = "rope",
    -- title is blank -- "Rope"
    tooltip = "Crafted from vines and grass. Used to craft traps, fishing nets and much more",
    icon = "res/textures/items/rope.png"
    -- foodstuff is 0 -- can't eat a rope
    -- type is blank -- just used for crafting other items
}

ITEM_ENTRY {
    name = "wooden_sword",
    title = "Wooden Sword",
    tooltip = "Poke things with a stick",
    icon = "res/textures/items/wooden_sword.png",
    -- foodstuff = 0 -- eating a sword is a terrible idea for most people
    type = "equipment",
    equipment = { durability = 5 } -- wooden swords aren't very durable
}

ITEM_ENTRY {
    name = "nail",
    tooltip = "Needed for constructing buildings",
    icon = "res/textures/items/nail.png"
}

ITEM_ENTRY {
    name = "ironingot",
    title = "Iron Ingot",
    tooltip = "Can be used for metal working",
    icon = "res/textures/items/ironingot.png"
}
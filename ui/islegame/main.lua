Include("ui/islegame/InventoryFrame.lua")

-- global singleton for all the UI components
UI = {
    inventory = InventoryFrame.New()
}

-- Easy to read and call "methods" of the singleton. 
function UI_Inventory_Toggle()
    print(UI.inventory)
    print(UI.inventory.SetVisible)
    UI.inventory:SetVisible(not UI.inventory:IsVisible())
end

function UI_Inventory_Setup()
    UI.inventory:Setup()
end

function _main()
    -- hide stuff until user shows
    -- UI.inventory:SetVisible(false)

end


_main()

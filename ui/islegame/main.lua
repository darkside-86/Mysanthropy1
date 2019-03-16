-- main.lua
-------------------------------------------------------------------------------
-- Author: darkside-86
-- (c) 2019
-------------------------------------------------------------------------------
-- This program is free software : you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
-- 
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.If not, see < https:--www.gnu.org/licenses/>.
-------------------------------------------------------------------------------

Include("ui/islegame/ActionBar.lua")
Include("ui/islegame/BuildingFrame.lua")
Include("ui/islegame/CastBar.lua")
Include("ui/islegame/Console.lua")
Include("ui/islegame/CraftingFrame.lua")
Include("ui/islegame/DataBar.lua")
Include("ui/islegame/ExperienceBar.lua")
Include("ui/islegame/InventoryFrame.lua")
Include("ui/islegame/UnitFrame.lua")

-- global singleton for all the UI components
UI = {
    -- unit frames and console and exp bar go at the bottom of z index
    dataBar = DataBar.New(nil, 24),
    console = Console.New(nil),
    playerUnitFrame = UnitFrame.New(nil),
    targetUnitFrame = UnitFrame.New(nil),
    actionBar = ActionBar.New(nil),
    expBar = ExperienceBar.New(nil),
    -- next are crafting windows
    buildingFrame = BuildingFrame.New(nil),
    crafting = CraftingFrame.New(nil),
    inventory = InventoryFrame.New(nil),
    -- casting bar stays at top
    castBar = CastBar.New(nil, GetScreenWidth() / 5, 16)
}

-- Action bar methods ---------------------------------------------------------
-------------------------------------------------------------------------------
function UI_ActionBar_SetLeftCDValue(value)
    UI.actionBar:SetLeftCDValue(value)
end

function UI_ActionBar_SetRightCDValue(value)
    UI.actionBar:SetRightCDValue(value)
end

-- Building frame methods -----------------------------------------------------
-------------------------------------------------------------------------------
function UI_BuildingFrame_Toggle()
    UI.buildingFrame:SetVisible(not UI.buildingFrame:IsVisible())
end

-- Cast bar methods -----------------------------------------------------------
-------------------------------------------------------------------------------
function UI_CastBar_SetActivity(act)
    UI.castBar:SetActivity(act)
end

function UI_CastBar_SetFillColor(r, g, b, a)
    UI.castBar:SetFillColor(r, g, b, a)
end

function UI_CastBar_SetValue(value)
    UI.castBar:SetValue(value)
end

function UI_CastBar_SetVisible(vis)
    UI.castBar:SetVisible(vis)
end

-- Console methods ------------------------------------------------------------
-------------------------------------------------------------------------------
function UI_Console_WriteLine(text, r, g, b, a)
    UI.console:WriteLine(text, r, g, b, a)
end

-- Crafting methods -----------------------------------------------------------
-------------------------------------------------------------------------------
function UI_Crafting_Toggle()
    UI.crafting:SetVisible(not UI.crafting:IsVisible())
end

-- Experience bar methods -----------------------------------------------------
-------------------------------------------------------------------------------
function UI_ExperienceBar_SetValue(v)
    UI.expBar:SetValue(v)
end

-- Inventory methods ----------------------------------------------------------
-------------------------------------------------------------------------------
function UI_Inventory_Toggle()
    print(UI.inventory)
    print(UI.inventory.SetVisible)
    UI.inventory:SetVisible(not UI.inventory:IsVisible())
end

function UI_Inventory_Setup()
    UI.inventory:Setup()
    UI.dataBar:SetFoodstuff()
end

-- Unit frame methods ---------------------------------------------------------
-------------------------------------------------------------------------------
function UI_UnitFrame_SetNameAndLevel(name, level, hostility, isPlayer)
    isPlayer = isPlayer or false
    if isPlayer then 
        UI.playerUnitFrame:SetNameAndLevel(name, level, "player")
    else
        UI.targetUnitFrame:SetNameAndLevel(name, level, hostility)
    end
end

function UI_UnitFrame_SetHealth(ch, mh, isPlayer)
    isPlayer = isPlayer or false 
    if isPlayer then
        UI.playerUnitFrame:SetHealth(ch, mh)
    else 
        UI.targetUnitFrame:SetHealth(ch, mh)
    end
end

function UI_UnitFrame_SetVisible(vis, isPlayer)
    isPlayer = isPlayer or false
    if isPlayer then
        UI.playerUnitFrame:SetVisible(vis)
    else 
        UI.targetUnitFrame:SetVisible(vis)
    end
end

-- main -----------------------------------------------------------------------

local function _main()
    -- get screen width and height for configuration of various frames
    local screenWidth = GetScreenWidth()
    local screenHeight = GetScreenHeight()
    
    -- hide most components until user shows
    UI.inventory:SetVisible(false)
    UI.castBar:SetVisible(false)
    UI.crafting:SetVisible(false)
    UI.targetUnitFrame:SetVisible(false)
    UI.buildingFrame:SetVisible(false)
    -- Show foodstuff amount now that inventory component is loaded
    UI.dataBar:SetFoodstuff()
    -- move console to above data bar
    UI.console:SetYPos(UI.console:GetYPos() - UI.dataBar:GetHeight())
    -- position unit frames. 
    UI.playerUnitFrame:SetXPos(5)
    UI.playerUnitFrame:SetYPos(5)
    UI.targetUnitFrame:SetXPos(UI.playerUnitFrame:GetXPos() + UI.playerUnitFrame:GetWidth() + 5)
    UI.targetUnitFrame:SetYPos(UI.playerUnitFrame:GetYPos())
    -- position experience bar
    UI.expBar:SetYPos(UI.dataBar:GetYPos() - UI.expBar:GetHeight() - 5)
    -- position action bar
    UI.actionBar:SetXPos(UI.console:GetXPos() + UI.console:GetWidth() + 5)
    UI.actionBar:SetYPos(UI.expBar:GetYPos() - UI.actionBar:GetHeight() - 5)

    -- Loading welcome message
    UI.console:WriteLine("Welcome to Despacito Island UI 0.1", 0, 1, 1, 1)
end

_main()

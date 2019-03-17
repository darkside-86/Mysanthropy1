-- CraftingFrame.lua
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
Include("ui/islegame/Fonts.lua")
Include("ui/islegame/GameButton.lua")
Include("ui/islegame/GameFrame.lua")
Include("ui/islegame/ItemTable.lua")
Include("ui/islegame/Textures.lua")

CraftingFrame = {
    New = function(parent)
        local frame = GameFrame.New(parent, GetScreenWidth() / 2, GetScreenHeight() / 2,
                "Crafting", true)
        -- get database
        local craftables = Game_Crafting_GetCraftables();
        -- build a table by name
        frame.database = {}
        for i, each in ipairs(craftables) do
            frame.database[each.name] = each
        end
        frame:SetColor(1,1,1,0.75)
        -- center on screen
        frame:SetXPos(GetScreenWidth() / 4)
        frame:SetYPos(GetScreenHeight() / 4)
        local GRID_SIZE = 48 -- 48x48 icons
        -- create formula information panel
        frame.content.info = UIFrame.New(frame.content, frame:GetWidth()-2, 
            GRID_SIZE * 2 - 2, 1, 1, TEXTURE_UIBLANK)
        frame.content.info:SetColor(0,0,0,0)
        frame.content.info:SetBorderSize(1)
        frame.content.info:SetBorderColor(0,0,0,1)
        -- create crafting button
        frame.content.info.craftBtn = GameButton.New(frame.content.info, "Craft", 7, false)
        frame.content.info.craftBtn:SetYPos(frame.content.info:GetHeight() -
                frame.content.info.craftBtn:GetHeight())
        frame.content.info.craftBtn:SetXPos(frame.content.info:GetWidth() - 
                frame.content.info.craftBtn:GetWidth())
        frame.content.info.craftBtn:AddOnClicked(function() 
            -- first make sure there is a selection
            if frame.content.info.selected == nil then 
                print("Warning: nothing selected to craft")
                return 
            end
            -- next make sure there are enough required items in inventory
            local itemz = Game_Inventory_GetItems()
            -- (turn item list into an assoc-array)
            local items = {}
            for i, ie in ipairs(itemz) do
                items[ie.name] = ie
            end
            for i, req in ipairs(frame.content.info.selected.required) do 
                for k,v in pairs(req) do 
                    print(k,v)
                end
                if items[req.item] == nil 
                  or items[req.item].count == nil 
                  or items[req.item].count < req.count 
                then 
                    print("You don't have enough " .. req.item .. " to craft this!")
                    return
                end
            end
            -- make sure level requirement is met.
            if frame.content.info.selected.level > Game_Player_GetLevel() then 
                print("You aren't high enough level to craft this yet")
                return
            end
            -- everything passes so tell game to start crafting sequence
            Game_StartCrafting(frame.content.info.selected.name)
        end)
        -- create craftable list grid
        frame.content.craftables = UIFrame.New(frame.content, frame:GetWidth()-2,
            frame.content:GetHeight() - frame.content.info:GetHeight(), 1, 1, TEXTURE_UIBLANK)
        frame.content.craftables:SetYPos(frame.content.info:GetYPos() + frame.content.info:GetHeight())
        frame.content.craftables:SetColor(0, 0, 0, 0)
        frame.content.craftables:SetBorderSize(1)
        frame.content.craftables:SetBorderColor(0,0,0,1)
        frame.content.craftables.list = {}
        local xCounter = 0
        local yCounter = 0
        for i, each in ipairs(craftables) do
            local tile = UIFrame.New(frame.content.craftables, GRID_SIZE, GRID_SIZE, xCounter,
                    yCounter, TEXTURE_UIBLANK)
            tile:SetColor(0,0,0,0)
            tile:SetBorderSize(1)
            tile:SetBorderColor(0,0,0,1)
            tile.data = each 
            tile.icon = UITexture.New(tile, itemTable[each.name].icon, GRID_SIZE, GRID_SIZE)
            tile.yieldLabel = UILabel.New(tile, each.yield, "mono12", 0,0,0,1)
            -- when a tile is clicked, fill in the info panel with info and select the data
            --  so that the craft button knows what to craft
            tile:AddOnClicked(function() 

                frame.content.info.selected = tile.data 
                -- first the yield
                frame.content.info.yieldLbl = UILabel.New(frame.content.info, 
                    tile.data.yield, GAMEFONT, 0, 0, 0, 1)
                -- anchored to far left and centered vertically 1/4
                frame.content.info.yieldLbl:SetYPos(frame.content.info:GetHeight() / 4 -
                    frame.content.info.yieldLbl:GetHeight() / 2)
                -- the icon goes next to the yield label
                frame.content.info.resultIcon = UITexture.New(frame.content.info, 
                    itemTable[tile.data.name].icon, GRID_SIZE, GRID_SIZE)
                -- anchored to the right of yield label and centered vertically 1/4
                frame.content.info.resultIcon:SetXPos(frame.content.info.yieldLbl:GetWidth() +
                    frame.content.info.yieldLbl:GetXPos())
                frame.content.info.resultIcon:SetYPos(frame.content.info:GetHeight() / 4 
                    - frame.content.info.resultIcon:GetHeight() / 2)
                -- equal sign next, anchored to left of icon and centered vertically 1/4
                frame.content.info.equalLbl = UILabel.New(frame.content.info, "=", GAMEFONT, 0,0,0,1)
                frame.content.info.equalLbl:SetXPos(frame.content.info.resultIcon:GetXPos() 
                    + frame.content.info.resultIcon:GetWidth())
                frame.content.info.equalLbl:SetYPos(frame.content.info:GetHeight() / 4
                    - frame.content.info.equalLbl:GetHeight() / 2)
                -- now create of elements indicating yield, icon, and a plus sign between each one
                local reqx = frame.content.info.equalLbl:GetXPos() + frame.content.info.equalLbl:GetWidth()
                frame.content.info.formula = {}
                for i, v in ipairs(tile.data.required) do 
                    frame.content.info.formula[i] = {}
                    frame.content.info.formula[i].reqCountLbl = UILabel.New(frame.content.info, 
                        v.count, GAMEFONT, 0, 0, 0, 1)
                    frame.content.info.formula[i].reqCountLbl:SetXPos(reqx)
                    frame.content.info.formula[i].reqCountLbl:SetYPos(frame.content.info:GetHeight() / 4 
                            - frame.content.info.formula[i].reqCountLbl:GetHeight() / 2)
                    reqx = reqx + frame.content.info.formula[i].reqCountLbl:GetWidth()
                    frame.content.info.formula[i].reqCountIcon = UITexture.New(frame.content.info, 
                        itemTable[v.item].icon, GRID_SIZE, GRID_SIZE)
                    frame.content.info.formula[i].reqCountIcon:SetXPos(reqx)
                    frame.content.info.formula[i].reqCountIcon:SetYPos(frame.content.info:GetHeight() / 4
                            - frame.content.info.formula[i].reqCountIcon:GetHeight() / 2)
                    reqx = reqx + frame.content.info.formula[i].reqCountIcon:GetWidth()
                    -- add + only if it's not the last item in the list
                    if i < #tile.data.required then 
                        frame.content.info.formula[i].plus = UILabel.New(frame.content.info, 
                            "+", GAMEFONT, 0,0,0,1)
                        frame.content.info.formula[i].plus:SetXPos(reqx)
                        frame.content.info.formula[i].plus:SetYPos(frame.content.info:GetHeight() / 4
                                - frame.content.info.formula[i].plus:GetHeight() / 2)
                        reqx = reqx + frame.content.info.formula[i].plus:GetWidth()
                    end
                    -- discard old UI elements without having to hide them manually
                    collectgarbage("collect")
                end

            end)
            frame.content.craftables.list[i] = tile
            if xCounter - GRID_SIZE > frame.content.craftables:GetWidth() then 
                xCounter = 0
                yCounter = yCounter + GRID_SIZE
            end
        end
        return frame
    end
}

-- InventoryFrame.lua
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

Include("ui/islegame/GameButton.lua")
Include("ui/islegame/GameFrame.lua")
Include("ui/islegame/ItemTable.lua")

-- defines a subclass of GameFrame that displays inventory
InventoryFrame = {
    New = function() 
        local screenWidth = GetScreenWidth()
        local screenHeight = GetScreenHeight()
        -- create the GameFrame
        local frame = GameFrame.New(nil, screenWidth / 2.5, screenHeight / 2.5, "Inventory", true)
        frame:SetXPos(screenWidth - frame:GetWidth())
        frame:SetYPos(screenHeight - frame:GetHeight())
        -- create the button
        frame.content.foodBtn = GameButton.New(frame.content, "Store food", 5, true)
        frame.content.foodBtn:SetXPos(2)
        frame.content.foodBtn:SetYPos(2)
        -- define Setup function
        local function Setup(this)
            -- get the inventory list 
            local items = Game_Inventory_GetItems()
            local xCounter = 2
            local yCounter = this.content.foodBtn:GetHeight() + this.content.foodBtn:GetYPos() + 2
            local tileWidth = 48
            local tileHeight = 48
            -- populate tiles of items with numbers in top left corner
            this.content.elements = {}
            collectgarbage("collect")
            for i, v in ipairs(items) do 
                -- ignore hidden items
                if not itemTable[v.name].hidden then 
                    local elem = UIFrame.New(this.content, tileWidth, tileHeight, xCounter, 
                            yCounter, TEXTURE_UIBLANK)
                    elem.data = v
                    elem:SetColor(0,0,0,0)
                    elem:SetBorderSize(1)
                    elem:SetBorderColor(0,0,0,1)
                    elem.icon = UITexture.New(elem, itemTable[elem.data.name].icon, tileWidth, tileHeight)
                    elem.countLbl = UILabel.New(elem.icon, tostring(elem.data.count), GAMEFONT, 0, 0, 0, 1)
                    local function elemOnClicked(x,y)
                        local capture = elem
                        -- The meaning of a click depends on what button if any is selected
                        print(x,y)
                        if this.content.foodBtn.selected then 
                            print(capture.data.name)
                            if capture.data.count >= 1 and itemTable[capture.data.name].foodstuff > 0 then
                                local success = Game_Inventory_ConvertItemToFoodstuff(
                                        capture.data.name, 1)
                                if success then
                                    capture.data.count = capture.data.count - 1
                                    capture.countLbl:SetText(tostring(capture.data.count))
                                end
                            end
                        end
                    end
                    elem:AddOnClicked(elemOnClicked)
                    --TODO: handle durability field
                    table.insert( this.content.elements, elem )
                    xCounter = xCounter + tileWidth
                    if xCounter > this:GetWidth() - tileWidth then 
                        xCounter = 2
                        yCounter = yCounter + tileHeight
                    end
                end
            end
        end
        frame.Setup = Setup
        Setup(frame)
        return frame
    end
}
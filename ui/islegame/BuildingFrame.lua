-- BuildingFrame.lua
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

Include("ui/islegame/BuildTable.lua")
Include("ui/islegame/Fonts.lua")
Include("ui/islegame/GameButton.lua")
Include("ui/islegame/GameFrame.lua")
Include("ui/islegame/ItemTable.lua")
Include("ui/islegame/Textures.lua")

-- defines an interface for selecting buildings and then telling game to attempt to place and build them
BuildingFrame = {
    New = function(parent)
        local frame = GameFrame.New(parent, GetScreenHeight() / 1.5, GetScreenHeight() / 1.5, "Buildings", true)
        -- top center
        frame:SetXPos(GetScreenWidth() / 2 - frame:GetWidth() / 2)
        frame:SetYPos(0) -- just for clarity
        local GRID_SIZE = 64
        frame.content.infoPanel = UIFrame.New(frame.content, frame.content:GetWidth()-1, GRID_SIZE-1, 
                1, 1, TEXTURE_UIBLANK)
        frame.content.infoPanel:SetColor(0,0,0,0)
        frame.content.infoPanel:SetBorderSize(1)
        frame.content.infoPanel:SetBorderColor(0,0,0,1)
        frame.content.infoPanel.buildBtn = GameButton.New(frame.content.infoPanel, "Build", 6, false)
        frame.content.infoPanel.buildBtn:SetXPos(frame.content.infoPanel:GetWidth()
                - frame.content.infoPanel.buildBtn:GetWidth())
        frame.content.infoPanel.buildBtn:SetYPos(frame.content.infoPanel:GetHeight() 
                - frame.content.infoPanel.buildBtn:GetHeight())
        frame.content.infoPanel.buildBtn:AddOnClicked(function() 
            -- validate building 
            if frame.selectedBuildName == nil  or buildTable[frame.selectedBuildName] == nil 
                    or buildTable[frame.selectedBuildName].hidden then 
                print("Nothing valid selected for building") -- todo: error messages on window
                return 
            end
            -- validate requirements
            local itemz = Game_Inventory_GetItems()
            local items = {}
            for i, eachItem in ipairs(itemz) do 
                items[eachItem.name] = eachItem 
            end
            for i, eachReq in ipairs(buildTable[frame.selectedBuildName].required) do 
                if items[eachReq.name].count < eachReq.count then 
                    print("You don't have enough materials to build this")
                    return
                end
            end
            if buildTable[frame.selectedBuildName].level > Game_Player_GetLevel() then 
                print("You aren't high enough level to build this yet")
                return
            end
            Game_StartBuilding(frame.selectedBuildName)
            -- hide the building frame so user can see where to build
            frame:SetVisible(false)
        end)

        frame.content.listPanel = UIFrame.New(frame.content, frame.content:GetWidth()-1,
                frame.content:GetHeight() - frame.content.infoPanel:GetHeight(), 1, 0, TEXTURE_UIBLANK)
        frame.content.listPanel:SetYPos(frame.content.infoPanel:GetHeight())
        frame.content.listPanel:SetColor(0,0,0,0)
        frame.content.listPanel:SetBorderSize(1)
        frame.content.listPanel:SetBorderColor(0,0,0,1)
        -- populate list panel
        local xCounter = 0
        local yCounter = 0
        frame.content.listPanel.elements = {}
        -- todo: build array out of database sorted by level and then title
        for k,v in pairs(buildTable) do 
            if v.hidden == false then 
                local bf = UIFrame.New(frame.content.listPanel, GRID_SIZE, GRID_SIZE, xCounter, yCounter, 
                    TEXTURE_UIBLANK)
                bf:SetColor(0,0,0,0)
                bf:SetBorderSize(1)
                bf:SetBorderColor(0,0,0,1)
                bf.icon = UITexture.New(bf, v.texture, GRID_SIZE, GRID_SIZE)
                bf:AddOnClicked(function() 
                    -- fill the information panel with building requirements
                    frame.content.infoPanel.elements = {}
                    collectgarbage("collect")
                    local x = 0 
                    for reqI, reqV in ipairs(v.required) do 
                        local reqItem = UITexture.New(frame.content.infoPanel, itemTable[reqV.name].icon, 
                                GRID_SIZE, GRID_SIZE)
                        reqItem:SetXPos(x)
                        reqItem.countLbl = UILabel.New(reqItem, tostring(reqV.count), "sans14", 0,0,0,1)
                        table.insert(frame.content.infoPanel.elements, reqItem)
                        x = x + GRID_SIZE
                    end
                    -- select this specific building
                    frame.selectedBuildName = v.name
                end)
                table.insert(frame.content.listPanel.elements, bf)
                xCounter = xCounter + GRID_SIZE 
                if xCounter - GRID_SIZE > frame.content.listPanel:GetWidth() then 
                    xCounter = 0
                    yCounter = yCounter + GRID_SIZE
                end
            end
        end
        return frame
    end
}
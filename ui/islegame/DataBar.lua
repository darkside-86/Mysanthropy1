-- DataBar.lua
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

-- defines a bar that extends across bottom of screen. For now just displays foodstuff currency
-- TODO: display UI button shortcuts

DataBar = {
    New = function(parent, height)
        local frame = UIFrame.New(parent, GetScreenWidth(), height, 0, 0, TEXTURE_UIBLANK)
        frame:SetYPos(GetScreenHeight() - frame:GetHeight())
        frame:SetColor(0.15, 0.15, 0.15, 1)
        local iconPath = itemTable["foodstuff"].icon 
        frame.fsIcon = UITexture.New(frame, iconPath, height, height)
        frame.fsLabel = UILabel.New(frame, "Foodstuff: ", "sans14", 1, 1, 1, 1)
        frame.fsLabel:SetXPos(frame.fsIcon:GetWidth())
        frame.fsAmountLbl = UILabel.New(frame, "0", "sans14", 1, 1, 0, 1)
        frame.fsAmountLbl:SetXPos(frame.fsLabel:GetXPos() + frame.fsLabel:GetWidth())
        frame.SetFoodstuff = DataBar.SetFoodstuff
        return frame
    end,

    SetFoodstuff = function(self)
        local fsItem = Game_Inventory_GetItems("foodstuff")
        -- supposed to be exactly one foodstuff entry or none
        if fsItem[1] == nil then 
            return
        end
        self.fsAmountLbl:SetText(tostring(fsItem[1].count))
    end
}
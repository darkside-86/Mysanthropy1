-- UnitFrame.lua
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
Include("ui/islegame/Textures.lua")

-- For now copy and paste old code until we can design something more eloquent
UnitFrame = {
    New = function(parent)
        local frame = UIFrame.New(parent, 175, 75, 0, 0, TEXTURE_WOOD)
        frame:SetColor(0.25, 0.25, 0.25, 1)
        frame.nameAndLevelLbl = UILabel.New(frame, "???, (Level ?)", GAMEFONT, 1, 1, 1, 1)
        frame.healthBar = UIFrame.New(frame, frame:GetWidth(), 20, 0, 0, TEXTURE_UIBLANK)
        frame.healthBar:SetYPos(frame.nameAndLevelLbl:GetHeight())
        frame.healthBar:SetColor(0.5, 0.5, 0.5, 1) -- valid color set by filling in data
        frame.healthBar.infoLbl = UILabel.New(frame.healthBar, "HP ? / ?", "sans14", 1, 1, 1, 1)
        -- todo: handle resources
        frame.SetNameAndLevel = UnitFrame.SetNameAndLevel
        frame.SetHealth = UnitFrame.SetHealth
        return frame
    end,

    SetNameAndLevel = function(self, name, level, hostility)
        self.nameAndLevelLbl:SetText(tostring(name) .. ", (Level " .. tostring(level) .. ")")
        if hostility == "neutral" then 
            self.healthBar:SetColor(0.7, 0.7, 0, 1)
        elseif hostility == "hostile" then 
            self.healthBar:SetColor(0.75, 0, 0, 1)
        else 
            self.healthBar:SetColor(0, 0.75, 0, 1)
        end
    end,

    SetHealth = function(self, currentHealth, maxHealth)
        self.healthBar:SetWidth(self:GetWidth() * (currentHealth/maxHealth))
        self.healthBar.infoLbl:SetText("HP " .. tostring(currentHealth) .. " / " .. tostring(maxHealth))
    end
}
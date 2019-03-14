-- ActionBar.lua
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

-- manages spell ability keybinds and GCD display as well as cooldown display
-- for left and right weapon attacks
ActionBar = {
    New = function(parent)
        local NUM_SLOTS = 12
        local SLOT_SIZE = 32 -- 32x32 for ability icons
        local WEAPON_CD_WIDTH = 16
        local frame = UIFrame.New(parent, NUM_SLOTS * SLOT_SIZE + 2 * WEAPON_CD_WIDTH, SLOT_SIZE, 
                0, 0, TEXTURE_UIBLANK)
        frame:SetColor(0.5, 0.5, 0.5, 0.2)
        -- let _main function position this
        -- create left side CD indicator and place it on the far left
        frame.leftCD = UIFrame.New(frame, WEAPON_CD_WIDTH, frame:GetHeight(), 0, 0, TEXTURE_UIBLANK)
        frame.leftCD:SetColor(0.05, 0.05, 0.05, 0.35)
        frame.leftCD.fill = UIFrame.New(frame.leftCD, frame.leftCD:GetWidth(), frame.leftCD:GetHeight(),
                0, 0, TEXTURE_UIBLANK)
        frame.leftCD.fill:SetColor(0.8, 0.8, 0.1, 0.55)
        frame.rightCD = UIFrame.New(frame, WEAPON_CD_WIDTH, frame:GetHeight(), 0, 0, TEXTURE_UIBLANK)
        frame.rightCD:SetColor(0.05, 0.05, 0.05, 0.35)
        frame.rightCD:SetXPos(frame:GetWidth() - frame.rightCD:GetWidth())
        frame.rightCD.fill = UIFrame.New(frame.rightCD, frame.rightCD:GetWidth(), frame.rightCD:GetHeight(),
                0, 0, TEXTURE_UIBLANK)
        frame.rightCD.fill:SetColor(0.8, 0.8, 0.1, 0.55)
        -- TODO: actionbar buttons, process keybinds, GCD for onGCD abilities, etc.
        frame.SetLeftCDValue = ActionBar.SetLeftCDValue
        frame.SetRightCDValue = ActionBar.SetRightCDValue
        return frame
    end,

    SetLeftCDValue = function(self, value)
        self.leftCD.fill:SetHeight(self.leftCD:GetHeight() * value)
        self.leftCD.fill:SetYPos(self.leftCD:GetHeight() - self.leftCD.fill:GetHeight())
    end,

    SetRightCDValue = function(self, value)
        self.rightCD.fill:SetHeight(self.rightCD:GetHeight() * value)
        self.rightCD.fill:SetYPos(self.rightCD:GetHeight() - self.rightCD.fill:GetHeight())
    end
}
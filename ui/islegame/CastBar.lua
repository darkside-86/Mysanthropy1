-- CastBar.lua
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

-- Defines a simple cast bar
CastBar = {
    New = function(parent, w, h)
        -- the backdrop primary frame
        local frame = UIFrame.New(parent, w, h, 0, 0, TEXTURE_UIBLANK)
        frame:SetColor(0.2, 0.2, 0.2, 0.5)
        frame.fill = UIFrame.New(frame, 0, h, 0, 0, TEXTURE_UIBLANK)
        frame.fill:SetColor(0.2, 0.8, 0.2, 0.5)
        frame.activity = UILabel.New(frame, "", "mono12", 1, 1, 1, 1)
        -- auto center at bottom center screen slightly above bottom
        frame:SetXPos(GetScreenWidth() / 2 - frame:GetWidth() / 2)
        frame:SetYPos(4 * GetScreenHeight() / 5 )
        -- Fill in methods
        frame.SetActivity = CastBar.SetActivity
        frame.SetValue = CastBar.SetValue
        frame.SetFillColor = CastBar.SetFillColor
        return frame
    end,

    SetActivity = function(self, act)
        self.activity:SetText(act)
    end,

    SetFillColor = function(self, r, g, b, a)
        self.fill:SetColor(r, g, b, a)
    end,

    SetValue = function(self, value)
        self.fill:SetWidth(value * self:GetWidth())
    end,
}

-- ExperienceBar.lua
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

ExperienceBar = {
    New = function(parent)
        local expbar = UIFrame.New(parent, GetScreenWidth() / 2, 15, 0, 0, TEXTURE_UIBLANK)
        expbar:SetXPos(GetScreenWidth() / 4)
        expbar:SetYPos(GetScreenHeight() - 50)
        expbar:SetColor(0.2, 0.2, 0.2, 0.4)
        expbar.fill = UIFrame.New(expbar, 0, expbar:GetHeight(), 0, 0, TEXTURE_UIBLANK)
        expbar.fill:SetColor(0.5, 0, 0.5, 0.7)
        expbar.segments = {}
        local NUM_SEGMENTS = 10
        for i = 1, NUM_SEGMENTS do 
            local x = (i - 1) * (expbar:GetWidth() / NUM_SEGMENTS)
            expbar.segments[i] = UIFrame.New(expbar, expbar:GetWidth() / NUM_SEGMENTS, expbar:GetHeight(),
                x, 0, TEXTURE_UIBLANK)
            expbar.segments[i]:SetColor(0,0,0,0)
            expbar.segments[i]:SetBorderSize(1)
            expbar.segments[i]:SetBorderColor(1, 1, 1, 0.7)
        end
        expbar.SetValue = ExperienceBar.SetValue
        return expbar
    end,

    SetValue = function(self, value)
        self.fill:SetWidth(value * self:GetWidth())
    end
}
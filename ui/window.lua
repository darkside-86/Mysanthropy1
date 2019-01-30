-- window.lua
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
-- along with this program.If not, see < https://www.gnu.org/licenses/>.
-------------------------------------------------------------------------------

-- Implements a Window class built from UI system --

if Window ~= nil then
    print("Window already loaded")
    return
end

LoadTexture('uiblank', 'res/textures/uiblank.png')
LoadTexture('uiwindow', 'res/textures/uiwindow.png')

Window = {
    New = function(parent, width, height, title, font)
        font = font or 'sans'
        local window = UIFrame.New(nil, width, height, 0, 0, 'uiblank')
        window:SetColor(0,0,0.5,0.6)
        window:SetBorderSize(2)
        window:SetBorderColor(0,0,0.9,1)
        window.title = UILabel.New(window, title, font, 1,1,1,1)
        window.closeBtn = UIButton.New(window, 'uiblank', 'X', font, 3)
        window.closeBtn:SetColor(0.5,0,0,1)
        window.closeBtn:SetTextColor(1,1,1,1)
        window.closeBtn:SetBorderSize(1)
        window.closeBtn:SetBorderColor(1,0,0,1)
        window.closeBtn:SetXPos(window:GetWidth() - window.closeBtn:GetWidth() - 2)
        window.closeBtn:SetYPos(2)
        window.closeBtn:AddOnClicked(function()
            window:SetVisible(false)
        end)
        window:AddOnDragged(function(x,y,dx,dy) 
            window:SetXPos(window:GetXPos() + dx)
            window:SetYPos(window:GetYPos() + dy)
        end)
        window.Resize = Window.Resize
        return window
    end,

    Resize = function(window, width, height)
        window:SetWidth(width)
        window:SetHeight(height)
        window.closeBtn:SetXPos(window:GetWidth() - window.closeBtn:GetWidth() - 2)
    end
}

-- GameFrame.lua
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

-- Defines a subclass of UIFrame that allows for moving and optional closing
GameFrame = {
    New = function(parent, width, height, title, closeable)
        -- the parameters describe inner panel size so account for surrounding elements
        local font = GAMEFONT 
        local texture = TEXTURE_PAPER
        local titleHeight = 27
        -- create the larger frame object as a container
        local frame = UIFrame.New(parent, 
                width, 
                height + titleHeight, 
                0, 0, texture)
        frame:SetColor(1,1,1,1) -- the full frame provides the textured background
        -- create the title bar
        frame.titleBar = UIFrame.New(frame, frame:GetWidth(), titleHeight, 0, 0, texture)
        frame.titleBar:SetColor(1,1,1,1)
        frame.titleBar.label = UILabel.New(frame.titleBar, title, font, 0, 0, 0, 1)
        frame.titleBar:AddOnDragged(function(x,y,dx,dy) 
            frame:SetXPos(frame:GetXPos() + dx)
            frame:SetYPos(frame:GetYPos() + dy)
        end)
        -- create a close button in the top right corner if closeable
        if closeable then 
            frame.closeBtn = UIFrame.New(frame, 21, 21, width-24, 4, TEXTURE_WOOD)
            frame.closeBtn:AddOnClicked(function() 
                frame:SetVisible(false)
            end)
        end
        -- create the inner content panel as a container. added for convenience so caller doesn't
        --  have to worry about dancing around Title and close button elements
        frame.content = UIFrame.New(frame, width, height, 0, titleHeight, texture)
        frame.content:SetColor(0,0,0,0)
        -- return the frame so that we can toggle the visibility of the window
        return frame
    end,

}
-- GameButton.lua
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

-- Implements a nicer button than stock UIButton (TODO: Subclass uiframe to use fancy borders)
GameButton = {
    New = function(parent, text, padding, selectable)
        selectable = selectable or false
        local font = GAMEFONT
        local button = UIButton.New(parent, TEXTURE_UIBLANK, text, font, padding)
        button:SetBorderSize(1)
        button:SetBorderColor(0,0,0,1)
        button:SetTextColor(1,1,1,1)
        button.selected = false
        button.selectable = selectable
        button.hoverColor = { 1, 1, 1, 0.1 }
        button.offColor = { 0, 0, 0, 0.1 }
        button.selectColor = { 0.5, 0, 0, 0.2 }
        button:SetColor(table.unpack(button.offColor))
        button:AddOnHover(function(x,y,dx,dy,over)
            if not button.selected then 
                if over then 
                    button:SetColor(table.unpack(button.hoverColor))
                else
                    button:SetColor(table.unpack(button.offColor)) 
                end
            end
        end)
        button:AddOnClicked(function(x,y) 
            if button.selectable then 
                if not button.selected then 
                    button.selected = true
                    button:SetColor(table.unpack(button.selectColor))
                else 
                    button.selected = false
                    button:SetColor(table.unpack(button.offColor))
                end
            end
        end)  
        return button
    end
}
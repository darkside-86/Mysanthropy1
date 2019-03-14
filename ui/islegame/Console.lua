-- Console.lua
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

local FONT = "mono12"

local function StrSplit(str, index)
    local ar = {}

end

local function StrToArray(str, pxWidth, fontToTest)
    
end

-- implements an ingame console with printing and eventually input
Console = {
    New = function(parent)
        local frame = UIFrame.New(nil, GetScreenWidth()/3, GetScreenHeight()/4, 0, 0, TEXTURE_UIBLANK)
        frame:SetColor(0.0, 0.0, 0.1, 0.5)
        frame.lines = {}
        frame.MAX_LINES = frame:GetHeight() / 13
        frame:SetHeight(frame:GetHeight() + 13)
        frame:SetYPos(GetScreenHeight() - frame:GetHeight())
        frame.WriteLine = Console.WriteLine
        return frame
    end,

    WriteLine = function(self, text, r, g, b, a)
        r = r or 1
        g = g or 1
        b = b or 1
        a = a or 1
        if text == "" then return end
        print("[CONSOLE] " .. text)
        local line = UILabel.New(self, text, FONT, r, g, b, a)
        if #self.lines > self.MAX_LINES then 
            -- discard the first line
            table.remove(self.lines, 1)
            collectgarbage("collect")
            -- move lines up
            for ind, eachLine in ipairs(self.lines) do 
                eachLine:SetYPos(eachLine:GetYPos() - eachLine:GetHeight())
            end
            line:SetYPos(self.lines[#self.lines]:GetYPos() + self.lines[#self.lines]:GetHeight())
            table.insert(self.lines, line)
        else
            if #self.lines > 1 then 
                line:SetYPos(self.lines[#self.lines]:GetYPos() + self.lines[#self.lines]:GetHeight())
            end
            table.insert(self.lines, line)
        end
    end,
}
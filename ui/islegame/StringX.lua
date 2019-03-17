-- StringX.lua
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

-- implements an extension to the stock string Lua library

StringX = {}

function StringX.SplitAt(str, index)
    local str1 = string.sub( str, 1, index )
    local str2 = string.sub( str, index+1)
    return str1, str2
end

function StringX.CharAt(str, index)
    return string.sub(str, index, index)
end

-- returns integer for position or nil if not found
function StringX.FindLastOf(str, ch)

end
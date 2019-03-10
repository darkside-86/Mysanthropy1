// Craftable.hpp
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2019
//-----------------------------------------------------------------------------
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see < https://www.gnu.org/licenses/>.
//-----------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>

namespace game
{
    // represents an entry in the crafting database
    class Craftable
    {
    public:
        // see required field
        class Required { public: std::string item; int count; };

        // name of an item in item database. what gets crafted.
        std::string name;
        // required items to be consumed from inventory to create the item.
        std::vector<Required> required;
        // how long it takes to produce the item in seconds
        int time;
        // optional building proximity requirement. entry in building database.
        std::string building;
        // minimum level requirement (defaults to 1)
        int level = 1;
    };
}
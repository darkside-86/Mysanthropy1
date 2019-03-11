// Crafting.hpp
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

#include "Craftable.hpp"
#include "Inventory.hpp"

namespace game
{
    // the crafting system. maintains a database of craftable items as well
    // as managing inventory to create items
    class Crafting
    {
    public:
        // ctor
        Crafting();
        // dtor
        virtual ~Crafting();
        // get read-only list of craftable items
        inline const std::vector<Craftable>& GetCraftables() const { return craftables_; }
        // try to modify inventory contents to produce a craftable item
        bool CraftItem(const std::string& itemToCraft, Inventory& inventory);
    private:
        // read a lua table as Craftable object and store it
        static int lua_Craftable(lua_State* L);
        // list of Craftable items
        std::vector<Craftable> craftables_;
    };
}
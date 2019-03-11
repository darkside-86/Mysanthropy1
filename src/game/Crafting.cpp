// Crafting.cpp
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

#include <lua/lua.hpp>

#include "Crafting.hpp"
#include "engine/GameEngine.hpp"

namespace game
{
    Crafting::Crafting()
    {
        lua_State* L = luaL_newstate();
        // store the CraftingSystem object
        lua_pushstring(L, "Crafting");
        lua_pushlightuserdata(L, this);
        lua_settable(L, LUA_REGISTRYINDEX);
        // bind globals
        lua_pushcfunction(L, lua_Craftable);
        lua_setglobal(L, "CRAFTABLE");
        // read the database file
        int ok = luaL_dofile(L, "res/config/crafting.lua");
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "%s: Unable to parse crafting database -- %s", __FUNCTION__, lua_tostring(L, -1));
            lua_pop(L, 1);
        }

        lua_close(L);
    }

    Crafting::~Crafting()
    {

    }

    bool Crafting::CraftItem(const std::string& itemToCraft, Inventory& inventory)
    {
        // first make sure the item to craft is in the database of craftable items.
        std::vector<Craftable>::iterator found = std::find_if(craftables_.begin(), craftables_.end(),
            [itemToCraft](const Craftable& c) -> bool {
                return itemToCraft == c.name;
            }
        );
        if(found == craftables_.end())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Craftable item `%s' does not exist.", __FUNCTION__, itemToCraft.c_str());
            return false;
        }
        // (if the item is in craftables_ but not inventory, inventory will report an error when
        //  we try to add it anyway)
        
        // check inventory to make sure we have enough of the required items to craft the item
        for(const Craftable::Required& requirement : found->required)
        {
            int has = inventory.GetItemAmount(requirement.item);
            if(has < requirement.count)
            {
                // NOTE: The UI should do its own validation to report insufficient items
                // to the user on screen. This check exists because the UI cannot be trusted.
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                    "%s: Not enough `%s' to make a `%s'. (Need: %d, Have %d)", __FUNCTION__,
                    requirement.item.c_str(), itemToCraft.c_str(), requirement.count, has);
                return false;
            }
            // don't consume items yet--need a separate loop in case we run into an unmet item
            // requirement. Otherwise items might get thrown away for nothing.
        }

        // item requirement passed so first consume the items then add the crafted item (1) to
        // inventory. And return true to indicate success.
        for(const Craftable::Required& requirement : found->required)
        {
            inventory.SetItemAmount(requirement.item, inventory.GetItemAmount(requirement.item) - requirement.count);
        }
        inventory.AddItemByName(found->name, 1);
        return true;
    }

    int Crafting::lua_Craftable(lua_State* L)
    {
        // get the CraftingSystem object.
        lua_pushstring(L, "Crafting");
        lua_gettable(L, LUA_REGISTRYINDEX);
        Crafting* cs = (Crafting*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        Craftable craftable;
        // fill in the fields of the craftable with fields of lua table argument

        // name : string
        lua_pushstring(L, "name");
        lua_gettable(L, 1);
        craftable.name = lua_tostring(L, -1); // required field so no checking
        lua_pop(L, 1);
        // requires : array of int,string...
        lua_pushstring(L, "requires");
        lua_gettable(L, 1);
        lua_len(L, -1);
        size_t len = lua_tointeger(L, -1);
        lua_pop(L, 1);
        for(size_t i = 1; i <= len; i += 2)
        {
            Craftable::Required req;
            // read the item count first
            lua_pushinteger(L, i);
            lua_gettable(L, -2);
            req.count = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            // read the item name
            lua_pushinteger(L, i+1);
            lua_gettable(L, -2);
            req.item = lua_tostring(L, -1);
            lua_pop(L, 1);
            craftable.required.push_back(req);
        }
        lua_pop(L, 1);
        // time : int
        lua_pushstring(L, "time");
        lua_gettable(L, 1);
        craftable.time = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // building : string (optional)
        lua_pushstring(L, "building");
        lua_gettable(L, 1);
        craftable.building = !lua_isnil(L,-1)? lua_tostring(L, -1): "";
        lua_pop(L, 1);
        // level : int (optional)
        lua_pushstring(L, "level");
        lua_gettable(L, 1);
        craftable.level = !lua_isnil(L,-1)? (int)lua_tointeger(L, -1): 1;
        lua_pop(L, 1);

        cs->craftables_.push_back(craftable);
        return 0;
    }
}
// ItemTable.cpp
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

#include "engine/GameEngine.hpp"
#include "ItemTable.hpp"

namespace game
{
    ItemTable::ItemTable()
    {
        // create lua state for reading db
        lua_State* L = luaL_newstate();
        // store this in registry
        lua_pushstring(L, "ItemTable");
        lua_pushlightuserdata(L, this);
        lua_settable(L, LUA_REGISTRYINDEX);
        // setup global function(s)
        lua_pushcfunction(L, lua_ItemEntry);
        lua_setglobal(L, "ITEM_ENTRY");
        // read the database file and report any errors
        int ok = luaL_dofile(L, "res/config/items.lua");
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "%s: Unable to read item database -- %s", __FUNCTION__, lua_tostring(L, -1));
            lua_pop(L, 1);
        }

        // close lua state -- no longer needed once data is read
        lua_close(L);
    }

    ItemTable::~ItemTable()
    {
        // delete all the entry pointers
        for(auto& pairEntry : itemEntries_)
        {
            delete pairEntry.second;
        }
    }

    int ItemTable::lua_ItemEntry(lua_State* L)
    {
        // get the ItemTable object stored in this state
        lua_pushstring(L, "ItemTable");
        lua_gettable(L, LUA_REGISTRYINDEX);
        ItemTable* itemTable = (ItemTable*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        // argument is a table with the following fields:
        //  name : string, 
        //  title : string (optional), defaults to name with first letter capitalized
        //  icon : string, 
        //  hidden : boolean (optional), defaults to false
        //  unique : boolean (optional), defaults to false
        //  tooltip : string (optional), defaults to ""
        //  foodstuff : integer (optional), defaults to 0
        //  type : string (optional), defaults to equivalent enum ItemType::None
        // Depending on type, the following fields are present.
        //  type="equipment" : equipment : table { durability : integer, TODO... } 
        //  TODO: type="consumable" : consumable : table { ??? }
        //  TODO: and type="food" : food : table { ??? }
        
        // read the name field. This is required and serves as the database primary key
        lua_pushstring(L, "name");
        lua_gettable(L, 1);
        std::string name = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        // read the title field if present. Otherwise use name and just capitalize the first letter
        //  by subtracting 0x20 from the first char if it is between 'a' and 'z' inclusive.
        lua_pushstring(L, "title");
        lua_gettable(L, 1);
        std::string title;
        if(lua_isnil(L, -1))
        {
            title = name;
            if(title[0] >= 'a' && title[0] <= 'z')
                title[0] -= 0x20;
        }
        else
        {
            title = luaL_checkstring(L, -1);
        }
        lua_pop(L, 1);

        // Read the optional tooltip
        lua_pushstring(L, "tooltip");
        lua_gettable(L, 1);
        std::string tooltip = lua_isnil(L, -1) ? "" : luaL_checkstring(L, -1);
        lua_pop(L, 1);

        // Read the icon path string
        lua_pushstring(L, "icon");
        lua_gettable(L, 1);
        std::string icon = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        // Read the hidden boolean
        lua_pushstring(L, "hidden");
        lua_gettable(L, 1);
        bool hidden = lua_toboolean(L, -1);
        lua_pop(L, 1);

        // read the unique boolean
        lua_pushstring(L, "unique");
        lua_gettable(L, 1);
        bool unique = lua_toboolean(L, -1);
        lua_pop(L, 1);

        // Read the foodstuff int
        lua_pushstring(L, "foodstuff");
        lua_gettable(L, 1);
        int foodstuff = (int)lua_tointeger(L, -1); // nil here defaults to 0
        lua_pop(L, 1);

        // Read type and convert it to enum
        lua_pushstring(L, "type");
        lua_gettable(L, 1);
        std::string typeStr = lua_isnil(L, -1) ? "" : luaL_checkstring(L, -1);
        lua_pop(L, 1);
        ItemType type = ItemType::None;
        if(typeStr == "consumable")
            type = ItemType::Consumable;
        else if(typeStr == "equipment")
            type = ItemType::Equipment;
        else if(typeStr == "food")
            type = ItemType::Food;

        // the object can be created and its appropriate info members filled
        ItemEntry* itemEntry = new ItemEntry(name, title, tooltip, icon, hidden, unique, foodstuff, type);
        if(itemEntry->type == ItemType::Consumable)
        {   // look for a "consumable" table
            lua_pushstring(L, "consumable");
            lua_gettable(L, 1);

            // here we would access the table at -1 to fill ((ConsumableItemInfo*)itemEntry->info)

            lua_pop(L, 1);
        }
        else if(itemEntry->type == ItemType::Equipment)
        {   // look for a "equipment" table
            lua_pushstring(L, "equipment");
            lua_gettable(L, 1);

            // only thing to read is "durability"
            lua_pushstring(L, "durability");
            lua_gettable(L, -2);
            ((EquipmentItemInfo*)itemEntry->info)->durability = (int)lua_tointeger(L, -1);
            lua_pop(L, 1); // pop durability

            lua_pop(L, 1); // pop equipment{} -- these are separate statements because in 
                            // the future more than durability will be read
        }
        else if(itemEntry->type == ItemType::Food)
        {   // get the "food" table
            lua_pushstring(L, "food");
            lua_gettable(L, 1);
            // here we would access fields at -1 to fill ((FoodItemInfo*)itemEntry->info)
            lua_pop(L, 1);
        }

        // add this itemEntry to the database finally
        itemTable->itemEntries_.insert(std::pair<std::string,const ItemEntry*>(
            itemEntry->name, itemEntry));

        return 0;
    }
}
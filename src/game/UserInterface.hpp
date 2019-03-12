// UserInterface.hpp
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

#include <lua/lua.hpp>

#include "engine/ui/LuaBindings.hpp"

namespace game
{
    class IsleGame; // forward declaration.
    
    class UserInterface
    {
    public:
        // ctor - Sets up lua state and then bindings
        UserInterface(IsleGame& isleGame);
        // dtor - Cleans up bindings and then lua state
        virtual ~UserInterface();

    //-- C++ interface to Lua defined UI methods ------------------------------

        // Shows or hides the InventoryFrame depending on its current visibility
        void UI_Inventory_Toggle();
        // Destroys and repopulates inventory item grid
        void UI_Inventory_Setup();

    //-------------------------------------------------------------------------

    private:
        // prints the Lua error and pops it after a bad pcall
        void PrintLuaError(lua_State* L);
        // retrieves the "this" object for the lua_ functions. The UISystem* is stored in the Lua state registry
        static UserInterface* GetUserInterface(lua_State* L);

    //-- lua bound functions --------------------------------------------------

        // Game_Inventory_ConvertItemToFoodstuff(name,count) : boolean
        // Attempts to convert a given number of item `name' into foodstuff
        //  currency. Returns true on success, else false.
        static int lua_Game_Inventory_ConvertItemToFoodstuff(lua_State* L);

        // Game_Inventory_GetItems() : {...}
        // returns an array of items currently inventory. fields of each element are
        // name and durability.
        static int lua_Game_Inventory_GetItems(lua_State* L);

        // Game_ItemTable_GetItemEntries() : {...}
        // returns a table of the entire item database. should be called once by UI.
        //  keys are the item name. See items.lua for fields.
        static int lua_Game_ItemTable_GetItemEntries(lua_State* L);

    //-------------------------------------------------------------------------

        // the lua state
        lua_State* script_;
        // bindings to UI library
        engine::ui::LuaBindings* luaBindings_;
        // toggle for crafting window
        bool toggleCraftingWindow_ = false;
        // reference to the IsleGame, set in constructor
        IsleGame& game_;
    };

}
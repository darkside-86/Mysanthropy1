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
    
        // Set the left CD indicator bar to value (0-1)
        void UI_ActionBar_SetLeftCDValue(float value);
        // Set the right CD indicator bar to value (0-1)
        void UI_ActionBar_SetRightCDValue(float value);
        // Toggle the building frame visibility
        void UI_BuildingFrame_Toggle();
        // Set the activity text of the cast bar
        void UI_CastBar_SetActivity(const std::string activity);
        // Set the fill color of the cast bar
        void UI_CastBar_SetFillColor(float r, float g, float b, float a);
        // Set the value (between 0 and 1) to change the width of the fill
        void UI_CastBar_SetValue(float value);
        // Set the visibility of the cast bar
        void UI_CastBar_SetVisible(bool visible);
        // Write a colored line of text to the console. TODO: Wrap
        void UI_Console_WriteLine(const std::string& text, float r=1.f, float g=1.f, float b=1.f, float a=1.f);
        // Show or hide the crafting window
        void UI_Crafting_Toggle();
        // Sets percentage (0 to 1) of progress of experience bar
        void UI_ExperienceBar_SetValue(float value);
        // Shows or hides the InventoryFrame depending on its current visibility
        void UI_Inventory_Toggle();
        // Destroys and repopulates inventory item grid
        void UI_Inventory_Setup();
        // Sets unit frame name and level
        void UI_UnitFrame_SetNameAndLevel(const std::string& name, 
                int level, const std::string& hostility, bool isPlayer=false);
        // Sets a unit frame health
        void UI_UnitFrame_SetHealth(int ch, int mh, bool isp=false);
        // Sets visibility of a unit frame
        void UI_UnitFrame_SetVisible(bool vis, bool isp=false);

    //-------------------------------------------------------------------------

    private:
        // prints the Lua error and pops it after a bad pcall
        void PrintLuaError(lua_State* L);
        // retrieves the "this" object for the lua_ functions. The UISystem* is stored in the Lua state registry
        static UserInterface* GetUserInterface(lua_State* L);

    //-- lua bound functions --------------------------------------------------

        // Game_StartBuilding(buildName) :
        // Tell the game to start placing a building outline on the ground to select a place to
        //  to start building. Argument is database key for building to build.
        static int lua_Game_StartBuilding(lua_State* L);

        // Game_StartCrafting(itemName) : 
        // Tell the game to begin crafting a specified item. The game checks for the validity and requirements.
        // Because crafting is not instant there's no way to get a meaningful return value here.
        static int lua_Game_StartCrafting(lua_State* L);

        // Game_BuildTable_GetEntries() : { {name:{ name:string, title:string, tooltip:string,
        //  texture:string, width:integer, height:integer, required:[{name:string, count:integer}...],
        //  collision = {left:integer, top:integer, right:integer, bottom:integer}, time:integer, level:integer
        //  harvesting:nil or {maxClicks:integer, time:integer, drops:[{item:string, count:integer, 
        //   chance:number}...], completed:string}, 
        //  farming:nil or {maxFarms:integer, time:integer, 
        //   frequency:integer, pending:string, drops:[{item:string, count:integer, chance:number}...],
        //   completed:string},
        //  crafting:nil or [{title:string, sources:[{name:string, count:integer}...], time:integer,
        //   pending:string, completed:string, results:[{item:string, count:integer, chance:number}...]}...]
        //  removal:nil or {time:integer, drops:[{item:string, count:integer, chance:number}...]}...}
        // Returns list of all possible buildings and information about each. Should only be called
        //  once because the database never changes during game play.
        static int lua_Game_BuildTable_GetEntries(lua_State* L);

        // Game_Crafting_GetCraftables() : { name : string, required : [ { item:string, count:integer }... ], 
        //  time : integer, building : string, level : integer, yield : integer }
        // Returns list of all craftable items according to the database. Should be called only once as
        //  crafting database never changes during game play.
        static int lua_Game_Crafting_GetCraftables(lua_State* L);

        // Game_Inventory_ConvertItemToFoodstuff(name,count) : boolean
        // Attempts to convert a given number of item `name' into foodstuff
        //  currency. Returns true on success, else false.
        static int lua_Game_Inventory_ConvertItemToFoodstuff(lua_State* L);

        // Game_Inventory_GetItems([name]) : {...}
        // returns an array of items currently inventory. fields of each element are
        // name and durability. If name is specified, only returns items whose name matches
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
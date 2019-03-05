// UISystem.hpp
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
class TileGame; // forward declaration.

class UISystem
{
public:
    // ctor - Sets up lua state and then bindings
    UISystem(TileGame& tileGame);
    // dtor - Cleans up bindings and then lua state
    virtual ~UISystem();
    // writes an unwrapped colored line to console
    void WriteLineToConsole(const std::string& line, float r=1.f, float g=1.f, float b=1.f, float a=1.f);
    // sets the percentage of progress on the cast bar (parameter between 0 and 1)
    void SetCastBarValue(float value);
    // Toggles the visibility of the taskbar
    void ToggleCastBar(bool show);
    // Sets the percentage of progress of the experience bar
    void SetExperienceBar(float value);
    // shows or hide inventory window
    void ShowInventory(bool show);
    // Creates the UI elements for the inventory window such as each item icon
    void BuildInventory();
    // Sets the value of the foodstuff counter in the corner
    void SetFoodstuffBarData(int amount);
    // Show dialog box for returning to main menu
    void ShowMMPopup(bool show);
private:
    // prints the Lua error and pops it after a bad pcall
    void PrintLuaError(lua_State* L);
    // retrieves the "this" object for the lua_ functions. The UISystem* is stored in the Lua state registry
    static UISystem* GetUISystem(lua_State* L);
    // lua bound functions
    static int lua_Game_GetInventory(lua_State* L);
    static int lua_Game_ConvertItemToFoodstuff(lua_State* L);
    static int lua_Game_GetFoodstuffCount(lua_State* L);
    static int lua_Game_ReturnToMainMenu(lua_State* L);

    // the lua state
    lua_State* script_;
    // bindings to UI library
    engine::ui::LuaBindings* luaBindings_;
    // reference to the TileGame, set in constructor
    TileGame& game_;
};
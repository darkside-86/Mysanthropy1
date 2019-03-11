// UserInterface.cpp
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

#include <vector>

#include "engine/GameEngine.hpp"
#include "UserInterface.hpp"
#include "IsleGame.hpp"

namespace game
{

    UserInterface::UserInterface(IsleGame& isleGame) : game_(isleGame)
    {
        script_ = luaL_newstate();
        luaL_openlibs(script_);
        luaBindings_ = new engine::ui::LuaBindings(script_);
        // set UserInterface instance in registry
        lua_pushstring(script_, "UserInterface");
        lua_pushlightuserdata(script_, this);
        lua_settable(script_, LUA_REGISTRYINDEX);
        // set globals
        lua_pushcfunction(script_, lua_Game_GetInventory);
        lua_setglobal(script_, "Game_GetInventory");
        lua_pushcfunction(script_, lua_Game_ConvertItemToFoodstuff);
        lua_setglobal(script_, "Game_ConvertItemToFoodstuff");
        lua_pushcfunction(script_, lua_Game_GetFoodstuffCount);
        lua_setglobal(script_, "Game_GetFoodstuffCount");
        lua_pushcfunction(script_, lua_Game_ReturnToMainMenu);
        lua_setglobal(script_, "Game_ReturnToMainMenu");
        lua_pushcfunction(script_, lua_Game_GetSaveSlot);
        lua_setglobal(script_, "Game_GetSaveSlot");
        lua_pushcfunction(script_, lua_Game_GetPlayerLevel);
        lua_setglobal(script_, "Game_GetPlayerLevel");
        lua_pushcfunction(script_, lua_Game_GetItemTexture);
        lua_setglobal(script_, "Game_GetItemTexture");
        lua_pushcfunction(script_, lua_Game_GetCraftables);
        lua_setglobal(script_, "Game_GetCraftables");
        // load core lua UI libraries and the main UI file
        std::vector<const char*> CORE_UI_LIB = {
            "ui/lib/fonts.lua", 
            "ui/lib/keycodes.lua", 
            "ui/lib/Window.lua", 
            "ui/IsleGame.lua" // main UI code
        };
        
        int errCode = 0;
        for(auto luaFile : CORE_UI_LIB)
        {
            errCode = luaL_dofile(script_, luaFile);
            if(errCode != 0)
                PrintLuaError(script_);
            // keep trying to load other files even if one was bad...
        }
    }

    UserInterface::~UserInterface()
    {
        delete luaBindings_;
        lua_close(script_);
    }

    void UserInterface::WriteLineToConsole(const std::string& line, float r, float g, float b, float a)
    {
        if(line == "")
            return; // nothing to write, don't generate blank lines wasting console space
        lua_getglobal(script_, "WriteLineToConsole");
        lua_pushstring(script_, line.c_str());
        lua_pushnumber(script_, r);
        lua_pushnumber(script_, g);
        lua_pushnumber(script_, b);
        lua_pushnumber(script_, a);
        int ok = lua_pcall(script_, 5, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::SetCastBarValue(float value)
    {
        lua_getglobal(script_, "SetCastBarValue");
        lua_pushnumber(script_, value);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::ToggleCastBar(bool show)
    {
        lua_getglobal(script_, "ToggleCastBar");
        lua_pushboolean(script_, show);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::SetExperienceBar(float value)
    {
        lua_getglobal(script_, "SetExperienceBar");
        lua_pushnumber(script_, value);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::ShowInventory(bool show)
    {
        lua_getglobal(script_, "ShowInventory");
        lua_pushboolean(script_, show);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::BuildInventory()
    {
        lua_getglobal(script_, "BuildInventory");
        int ok = lua_pcall(script_, 0, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::SetFoodstuffBarData(int amount)
    {
        lua_getglobal(script_, "SetFoodstuffBarData");
        lua_pushinteger(script_, amount);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::ShowMMPopup(bool show)
    {
        lua_getglobal(script_, "ToggleMMPopup");
        lua_pushboolean(script_, show);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::PlayerUnitFrame_SetNameAndLevel(const std::string& name, int level)
    {
        lua_getglobal(script_, "PlayerUnitFrame_SetNameAndLevel");
        lua_pushstring(script_, name.c_str());
        lua_pushinteger(script_, level);
        int ok = lua_pcall(script_, 2, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::PlayerUnitFrame_SetHealth(const int current, const int max)
    {
        lua_getglobal(script_, "PlayerUnitFrame_SetHealth");
        lua_pushinteger(script_, current);
        lua_pushinteger(script_, max);
        int ok = lua_pcall(script_, 2, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::TargetUnitFrame_SetNameAndLevel(const std::string& name, int level)
    {
        lua_getglobal(script_, "TargetUnitFrame_SetNameAndLevel");
        lua_pushstring(script_, name.c_str());
        lua_pushinteger(script_, level);
        int ok = lua_pcall(script_, 2, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::TargetUnitFrame_SetHealth(const int current, const int max, const std::string& hostility)
    {
        lua_getglobal(script_, "TargetUnitFrame_SetHealth");
        lua_pushinteger(script_, current);
        lua_pushinteger(script_, max);
        lua_pushstring(script_, hostility.c_str());
        int ok = lua_pcall(script_, 3, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::TargetUnitFrame_Toggle(bool show)
    {
        lua_getglobal(script_, "TargetUnitFrame_Toggle");
        lua_pushboolean(script_, show);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::LeftHandFrame_SetValue(float value)
    {
        lua_getglobal(script_, "LeftHandFrame_SetValue");
        lua_pushnumber(script_, value);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }
       
    void UserInterface::RightHandFrame_SetValue(float value)
    {
        lua_getglobal(script_, "RightHandFrame_SetValue");
        lua_pushnumber(script_, value);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

    void UserInterface::CraftingWindow_Toggle()
    {
        toggleCraftingWindow_ = !toggleCraftingWindow_;
        lua_getglobal(script_, "CraftingWindow_Toggle");
        lua_pushboolean(script_, toggleCraftingWindow_);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK)
            PrintLuaError(script_);
    }

//-----------------------------------------------------------------------------

    void UserInterface::PrintLuaError(lua_State* L)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
            "%s: %s", __FUNCTION__, lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    UserInterface* UserInterface::GetUserInterface(lua_State* L)
    {
        // get "this" UserInterface object
        lua_pushstring(L, "UserInterface");
        lua_gettable(L, LUA_REGISTRYINDEX);
        UserInterface* ui= (UserInterface*)lua_touserdata(L, -1);
        lua_pop(L, 1);
        return ui;
    }

    int UserInterface::lua_Game_GetInventory(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);
        Inventory& inventory = ui->game_.GetInventory();
        lua_newtable(L);
        inventory.ForEachItemEntry([L](const std::string& name, const ItemEntry& ie){
            if(ie.count > 0 && !ie.item->IsHiddenFromInventory())
            {
                int nextIndex = (int)lua_rawlen(L, -1);
                nextIndex++;
                lua_newtable(L);
                lua_pushstring(L, "name");
                lua_pushstring(L, name.c_str());
                lua_rawset(L, -3);
                lua_pushstring(L, "texture");
                const std::string tpath = ie.item->GetIcon()->GetPath();
                lua_pushstring(L, tpath.c_str());
                lua_rawset(L, -3);
                lua_pushstring(L, "count");
                lua_pushinteger(L, ie.count);
                lua_rawset(L, -3);
                lua_rawseti(L, -2, nextIndex);
            }
        });
        return 1;
    }

    int UserInterface::lua_Game_ConvertItemToFoodstuff(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);

        const char* name = lua_tostring(L, 1);
        int amount = (int)lua_tointeger(L, 2);
        bool result = ui->game_.GetInventory().ConvertItemToFoodstuff(name, amount);

        lua_pushboolean(L, result);
        return 1;
    }

    int UserInterface::lua_Game_GetFoodstuffCount(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);

        int count = ui->game_.GetInventory().GetItemAmount("foodstuff");

        lua_pushinteger(L, count);
        return 1;
    }

    int UserInterface::lua_Game_ReturnToMainMenu(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);

        ui->game_.SetGameState(IsleGame::GAME_STATE::RETURNING_TO_MENU);
        
        return 0;
    }

    int UserInterface::lua_Game_GetSaveSlot(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);

        std::string slot = ui->game_.GetSaveSlot();
        lua_pushstring(L, slot.c_str());
        return 1;
    }

    int UserInterface::lua_Game_GetPlayerLevel(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);
        auto& playerSprite = ui->game_.GetPlayerSprite();
        lua_pushinteger(L, playerSprite.GetPlayerCombatUnit().GetAttributeSheet().GetLevel());
        return 1;
    }

    int UserInterface::lua_Game_GetCraftables(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);
        const auto& craftables = ui->game_.GetCrafting().GetCraftables();
        // build a Lua array of Lua tables with the fields specified by Craftable class
        lua_newtable(L);
        int luaArrayCounter = 1;
        for(const auto& craftable : craftables)
        {
            lua_pushinteger(L, luaArrayCounter); // the array key
            lua_newtable(L); // the array value
             // name : string
             lua_pushstring(L, "name");
             lua_pushstring(L, craftable.name.c_str());
             lua_settable(L, -3);
             // required : array of { name="some item", count=howmanyareneeded}...
             lua_pushstring(L, "required");
             lua_newtable(L);
             int requiredCounter = 1;
             for(const auto& eachRequirement : craftable.required)
             {
                lua_pushinteger(L, requiredCounter);
                lua_newtable(L);
                 lua_pushstring(L, "name");
                 lua_pushstring(L, eachRequirement.item.c_str());
                 lua_settable(L, -3);
                 lua_pushstring(L, "count");
                 lua_pushinteger(L, eachRequirement.count);
                 lua_settable(L, -3);
                lua_settable(L, -3);
                ++requiredCounter;
             }
             lua_settable(L, -3);
             // time : integer
             lua_pushstring(L, "time");
             lua_pushinteger(L, craftable.time);
             lua_settable(L, -3);
             // building : string
             lua_pushstring(L, "building");
             lua_pushstring(L, craftable.building.c_str());
             lua_settable(L, -3);
             // level : integer
             lua_pushstring(L, "level");
             lua_pushinteger(L, craftable.level);
             lua_settable(L, -3);
            lua_settable(L, -3);
            ++luaArrayCounter;
        }
        return 1;
    }

    int UserInterface::lua_Game_GetItemTexture(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);
        std::string itemName = luaL_checkstring(L, 1);
        auto item = ui->game_.GetInventory().GetItemEntryByName(itemName).item;
        if(item == nullptr)
        {
            luaL_error(L, "%s: Item `%s' is not valid!", __FUNCTION__, itemName.c_str());
        }

        std::string texturePath = item->GetTexturePath();
        lua_pushstring(L, texturePath.c_str());        
        
        return 1;
    }

}
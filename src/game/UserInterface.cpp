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
        lua_pushcfunction(script_, lua_Game_StartCrafting);
        lua_setglobal(script_, "Game_StartCrafting");
        lua_pushcfunction(script_, lua_Game_Crafting_GetCraftables);
        lua_setglobal(script_, "Game_Crafting_GetCraftables");
        lua_pushcfunction(script_, lua_Game_Inventory_ConvertItemToFoodstuff);
        lua_setglobal(script_, "Game_Inventory_ConvertItemToFoodstuff");
        lua_pushcfunction(script_, lua_Game_Inventory_GetItems);
        lua_setglobal(script_, "Game_Inventory_GetItems");
        lua_pushcfunction(script_, lua_Game_ItemTable_GetItemEntries);
        lua_setglobal(script_, "Game_ItemTable_GetItemEntries");
        // load core lua UI libraries and the main UI file
        std::vector<const char*> CORE_UI_LIB = {
            "ui/lib/fonts.lua", 
            "ui/lib/keycodes.lua", 
            "ui/lib/Window.lua", 
            "ui/islegame/main.lua"
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

//-----------------------------------------------------------------------------

    void UserInterface::UI_ActionBar_SetLeftCDValue(float value)
    {
        lua_getglobal(script_, "UI_ActionBar_SetLeftCDValue");
        lua_pushnumber(script_, value);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }
    
    void UserInterface::UI_ActionBar_SetRightCDValue(float value)
    {
        lua_getglobal(script_, "UI_ActionBar_SetRightCDValue");
        lua_pushnumber(script_, value);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }

    void UserInterface::UI_CastBar_SetActivity(const std::string activity)
    {
        lua_getglobal(script_, "UI_CastBar_SetActivity");
        lua_pushstring(script_, activity.c_str());
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }
        
    void UserInterface::UI_CastBar_SetFillColor(float r, float g, float b, float a)
    {
        lua_getglobal(script_, "UI_CastBar_SetFillColor");
        lua_pushnumber(script_, r);     lua_pushnumber(script_, g);
        lua_pushnumber(script_, b);     lua_pushnumber(script_, a);
        int ok = lua_pcall(script_, 4, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }
    
    void UserInterface::UI_CastBar_SetValue(float value)
    {
        lua_getglobal(script_, "UI_CastBar_SetValue");
        lua_pushnumber(script_, value);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }
        
    void UserInterface::UI_CastBar_SetVisible(bool visible)
    {
        lua_getglobal(script_, "UI_CastBar_SetVisible");
        lua_pushboolean(script_, visible);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }

    void UserInterface::UI_Console_WriteLine(const std::string& text, float r, float g, float b, float a)
    {
        lua_getglobal(script_, "UI_Console_WriteLine");
        lua_pushstring(script_, text.c_str());
        lua_pushnumber(script_, r);     lua_pushnumber(script_, g);
        lua_pushnumber(script_, b);     lua_pushnumber(script_, a);
        int ok = lua_pcall(script_, 5, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }

    void UserInterface::UI_Crafting_Toggle()
    {
        lua_getglobal(script_, "UI_Crafting_Toggle");
        int ok = lua_pcall(script_, 0, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }

    void UserInterface::UI_ExperienceBar_SetValue(float value)
    {
        lua_getglobal(script_, "UI_ExperienceBar_SetValue");
        lua_pushnumber(script_, value);
        int ok = lua_pcall(script_, 1, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }

    void UserInterface::UI_Inventory_Toggle()
    {
        lua_getglobal(script_, "UI_Inventory_Toggle");
        int ok = lua_pcall(script_, 0, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }

    void UserInterface::UI_Inventory_Setup()
    {
        lua_getglobal(script_, "UI_Inventory_Setup");
        int ok = lua_pcall(script_, 0, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }

    void UserInterface::UI_UnitFrame_SetNameAndLevel(const std::string& name, 
        int level, const std::string& hostility, bool isPlayer)
    {
        lua_getglobal(script_, "UI_UnitFrame_SetNameAndLevel");
        lua_pushstring(script_, name.c_str());
        lua_pushinteger(script_, level);
        lua_pushstring(script_, hostility.c_str());
        lua_pushboolean(script_, isPlayer);
        int ok = lua_pcall(script_, 4, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }
    
    void UserInterface::UI_UnitFrame_SetHealth(int ch, int mh, bool isp)
    {
        lua_getglobal(script_, "UI_UnitFrame_SetHealth");
        lua_pushinteger(script_, ch);
        lua_pushinteger(script_, mh);
        lua_pushboolean(script_, isp);
        int ok = lua_pcall(script_, 3, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
    }

    void UserInterface::UI_UnitFrame_SetVisible(bool vis, bool isp)
    {
        lua_getglobal(script_, "UI_UnitFrame_SetVisible");
        lua_pushboolean(script_, vis);
        lua_pushboolean(script_, isp);
        int ok = lua_pcall(script_, 2, 0, 0);
        if(ok != LUA_OK) PrintLuaError(script_);
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

    // Lua bound C++ functions ////////////////////////////////////////////////

    int UserInterface::lua_Game_StartCrafting(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);
        const std::string itemName = luaL_checkstring(L, 1);
        ui->game_.StartCrafting(itemName);
        return 0;
    }

    int UserInterface::lua_Game_Crafting_GetCraftables(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);
        const auto& craftables = ui->game_.GetCrafting().GetCraftables();
        lua_newtable(L);
        int arrayCounter = 1;
        for(const auto& entry : craftables)
        {
            lua_pushinteger(L, arrayCounter);
            lua_newtable(L);

            lua_pushstring(L, "name");
            lua_pushstring(L, entry.name.c_str());
            lua_settable(L, -3);
            lua_pushstring(L, "yield");
            lua_pushinteger(L, entry.yield);
            lua_settable(L, -3);
            lua_pushstring(L, "required");
            lua_newtable(L);
            int rc = 1;
            for(const auto& req : entry.required)
            {
                lua_pushinteger(L, rc);
                lua_newtable(L);

                lua_pushstring(L, "item");
                lua_pushstring(L, req.item.c_str());
                lua_settable(L, -3);
                lua_pushstring(L, "count");
                lua_pushinteger(L, req.count);
                lua_settable(L, -3);
                
                lua_settable(L, -3);
                ++rc;
            }
            lua_settable(L, -3);
            lua_pushstring(L, "time");
            lua_pushinteger(L, entry.time);
            lua_settable(L, -3);
            lua_pushstring(L, "building");
            lua_pushstring(L, entry.building.c_str());
            lua_settable(L, -3);
            lua_pushstring(L, "level");
            lua_pushinteger(L, entry.level);
            lua_settable(L, -3);

            lua_settable(L, -3);
            ++arrayCounter;
        }
        return 1;
    }

    int UserInterface::lua_Game_Inventory_ConvertItemToFoodstuff(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);
        const std::string name = luaL_checkstring(L, 1);
        int count = (int)lua_tointeger(L, 2);
        lua_pushboolean(L, ui->game_.GetInventory().ConvertItemToFoodstuff(name, count));
        return 1;
    }

    int UserInterface::lua_Game_Inventory_GetItems(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);
        std::string search = "";
        if(lua_gettop(L) > 0)
            search = luaL_checkstring(L, 1);
        const auto& items = ui->game_.GetInventory().GetItems();
        lua_newtable(L);
        int arrayCounter = 1;
        for(const Item* item : items)
        {
            // if argument 1 was specified, that is what to check before adding to array
            if(search != "")
            {
                if(item->GetItemEntry().name != search)
                    continue;
            }
            lua_pushinteger(L, arrayCounter);
            lua_newtable(L);

            // name to link it to item database
            lua_pushstring(L, "name");
            const std::string name = item->GetItemEntry().name;
            lua_pushstring(L, name.c_str());
            lua_settable(L, -3);
            // count (how many)
            lua_pushstring(L, "count");
            lua_pushinteger(L, item->count);
            lua_settable(L, -3);
            // durability
            lua_pushstring(L, "durability");
            lua_pushinteger(L, item->durability);
            lua_settable(L, -3);

            lua_settable(L, -3);
            ++arrayCounter;
        }
        return 1;
    }

    int UserInterface::lua_Game_ItemTable_GetItemEntries(lua_State* L)
    {
        UserInterface* ui = GetUserInterface(L);

        const auto& itemEntries = ui->game_.GetItemTable().GetItemEntries();

        lua_newtable(L);
        for(const auto& kv : itemEntries)
        {
            lua_pushstring(L, kv.first.c_str());
            lua_newtable(L);

            // name : string
            lua_pushstring(L, "name");
            lua_pushstring(L, kv.second->name.c_str());
            lua_settable(L, -3);
            // title : string
            lua_pushstring(L, "title");
            lua_pushstring(L, kv.second->name.c_str());
            lua_settable(L, -3);
            // tooltip : string
            lua_pushstring(L, "tooltip");
            lua_pushstring(L, kv.second->tooltip.c_str());
            lua_settable(L, -3);
            // icon : string
            lua_pushstring(L, "icon");
            lua_pushstring(L, kv.second->icon.c_str());
            lua_settable(L, -3);
            // hidden : boolean
            lua_pushstring(L, "hidden");
            lua_pushboolean(L, kv.second->hidden);
            lua_settable(L, -3);
            // unique : boolean 
            lua_pushstring(L, "unique");
            lua_pushboolean(L, kv.second->unique);
            lua_settable(L, -3);
            // foodstuff : integer
            lua_pushstring(L, "foodstuff");
            lua_pushinteger(L, kv.second->foodstuff);
            lua_settable(L, -3);
            // type : string AND the associated table if any
            lua_pushstring(L, "type");
            switch(kv.second->type)
            {
                case ItemType::Consumable: lua_pushstring(L, "consumable"); break;
                case ItemType::Equipment: lua_pushstring(L, "equipment"); break;
                case ItemType::Food: lua_pushstring(L, "food"); break;
                default: lua_pushstring(L, "");
            }
            lua_settable(L, -3);
            if(kv.second->type != ItemType::None)
            {
                switch(kv.second->type)
                {
                    case ItemType::Consumable: 
                        lua_pushstring(L, "consumable"); 
                        lua_newtable(L);
                        // nothing to push on to table yet
                        break;
                    case ItemType::Equipment: 
                        lua_pushstring(L, "equipment"); 
                        lua_newtable(L);
                        lua_pushstring(L, "durability");
                        lua_pushinteger(L, ((EquipmentItemInfo*)kv.second->info)->durability);
                        lua_settable(L, -3);
                        break;
                    case ItemType::Food: 
                        lua_pushstring(L, "food"); 
                        lua_newtable(L);
                        // nothing to push on to table yet
                        break;
                }
                lua_settable(L, -3);
            }
            lua_settable(L, -3);
        }
        return 1;
    }

}
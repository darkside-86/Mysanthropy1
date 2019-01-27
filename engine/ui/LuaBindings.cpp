// LuaBindings.cpp
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2018
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

#include "LuaBindings.h"

#include <sstream>

#include "Root.h"

namespace engine { namespace ui {
    
    // keys for RTTI table
    static const char* RTTI_TABLE_KEY = "classes";
    static const char* IS_OBJECT = "Object"; 
    static const char* IS_FRAME = "Frame";

    // general functions
    static void GetRTTI(lua_State* L, int index);
    static bool CheckRTTI(lua_State* L, const char* className);
    Object* CheckObject(lua_State* L, int index);
    Frame* CheckFrame(lua_State* L, int index);

    // Place RTTI table on to stack
    static void GetRTTI(lua_State* L, int index)
    {
        lua_getmetatable(L, index); // m{}
        lua_pushstring(L, RTTI_TABLE_KEY); // m{} $
        lua_gettable(L, -2);        // m{} rtti{}
        if(lua_isnil(L, -1) || !lua_istable(L, -1))
        {
            luaL_error(L, "%s: Invalid RTTI table in metatable", __FUNCTION__);
        }
        lua_remove(L, -2);          // rtti{}
        // RTTI is now on the stack
    }

    // Check RTTI table at stack = -1
    static bool CheckRTTI(lua_State* L, const char* className)
    {
        lua_pushstring(L, className);
        lua_gettable(L, -2);
        bool isInstance = !lua_isnil(L, -1);
        lua_pop(L, 1); // pop nil or true from stack
        return isInstance;
    }

    struct LuaWriterData
    {
        char* bytecode = nullptr;
        size_t size = 0;
    };

    // lua : UIObject.AddOnClicked(self, luaFunction)
    static int lua_UIObject_AddOnClicked(lua_State *L)
    {
        Object* self = CheckObject(L, 1);
        luaL_checktype(L, 2, LUA_TFUNCTION);
        LuaWriterData data;
        lua_pushvalue(L, 2);
        int result = lua_dump(L,[](lua_State*,const void* p, size_t sz, void* ud)->int{
            LuaWriterData* data = (LuaWriterData*)ud;
            data->bytecode = (char*)realloc(data->bytecode, data->size + sz);
            memcpy(data->bytecode + data->size, p, sz);
            data->size += sz;
            return 0;
        }, &data, 0);
        lua_pop(L, 1);
        ClickedEventCallback ceb = [L, data](const ClickedEvent& e) {
            int result = luaL_loadbuffer(L, data.bytecode, data.size, "OnClick");
            lua_pushinteger(L, e.x);
            lua_pushinteger(L, e.y);
            lua_pushinteger(L, e.button);
            lua_call(L, 3, 0);
        };
        self->AddOnClicked(ceb);
        return 0;
    }

    // lua : UIFrame.New(parent,width,height,xpos,ypos,'texture')
    static int lua_UIFrame_New(lua_State* L)
    {
        Object* parent;
        if(lua_isnil(L, 1))
            parent = Root::Get();
        else
            parent = CheckObject(L, 1);
        
        int width = (int)luaL_checkinteger(L, 2);
        int height = (int)luaL_checkinteger(L, 3);
        int xpos = (int)luaL_checkinteger(L, 4);
        int ypos = (int)luaL_checkinteger(L, 5);
        const char* textureAlias = luaL_checkstring(L, 6);
        engine::ui::Color white = {1.f,1.f,1.f,1.f};
        ogl::Texture* texture = GameEngine::Get().GetTextureManager().GetTexture(textureAlias);
        // Frame frame(parent,width,height,xpos,ypos,texture,white);
        void* ud = lua_newuserdata(L, sizeof(Frame));
        // memcpy(ud, &frame, sizeof(Frame));
        Frame *frame = (Frame*)ud;
        frame = new (frame) Frame(parent,width,height,xpos,ypos,texture,white);
        luaL_setmetatable(L, IS_FRAME);
        lua_newtable(L);
        lua_setuservalue(L, -2);
        return 1;
    }

    // lua : UIFrame.SetColor(self, r, g, b, a)
    static int lua_UIFrame_SetColor(lua_State* L)
    {
        Frame* self = CheckFrame(L, 1);
        lua_Number r = luaL_checknumber(L, 2);
        lua_Number g = luaL_checknumber(L, 3);
        lua_Number b = luaL_checknumber(L, 4);
        lua_Number a = luaL_checknumber(L, 5);
        self->SetColor({(float)r, (float)g, (float)b, (float)a});
        return 0;
    }

    static int lua_UIFrame_mtGC(lua_State* L)
    {
        Frame* frame = CheckFrame(L, 1);
        GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, "Deleting frame %llx", frame);
        frame->~Frame();
        return 0;
    }

    static int lua_UIFrame_mtToString(lua_State* L)
    {
        Frame* frame = CheckFrame(L, 1);
        std::stringstream ss;
        ss << std::string("UIFrame ") << std::hex << frame;
        std::string str = ss.str();
        lua_pushstring(L, str.c_str());
        return 1;
    }

    // lua : getmetatable(frame).__index(self, key)
    static int lua_UIFrame_mtIndex(lua_State* L)
    {
        Frame* self = CheckFrame(L, 1); // [Top] self key
        // check uservalue first
        lua_getuservalue(L, 1); // {} [Top] self key
        lua_pushvalue(L, 2); // {} key [Top] self key
        lua_gettable(L, -2); // {} nilOrUservalue.key [Top] self key
        lua_remove(L, -2); // nilOrUservalue.key [Top] self key
        if(lua_isnil(L, -1))
        {
            // nil so check UIFrame
            lua_pop(L, 1); // [Top] self key
            lua_getglobal(L, "UIFrame"); // UIFrame [top] self key
            lua_pushvalue(L, 2); // UIFrame key [top] self key
            lua_gettable(L, -2); // UIFrame UIFrame.key [top] self key
            lua_remove(L, -2); // UIFrame.key [top] self key
            if(lua_isnil(L, -1))
            {
                // still nil so check UIObject
                lua_pop(L, 1);
                lua_getglobal(L, "UIObject");
                lua_pushvalue(L, 2);
                lua_gettable(L, -2);
                lua_remove(L, -2);
            }
        }
        return 1;
    }

    // lua : getmetatable(frame).__newindex(self, key, value)
    static int lua_UIFrame_mtNewIndex(lua_State* L)
    {
        Frame* self = CheckFrame(L, 1);
        // values are simply written to the uservalue table because it is searched first,
        //  allowing methods to be overidden
        lua_getuservalue(L, 1);
        lua_pushvalue(L, 2);
        lua_pushvalue(L, 3);
        lua_settable(L, -3);
        lua_pop(L, 1);
        return 0;
    }

    Object* CheckObject(lua_State* L, int index)
    {
        if(!lua_isuserdata(L, index))
        {
            luaL_error(L, "%s: Not a userdata", __FUNCTION__);
            return nullptr;
        }

        Object* o = (Object*)lua_touserdata(L, index);

        GetRTTI(L, index); // rtti{}
        bool isObject = CheckRTTI(L, IS_OBJECT);
        if(!isObject)
        {
            lua_pop(L, 1); // pop RTTI table
            luaL_error(L, "%s: Not instance of %s", __FUNCTION__, IS_OBJECT);
            return nullptr;
        }

        lua_pop(L, 1); // remove RTTI{}
        return o;
    }

    Frame* CheckFrame(lua_State* L, int index)
    {
        if(!lua_isuserdata(L, index))
        {
            luaL_error(L, "%s: Not a userdata", __FUNCTION__);
            return nullptr;
        }

        Frame* frame = (Frame*)lua_touserdata(L, index);

        GetRTTI(L, index); // rtti{}
        bool isObject = CheckRTTI(L, IS_FRAME);
        if(!isObject)
        {
            lua_pop(L, 1); // pop RTTI table
            luaL_error(L, "%s: Not instance of %s", __FUNCTION__, IS_FRAME);
            return nullptr;
        }

        lua_pop(L, 1); // remove RTTI{}
        return frame;   
    }

    // lua : LoadTexture(alias, path)
    static int lua_LoadTexture(lua_State* L)
    {
        const char* alias = luaL_checkstring(L, 1);
        const char* path = luaL_checkstring(L, 2);
        GameEngine::Get().GetTextureManager().LoadTexture(alias, path);
        return 0;
    }

    void InitLuaBindings(lua_State* L)
    {
        // set LoadTexture(alias,path)
        lua_pushcfunction(L, lua_LoadTexture);
        lua_setglobal(L, "LoadTexture");
        // UIObject
        lua_newtable(L);
        lua_pushstring(L, "AddOnClicked");
        lua_pushcfunction(L, lua_UIObject_AddOnClicked);
        lua_settable(L, -3);
        lua_setglobal(L, "UIObject");
        // UIFrame
        lua_newtable(L);
        lua_pushstring(L, "New");
        lua_pushcfunction(L, lua_UIFrame_New);
        lua_settable(L, -3);
        lua_pushstring(L, "SetColor");
        lua_pushcfunction(L, lua_UIFrame_SetColor);
        lua_settable(L, -3);
        lua_setglobal(L, "UIFrame");
        // create UIFrame's metatable
        luaL_newmetatable(L, IS_FRAME);
        lua_pushstring(L, "classes");
        lua_newtable(L);
        lua_pushstring(L, IS_FRAME);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_pushstring(L, IS_OBJECT);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_settable(L, -3);
        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, lua_UIFrame_mtGC);
        lua_settable(L, -3);
        lua_pushstring(L, "__tostring");
        lua_pushcfunction(L, lua_UIFrame_mtToString);
        lua_settable(L, -3);
        lua_pushstring(L, "__index");
        lua_pushcfunction(L, lua_UIFrame_mtIndex);
        lua_settable(L, -3);
        lua_pushstring(L, "__newindex");
        lua_pushcfunction(L, lua_UIFrame_mtNewIndex);
        lua_settable(L, -3);
        lua_pop(L, 1);
    }

}}
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
#include <unordered_map>
#include <vector>

#include "Button.h"
#include "Frame.h"
#include "Label.h"
#include "Object.h"
#include "Root.h"

namespace engine { namespace ui {

    // keys for RTTI table
    static const char* RTTI_TABLE_KEY = "classes";
    static const char* IS_BUTTON = "Button";
    static const char* IS_LABEL = "Label";
    static const char* IS_OBJECT = "Object"; 
    static const char* IS_FRAME = "Frame";

    // general functions
    static int lua_ErrorHandler(lua_State* L);
    static void GetRTTI(lua_State* L, int index);
    static bool CheckRTTI(lua_State* L, const char* className);
    Button* CheckButton(lua_State* L, int index);
    Label* CheckLabel(lua_State* L, int index);
    Object* CheckObject(lua_State* L, int index);
    Frame* CheckFrame(lua_State* L, int index);

    static int lua_ErrorHandler(lua_State* L)
    {
        const char* errorMsg = lua_tostring(L, 1);
        GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, "%s", errorMsg);
        return 0;
    }

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

    // lua : UIObject.AddOnClicked(self, luaFunction)
    static int lua_UIObject_AddOnClicked(lua_State *L)
    {
        static lua_Integer counter = 0;
        Object* self = CheckObject(L, 1);
        // An array of callbacks in the lua registry
        luaL_checktype(L, 2, LUA_TFUNCTION);
        lua_pushstring(L, "ClickedEventCallbacks");
        lua_gettable(L, LUA_REGISTRYINDEX);
        // increment the counter
        counter++;
        // capture the current value of the counter to access the correct callback
        lua_Integer current = counter;
        lua_pushinteger(L, counter);
        lua_pushvalue(L, 2);
        lua_settable(L, -3);
        lua_pop(L, 1);
        ClickedEventCallback cb = [L, current](const ClickedEvent& e) {
            lua_pushcfunction(L, lua_ErrorHandler);
            lua_pushstring(L, "ClickedEventCallbacks");
            lua_gettable(L, LUA_REGISTRYINDEX);
            lua_pushinteger(L, current);
            lua_gettable(L, -2);
            lua_remove(L, -2);
            lua_pushinteger(L, e.x);
            lua_pushinteger(L, e.y);
            lua_pushinteger(L, e.button);
            lua_pcall(L, 3, 0, -5);
            lua_pop(L, 1);
        };
        self->AddOnClicked(cb);
        return 0;
    }

    static int lua_UIObject_AddOnHover(lua_State* L)
    {
        static lua_Integer counter = 0;
        Object* self = CheckObject(L, 1);
        luaL_checktype(L, 2, LUA_TFUNCTION);
        lua_pushstring(L, "HoverEventCallbacks");
        lua_gettable(L, LUA_REGISTRYINDEX);
        counter++;
        lua_Integer current = counter;
        lua_pushinteger(L, counter);
        lua_pushvalue(L, 2);
        lua_settable(L, -3);
        lua_pop(L, 1);
        HoverEventCallback cb = [L, current](const HoverEvent& e) {
            lua_pushcfunction(L, lua_ErrorHandler);
            lua_pushstring(L, "HoverEventCallbacks");
            lua_gettable(L, LUA_REGISTRYINDEX);
            lua_pushinteger(L, current);
            lua_gettable(L, -2);
            lua_remove(L, -2);
            lua_pushinteger(L, e.x);
            lua_pushinteger(L, e.y);
            lua_pushinteger(L, e.xrel);
            lua_pushinteger(L, e.yrel);
            lua_pushboolean(L, e.over);
            lua_pcall(L, 5, 0, -7);
            lua_pop(L, 1);
        };
        self->AddOnHover(cb);
        return 0;
    }

    static int lua_UIObject_ContainsPoint(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        int x = (int)luaL_checkinteger(L, 2);
        int y = (int)luaL_checkinteger(L, 3);
        bool result = self->ContainsPoint(x,y);
        lua_pushboolean(L, result);
        return 1;
    }

    static int lua_UIObject_IsVisible(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        lua_pushboolean(L, self->IsVisible());
        return 1;
    }

    static int lua_UIObject_SetVisible(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        bool visible = lua_toboolean(L, 2);
        self->SetVisible(visible);
        return 0;
    }

    static int lua_UIObject_GetWidth(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        lua_pushinteger(L, self->GetWidth());
        return 1;
    }

    static int lua_UIObject_SetWidth(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        int width = (int)lua_tonumber(L, 2);
        self->SetWidth(width);
        return 0;
    }

    static int lua_UIObject_GetHeight(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        lua_pushinteger(L, self->GetHeight());
        return 1;
    }

    static int lua_UIObject_SetHeight(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        int height = (int)lua_tonumber(L, 2);
        self->SetHeight(height);
        return 0;
    }

    static int lua_UIObject_GetXPos(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        lua_pushinteger(L, self->GetXPos());
        return 1;
    }

    static int lua_UIObject_SetXPos(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        int x = (int)lua_tonumber(L, 2);
        self->SetXPos(x);
        return 0;
    }

    static int lua_UIObject_GetYPos(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        lua_pushinteger(L, self->GetYPos());
        return 1;
    }

    static int lua_UIObject_SetYPos(lua_State* L)
    {
        Object* self = CheckObject(L, 1);
        int y = (int)lua_tonumber(L, 2);
        self->SetYPos(y);
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
        void* ud = lua_newuserdata(L, sizeof(Frame));
        Frame *frame = (Frame*)ud;
        frame = new (frame) Frame(parent,width,height,xpos,ypos,texture,white);
        luaL_setmetatable(L, IS_FRAME);
        lua_newtable(L);
        lua_pushstring(L, "parent");
        lua_pushvalue(L, 1);
        lua_settable(L, -3);
        lua_setuservalue(L, -2);
        return 1;
    }

    // lua : UIFrame.GetColor() : r,g,b,a
    static int lua_UIFrame_GetColor(lua_State* L)
    {
        Frame* self = CheckFrame(L, 1);
        engine::ui::Color color = self->GetColor();
        lua_pushnumber(L, color.r);
        lua_pushnumber(L, color.g);
        lua_pushnumber(L, color.b);
        lua_pushnumber(L, color.a);
        return 4;
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

    static int lua_UIFrame_GetBorderColor(lua_State* L)
    {
        Frame* self = CheckFrame(L, 1);
        engine::ui::Color color = self->GetBorderColor();
        lua_pushnumber(L, color.r);
        lua_pushnumber(L, color.g);
        lua_pushnumber(L, color.b);
        lua_pushnumber(L, color.a);
        return 4;    
    }

    static int lua_UIFrame_SetBorderColor(lua_State* L)
    {
        Frame* self = CheckFrame(L, 1);
        lua_Number r = luaL_checknumber(L, 2);
        lua_Number g = luaL_checknumber(L, 3);
        lua_Number b = luaL_checknumber(L, 4);
        lua_Number a = luaL_checknumber(L, 5);
        self->SetBorderColor({(float)r, (float)g, (float)b, (float)a});
        return 0;
    }

    static int lua_UIFrame_GetBorderSize(lua_State* L)
    {
        Frame* self = CheckFrame(L, 1);
        lua_pushinteger(L,self->GetBorderSize());
        return 1;
    }

    static int lua_UIFrame_SetBorderSize(lua_State* L)
    {
        Frame* self = CheckFrame(L, 1);
        int size = (int)luaL_checkinteger(L, 2);
        self->SetBorderSize(size);
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

    static int lua_UIButton_New(lua_State* L)
    {
        Object* parent;
        if(lua_isnil(L, 1))
            parent = Root::Get();
        else
            parent = CheckObject(L, 1);
        
        const char* textureAlias = luaL_checkstring(L, 2);
        std::string btnText = luaL_checkstring(L, 3);
        std::string fontAlias = luaL_checkstring(L, 4);
        int padding = (int)luaL_checkinteger(L, 5);
        Button* button = (Button*)lua_newuserdata(L, sizeof(Button));
        ogl::Texture* texture = GameEngine::Get().GetTextureManager().GetTexture(textureAlias);
        button = new (button) Button(parent, texture, {1,1,1,1}, btnText, fontAlias, {.5,.5,.5,1}, padding);
        luaL_setmetatable(L, IS_BUTTON);
        lua_newtable(L);
        lua_pushstring(L, "parent");
        lua_pushvalue(L, 1);
        lua_settable(L, -3);
        lua_setuservalue(L, -2);
        return 1;
    }

    static int lua_UIButton_SetText(lua_State* L)
    {
        Button* self = CheckButton(L, 1);
        std::string text = luaL_checkstring(L, 2);
        self->SetText(text);
        return 0;
    }

    static int lua_UIButton_SetTextColor(lua_State* L)
    {
        Button* self = CheckButton(L, 1);
        float r = (float)luaL_checknumber(L, 2);
        float g = (float)luaL_checknumber(L, 3);
        float b = (float)luaL_checknumber(L, 4);
        float a = (float)luaL_checknumber(L, 5);
        self->SetTextColor({r,g,b,a});
        return 0;
    }

    static int lua_UIButton_mtIndex(lua_State* L)
    {
        Button* self = CheckButton(L, 1);
        lua_getuservalue(L, 1);
        lua_pushvalue(L, 2);
        lua_gettable(L, -2);
        lua_remove(L, -2);
        if(lua_isnil(L, -1))
        {
            // check UIButton
            lua_pop(L, 1);
            lua_getglobal(L, "UIButton");
            lua_pushvalue(L, 2);
            lua_gettable(L, -2);
            lua_remove(L, -2);
            if(lua_isnil(L, -1))
            {
                // check UIFrame's metamethod index
                lua_pop(L, 1);
                lua_pushcfunction(L, lua_ErrorHandler);
                lua_pushcfunction(L, lua_UIFrame_mtIndex);
                lua_pushvalue(L, 1);
                lua_pushvalue(L, 2);
                lua_pcall(L, 2, 1, -4);
                lua_remove(L, -2);
            }
        }
        return 1;
    }

    static int lua_UIButton_mtNewIndex(lua_State* L)
    {
        Button* self = CheckButton(L, 1);
        lua_getuservalue(L, 1);
        lua_pushvalue(L, 2);
        lua_pushvalue(L, 3);
        lua_settable(L, -3);
        lua_pop(L, 1);
        return 0;
    }

    static int lua_UIButton_mtGC(lua_State* L)
    {
        Button* self = CheckButton(L, 1);
        self->~Button();
        return 0;
    }

    static int lua_UILabel_New(lua_State* L)
    {
        Object* parent;
        if(lua_isnil(L, 1))
            parent = Root::Get();
        else
            parent = CheckObject(L, 1);
        std::string text = luaL_checkstring(L, 2);
        std::string fontAlias = luaL_checkstring(L, 3);
        float r = (float)luaL_checknumber(L, 4);
        float g = (float)luaL_checknumber(L, 5);
        float b = (float)luaL_checknumber(L, 6);
        float a = (float)luaL_checknumber(L, 7);
        Label *label = (Label*)lua_newuserdata(L, sizeof(Label));
        label = new (label) Label(parent, text, fontAlias, {r,g,b,a});
        luaL_setmetatable(L, IS_LABEL);
        lua_newtable(L);
        lua_pushstring(L, "parent");
        lua_pushvalue(L, 1);
        lua_settable(L, -3);
        lua_setuservalue(L, -2);
        return 1;
    }

    static int lua_UILabel_SetText(lua_State* L)
    {
        Label* self = CheckLabel(L, 1);
        std::string text = luaL_checkstring(L, 2);
        self->SetText(text);
        return 0;
    }

    static int lua_UILabel_mtIndex(lua_State* L)
    {
        Label* self = CheckLabel(L, 1);
        lua_getuservalue(L, 1);
        lua_pushvalue(L, 2);
        lua_gettable(L, -2);
        lua_remove(L, -2);
        if(lua_isnil(L, -1))
        {
            // check UILabel
            lua_pop(L, 1);
            lua_getglobal(L, "UILabel");
            lua_pushvalue(L, 2);
            lua_gettable(L, -2);
            lua_remove(L, -2);
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

    static int lua_UILabel_mtNewIndex(lua_State* L)
    {
        Label* self = CheckLabel(L, 1);
        lua_getuservalue(L, 1);
        lua_pushvalue(L, 2);
        lua_pushvalue(L, 3);
        lua_settable(L, -3);
        lua_pop(L, 1);
        return 0;
    }

    static int lua_UILabel_mtGC(lua_State* L)
    {
        Label* self = CheckLabel(L, 1);
        self->~Label();
        return 0;
    }

    static int lua_UILabel_mtToString(lua_State* L)
    {
        Label* self = CheckLabel(L, 1);
        std::stringstream ss;
        ss << "UILabel " << std::hex << self;
        std::string result = ss.str();
        lua_pushstring(L, result.c_str());
        return 1;
    }

    Button* CheckButton(lua_State* L, int index)
    {
        if(!lua_isuserdata(L, index))
        {
            luaL_error(L, "%s: Not a userdata", __FUNCTION__);
            return nullptr;
        }

        Button* btn = (Button*)lua_touserdata(L, index);

        GetRTTI(L, index);
        bool isButton = CheckRTTI(L, IS_BUTTON);
        lua_pop(L, 1);
        if(!isButton)
        {
            luaL_error(L, "%s: Not instance of %s", __FUNCTION__, IS_BUTTON);
            return nullptr;
        }
        return btn;
    }

    Label* CheckLabel(lua_State* L, int index)
    {
        if(!lua_isuserdata(L, index))
        {
            luaL_error(L, "%s: Not a userdata", __FUNCTION__);
            return nullptr;
        }

        Label* label = (Label*)lua_touserdata(L, index);

        GetRTTI(L, index); // rtti{}
        bool isLabel = CheckRTTI(L, IS_LABEL);
        lua_pop(L, 1); // pop RTTI table
        if(!isLabel)
        {
            luaL_error(L, "%s: Not instance of %s", __FUNCTION__, IS_LABEL);
            return nullptr;
        }
        return label;   
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
        lua_pop(L, 1); // pop RTTI table
        if(!isObject)
        {
            luaL_error(L, "%s: Not instance of %s", __FUNCTION__, IS_OBJECT);
            return nullptr;
        }
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
        bool isFrame = CheckRTTI(L, IS_FRAME);
        lua_pop(L, 1); // pop RTTI table
        if(!isFrame)
        {
            luaL_error(L, "%s: Not instance of %s", __FUNCTION__, IS_FRAME);
            return nullptr;
        }
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

    // lua : LoadFont(alias, path)
    static int lua_LoadFont(lua_State* L)
    {
        const char* alias = luaL_checkstring(L, 1);
        const char* path = luaL_checkstring(L, 2);
        int size = (int)luaL_checkinteger(L, 3);
        GameEngine::Get().GetTextRenderer().LoadFont(path, alias, size);
        return 0;
    }

#define BIND_METHOD(c,m) lua_pushstring(L, #m); lua_pushcfunction(L, lua_ ## c ## _ ## m); lua_settable(L, -3);

    LuaBindings::LuaBindings(lua_State* L)
    {
        luastate_ = L;
        // register data
        lua_pushstring(L, "ClickedEventCallbacks");
        lua_newtable(L);
        lua_settable(L, LUA_REGISTRYINDEX);
        lua_pushstring(L, "HoverEventCallbacks");
        lua_newtable(L);
        lua_settable(L, LUA_REGISTRYINDEX);
        // set LoadTexture(alias,path)
        lua_pushcfunction(L, lua_LoadTexture);
        lua_setglobal(L, "LoadTexture");
        // set LoadFont(alias, path, size)
        lua_pushcfunction(L, lua_LoadFont);
        lua_setglobal(L, "LoadFont");
        // UIObject
        lua_newtable(L);
        BIND_METHOD(UIObject, AddOnClicked);
        BIND_METHOD(UIObject, AddOnHover);
        BIND_METHOD(UIObject, ContainsPoint);
        BIND_METHOD(UIObject, IsVisible);
        BIND_METHOD(UIObject, SetVisible);
        BIND_METHOD(UIObject, GetWidth);
        BIND_METHOD(UIObject, SetWidth);
        BIND_METHOD(UIObject, GetHeight);
        BIND_METHOD(UIObject, SetHeight);
        BIND_METHOD(UIObject, GetXPos);
        BIND_METHOD(UIObject, SetXPos);
        BIND_METHOD(UIObject, GetYPos);
        BIND_METHOD(UIObject, SetYPos);
        lua_setglobal(L, "UIObject");
        // UIButton
        lua_newtable(L);
        BIND_METHOD(UIButton, New);
        BIND_METHOD(UIButton, SetText);
        BIND_METHOD(UIButton, SetTextColor);
        lua_setglobal(L, "UIButton");
        // UIFrame
        lua_newtable(L);
        BIND_METHOD(UIFrame, New);
        BIND_METHOD(UIFrame, GetColor);
        BIND_METHOD(UIFrame, SetColor);
        BIND_METHOD(UIFrame, GetBorderColor);
        BIND_METHOD(UIFrame, SetBorderColor);
        BIND_METHOD(UIFrame, GetBorderSize);
        BIND_METHOD(UIFrame, SetBorderSize);
        lua_setglobal(L, "UIFrame");
        // UILabel
        lua_newtable(L);
        BIND_METHOD(UILabel, New);
        BIND_METHOD(UILabel, SetText);
        lua_setglobal(L, "UILabel");
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
        // create UIButton's metatable
        luaL_newmetatable(L, IS_BUTTON);
        lua_pushstring(L, "classes");
        lua_newtable(L);
        lua_pushstring(L, IS_BUTTON);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_pushstring(L, IS_FRAME);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_pushstring(L, IS_OBJECT);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_settable(L, -3);
        lua_pushstring(L, "__index");
        lua_pushcfunction(L, lua_UIButton_mtIndex);
        lua_settable(L, -3);
        lua_pushstring(L, "__newindex");
        lua_pushcfunction(L, lua_UIButton_mtNewIndex);
        lua_settable(L, -3);
        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, lua_UIButton_mtGC);
        lua_settable(L, -3);
        lua_pop(L, 1);
        // create UILabel's metatable
        luaL_newmetatable(L, IS_LABEL);
        lua_pushstring(L, "classes");
        lua_newtable(L);
        lua_pushstring(L, IS_OBJECT);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_pushstring(L, IS_LABEL);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_settable(L, -3);
        lua_pushstring(L, "__index");
        lua_pushcfunction(L, lua_UILabel_mtIndex);
        lua_settable(L, -3);
        lua_pushstring(L, "__newindex");
        lua_pushcfunction(L, lua_UILabel_mtNewIndex);
        lua_settable(L, -3);
        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, lua_UILabel_mtGC);
        lua_settable(L, -3);
        lua_pushstring(L, "__tostring");
        lua_pushcfunction(L, lua_UILabel_mtToString);
        lua_settable(L, -3);
        lua_pop(L, 1);
    }

    LuaBindings::~LuaBindings()
    {
        lua_pushstring(luastate_, "ClickedEventCallbacks");
        lua_pushnil(luastate_);
        lua_settable(luastate_, LUA_REGISTRYINDEX);
        lua_pushstring(luastate_, "HoverEventCallbacks");
        lua_pushnil(luastate_);
        lua_settable(luastate_, LUA_REGISTRYINDEX);
    }

}}
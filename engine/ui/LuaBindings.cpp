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
#include "Slider.h"
#include "TextField.h"

namespace engine { namespace ui {

    // keys for RTTI table
    static const char* RTTI_TABLE_KEY = "classes";
    static const char* IS_BUTTON = "UIButton";
    static const char* IS_FRAME = "UIFrame";
    static const char* IS_LABEL = "UILabel";
    static const char* IS_OBJECT = "UIObject"; 
    static const char* IS_SLIDER = "UISlider";
    static const char* IS_TEXTFIELD = "UITextField";

    // general functions
    static int lua_ErrorHandler(lua_State* L);
    static void GetRTTI(lua_State* L, int index);
    static bool CheckRTTI(lua_State* L, const char* className);
    Button* CheckButton(lua_State* L, int index);
    Label* CheckLabel(lua_State* L, int index);
    Object* CheckObject(lua_State* L, int index);
    Frame* CheckFrame(lua_State* L, int index);
    Slider* CheckSlider(lua_State* L, int index);
    TextField* CheckTextField(lua_State* L, int index);

    // error handling function for running event callbacks
    static int lua_ErrorHandler(lua_State* L)
    {
        const char* errorMsg = lua_tostring(L, 1);
        GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, "%s", errorMsg);
        return 0;
    }

    // Places RTTI table on to stack
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

    // Checks RTTI table at stack = -1
    static bool CheckRTTI(lua_State* L, const char* className)
    {
        lua_pushstring(L, className);
        lua_gettable(L, -2);
        bool isInstance = !lua_isnil(L, -1);
        lua_pop(L, 1); // pop nil or true from stack
        return isInstance;
    }

// UIObject ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

    // lua : UIObject.AddOnClicked(self, someFunction)
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

    // lua : UIObject.AddOnHover(self, someFunction)
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

    static int lua_UIObject_AddOnKeypressed(lua_State* L)
    {
        static lua_Integer counter = 0;
        Object* self = CheckObject(L, 1);
        luaL_checktype(L, 2, LUA_TFUNCTION);
        lua_pushstring(L, "KeypressedEventCallbacks");
        lua_gettable(L, LUA_REGISTRYINDEX);
        counter++;
        lua_Integer current = counter;
        lua_pushinteger(L, counter);
        lua_pushvalue(L, 2);
        lua_settable(L, -3);
        lua_pop(L, 1);
        KeypressedEventCallback cb = [L, current](const KeypressedEvent& e) {
            lua_pushcfunction(L, lua_ErrorHandler);
            lua_pushstring(L, "KeypressedEventCallbacks");
            lua_gettable(L, LUA_REGISTRYINDEX);
            lua_pushinteger(L, current);
            lua_gettable(L, -2);
            lua_remove(L, -2);
            lua_pushinteger(L, e.keyCode);
            lua_pushinteger(L, e.scanCode);
            lua_pushinteger(L, e.mod);
            lua_pushboolean(L, e.repeated);
            lua_pcall(L, 4, 0, -6);
            lua_pop(L, 1);
        };
        self->AddOnKeypressed(cb);
        return 0;
    }

    static int lua_UIObject_AddOnDragged(lua_State* L)
    {
        static lua_Integer counter = 0;
        Object* self = CheckObject(L, 1);
        luaL_checktype(L, 2, LUA_TFUNCTION);
        lua_pushstring(L, "DraggedEventCallbacks");
        lua_gettable(L, LUA_REGISTRYINDEX);
        counter++;
        lua_Integer current = counter;
        lua_pushinteger(L, counter);
        lua_pushvalue(L, 2);
        lua_settable(L, -3);
        lua_pop(L, 1);
        DraggedEventCallback cb = [L, current](const DraggedEvent& e) {
            lua_pushcfunction(L, lua_ErrorHandler);
            lua_pushstring(L, "DraggedEventCallbacks");
            lua_gettable(L, LUA_REGISTRYINDEX);
            lua_pushinteger(L, current);
            lua_gettable(L, -2);
            lua_remove(L, -2);
            lua_pushinteger(L, e.x);
            lua_pushinteger(L, e.y);
            lua_pushinteger(L, e.xrel);
            lua_pushinteger(L, e.yrel);
            lua_pcall(L, 4, 0, -6);
            lua_pop(L, 1);
        };
        self->AddOnDragged(cb);
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

// UIFrame ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

    // lua : UIFrame.New(parent,width,height,xpos,ypos,'texture')
    static int lua_UIFrame_New(lua_State* L)
    {
        Object* parent;
        if(lua_isnil(L, 1))
            parent = Root::Get();
        else
            parent = CheckObject(L, 1);
        
        int width = (int)lua_tonumber(L, 2);
        int height = (int)lua_tonumber(L, 3);
        int xpos = (int)lua_tonumber(L, 4);
        int ypos = (int)lua_tonumber(L, 5);
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

    // lua : r,g,b,a = UIFrame.GetColor()
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
        // GameEngine::Get().GetLogger().Logf(Logger::Severity::INFO, "__gc on UIFrame %llx", frame);
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

// UIButton ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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

// UILabel ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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

    static int lua_UILabel_GetText(lua_State* L)
    {
        Label* self = CheckLabel(L, 1);
        lua_pushstring(L, self->GetText().c_str());
        return 1;
    }

    static int lua_UILabel_SetText(lua_State* L)
    {
        Label* self = CheckLabel(L, 1);
        std::string text = luaL_checkstring(L, 2);
        self->SetText(text);
        return 0;
    }

    static int lua_UILabel_GetColor(lua_State* L)
    {
        Label* self = CheckLabel(L, 1);
        Color c = self->GetColor();
        lua_pushnumber(L, c.r);
        lua_pushnumber(L, c.g);
        lua_pushnumber(L, c.b);
        lua_pushnumber(L, c.a);
        return 4;
    }

    static int lua_UILabel_SetColor(lua_State* L)
    {
        Label* self = CheckLabel(L, 1);
        Color c;
        c.r = (float)lua_tonumber(L, 2);
        c.g = (float)lua_tonumber(L, 3);
        c.b = (float)lua_tonumber(L, 4);
        c.a = (float)lua_tonumber(L, 5);
        self->SetColor(c);
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

// UISlider ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

    static int lua_UISlider_New(lua_State* L)
    {
        Object* parent;
        if(lua_isnil(L, 1))
            parent = Root::Get();
        else
            parent = CheckObject(L, 1);
        
        int width = (int)luaL_checkinteger(L, 2);
        int height = (int)luaL_checkinteger(L, 3);
        const char* s = luaL_checkstring(L, 4);
        const char* textureAlias = luaL_checkstring(L, 5);

        Slider::ORIENTATION orientation = strcmp(s, "horizontal") == 0 ? 
                Slider::ORIENTATION::HORIZONTAL
                : Slider::ORIENTATION::VERTICAL;

        ogl::Texture* texture = GameEngine::Get().GetTextureManager().GetTexture(textureAlias);

        Slider* slider = (Slider*)lua_newuserdata(L, sizeof(Slider));
        luaL_setmetatable(L, IS_SLIDER);
        slider = new (slider) Slider(parent, width, height, orientation, texture);
        lua_newtable(L);
        lua_pushstring(L, "parent");
        lua_pushvalue(L, 1);
        lua_settable(L, -3);
        lua_setuservalue(L, -2);
        return 1;
    }

    static int lua_UISlider_GetOrientation(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        const char* ori = self->GetOrientation() == Slider::ORIENTATION::HORIZONTAL ?
                "horizontal" : "vertical";
        lua_pushstring(L, ori);
        return 1;
    }

    static int lua_UISlider_GetSlideColor(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        Color c = self->GetSlideColor();
        lua_pushnumber(L, c.r);
        lua_pushnumber(L, c.g);
        lua_pushnumber(L, c.b);
        lua_pushnumber(L, c.a);
        return 4;
    }

    static int lua_UISlider_SetSlideColor(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        float r = (float)luaL_checknumber(L, 2);
        float g = (float)luaL_checknumber(L, 3);
        float b = (float)luaL_checknumber(L, 4);
        float a = (float)luaL_checknumber(L, 5);
        self->SetSlideColor({r,g,b,a});
        return 0;
    }

    static int lua_UISlider_GetKnobColor(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        Color c = self->GetKnobColor();
        lua_pushnumber(L, c.r);
        lua_pushnumber(L, c.g);
        lua_pushnumber(L, c.b);
        lua_pushnumber(L, c.a);
        return 4;
    }

    static int lua_UISlider_SetKnobColor(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        float r = (float)luaL_checknumber(L, 2);
        float g = (float)luaL_checknumber(L, 3);
        float b = (float)luaL_checknumber(L, 4);
        float a = (float)luaL_checknumber(L, 5);
        self->SetKnobColor({r,g,b,a});
        return 0;    
    }

    static int lua_UISlider_GetValue(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        lua_pushnumber(L, self->GetValue());
        return 1;
    }

    static int lua_UISlider_SetValue(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        double value = luaL_checknumber(L, 2);
        self->SetValue(value);
        return 0;
    }

    static int lua_UISlider_mtIndex(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        lua_getuservalue(L, 1);
        lua_pushvalue(L, 2);
        lua_gettable(L, -2);
        lua_remove(L, -2);
        if(lua_isnil(L, -1))
        {
            // check UIButton
            lua_pop(L, 1);
            lua_getglobal(L, "UISlider");
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

    static int lua_UISlider_mtNewIndex(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        lua_getuservalue(L, 1);
        lua_pushvalue(L, 2);
        lua_pushvalue(L, 3);
        lua_settable(L, -3);
        lua_pop(L, 1);
        return 0;
    }

    static int lua_UISlider_mtGC(lua_State* L)
    {
        Slider* self = CheckSlider(L, 1);
        self->~Slider();
        return 0;
    }

// UITextField ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

    static int lua_UITextField_New(lua_State* L)
    {
        Object* parent;
        if(lua_isnil(L, 1))
            parent = Root::Get();
        else
            parent = CheckObject(L, 1);
        
        int width = (int)luaL_checkinteger(L, 2);
        int height = (int)luaL_checkinteger(L, 3);
        const char* fontAlias = luaL_checkstring(L, 4);
        const char* textureAlias = luaL_checkstring(L, 5);
        ogl::Texture* texture = GameEngine::Get().GetTextureManager().GetTexture(textureAlias);

        TextField* tf = (TextField*)lua_newuserdata(L, sizeof(TextField));
        luaL_setmetatable(L, IS_TEXTFIELD);
        tf = new (tf) TextField(parent, width, height, fontAlias, texture);
        lua_newtable(L);
        lua_pushstring(L, "parent");
        lua_pushvalue(L, 1);
        lua_settable(L, -3);
        lua_setuservalue(L, -2);
        return 1;
    }

    static int lua_UITextField_GetText(lua_State* L)
    {
        TextField* self = CheckTextField(L, 1);
        std::string text = self->GetText();
        lua_pushstring(L, text.c_str());
        return 1;
    }

    static int lua_UITextField_SetText(lua_State* L)
    {
        TextField* self = CheckTextField(L, 1);
        const char* text = luaL_checkstring(L, 2);
        self->SetText(text);
        return 0;
    }

    static int lua_UITextField_IsEditable(lua_State* L)
    {
        TextField* self = CheckTextField(L, 1);
        lua_pushboolean(L, self->IsEditable());
        return 1;
    }

    static int lua_UITextField_SetEditable(lua_State* L)
    {
        TextField* self = CheckTextField(L, 1);
        bool isEditable = lua_toboolean(L, 2);
        self->SetEditable(isEditable);
        return 0;
    }

    static int lua_UITextField_mtGC(lua_State* L)
    {
        TextField* self = CheckTextField(L, 1);
        self->~TextField();
        return 0;
    }

    static int lua_UITextField_mtIndex(lua_State* L)
    {
        TextField* self = CheckTextField(L, 1);
        lua_getuservalue(L, 1);
        lua_pushvalue(L, 2);
        lua_gettable(L, -2);
        lua_remove(L, -2);
        if(lua_isnil(L, -1))
        {
            // check UIButton
            lua_pop(L, 1);
            lua_getglobal(L, "UITextField");
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

    static int lua_UITextField_mtNewIndex(lua_State* L)
    {
        TextField* self = CheckTextField(L, 1);
        lua_getuservalue(L, 1);
        lua_pushvalue(L, 2);
        lua_pushvalue(L, 3);
        lua_settable(L, -3);
        lua_pop(L, 1);
        return 0;
    }

// check functions ////////////////////////////////////////////////////////////

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

    Slider* CheckSlider(lua_State* L, int index)
    {
        if(!lua_isuserdata(L, index))
        {
            luaL_error(L, "%s: Not a userdata", __FUNCTION__);
            return nullptr;
        }

        Slider* slider = (Slider*)lua_touserdata(L, index);

        GetRTTI(L, index); // rtti{}
        bool isSlider = CheckRTTI(L, IS_SLIDER);
        lua_pop(L, 1); // pop RTTI table
        if(!isSlider)
        {
            luaL_error(L, "%s: Not instance of %s", __FUNCTION__, IS_SLIDER);
            return nullptr;
        }
        return slider;       
    }

    TextField* CheckTextField(lua_State* L, int index)
    {
        if(!lua_isuserdata(L, index))
        {
            luaL_error(L, "%s: Not a userdata", __FUNCTION__);
            return nullptr;
        }

        TextField* tf = (TextField*)lua_touserdata(L, index);

        GetRTTI(L, index); // rtti{}
        bool isTextField = CheckRTTI(L, IS_TEXTFIELD);
        lua_pop(L, 1); // pop RTTI table
        if(!isTextField)
        {
            luaL_error(L, "%s: Not instance of %s", __FUNCTION__, IS_TEXTFIELD);
            return nullptr;
        }
        return tf;        
    }

// global functions ///////////////////////////////////////////////////////////

    // lua : GetScreenWidth()
    static int lua_GetScreenWidth(lua_State* L)
    {
        lua_pushinteger(L, GameEngine::Get().GetWidth());
        return 1;
    }

    // lua : GetScreenHeight()
    static int lua_GetScreenHeight(lua_State* L)
    {
        lua_pushinteger(L, GameEngine::Get().GetHeight());
        return 1;
    }

    // lua : LoadTexture(alias, path)
    static int lua_LoadTexture(lua_State* L)
    {
        const char* path = luaL_checkstring(L, 1);
        GameEngine::Get().GetTextureManager().LoadTexture(path);
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

    // runs lua files from the current working directory of host app
    static int lua_RunFile(lua_State* L)
    {
        const char* path = luaL_checkstring(L, 1);
        int errCode = luaL_dofile(L, path);
        if(errCode != LUA_OK)
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, 
                    "%s: %s", __FUNCTION__, lua_tostring(L, -1));
            lua_pop(L, 1);
        }
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
        lua_pushstring(L, "KeypressedEventCallbacks");
        lua_newtable(L);
        lua_settable(L, LUA_REGISTRYINDEX);
        lua_pushstring(L, "DraggedEventCallbacks");
        lua_newtable(L);
        lua_settable(L, LUA_REGISTRYINDEX);
        // set global functions
        lua_pushcfunction(L, lua_LoadTexture);
        lua_setglobal(L, "LoadTexture");
        lua_pushcfunction(L, lua_LoadFont);
        lua_setglobal(L, "LoadFont");
        lua_pushcfunction(L, lua_GetScreenWidth);
        lua_setglobal(L, "GetScreenWidth");
        lua_pushcfunction(L, lua_GetScreenHeight);
        lua_setglobal(L, "GetScreenHeight");
        lua_pushcfunction(L, lua_RunFile);
        lua_setglobal(L, "RunFile");
        // UIObject
        lua_newtable(L);
        BIND_METHOD(UIObject, AddOnClicked);
        BIND_METHOD(UIObject, AddOnHover);
        BIND_METHOD(UIObject, AddOnKeypressed);
        BIND_METHOD(UIObject, AddOnDragged);
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
        BIND_METHOD(UILabel, GetText);
        BIND_METHOD(UILabel, SetText);
        BIND_METHOD(UILabel, GetColor);
        BIND_METHOD(UILabel, SetColor);
        lua_setglobal(L, "UILabel");
        // UISlider
        lua_newtable(L);
        BIND_METHOD(UISlider, New);
        BIND_METHOD(UISlider, GetOrientation);
        BIND_METHOD(UISlider, GetSlideColor);
        BIND_METHOD(UISlider, SetSlideColor);
        BIND_METHOD(UISlider, GetKnobColor);
        BIND_METHOD(UISlider, SetKnobColor);
        BIND_METHOD(UISlider, GetValue);
        BIND_METHOD(UISlider, SetValue);
        lua_setglobal(L, "UISlider");
        // UITextField
        lua_newtable(L);
        BIND_METHOD(UITextField, New);
        BIND_METHOD(UITextField, GetText);
        BIND_METHOD(UITextField, SetText);
        BIND_METHOD(UITextField, IsEditable);
        BIND_METHOD(UITextField, SetEditable);
        lua_setglobal(L, "UITextField");
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
        // create UISlider's metatable
        luaL_newmetatable(L, IS_SLIDER);
        lua_pushstring(L, "classes");
        lua_newtable(L);
        lua_pushstring(L, IS_OBJECT);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_pushstring(L, IS_FRAME);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_pushstring(L, IS_SLIDER);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_settable(L, -3);
        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, lua_UISlider_mtGC);
        lua_settable(L, -3);
        lua_pushstring(L, "__index");
        lua_pushcfunction(L, lua_UISlider_mtIndex);
        lua_settable(L, -3);
        lua_pushstring(L, "__newindex");
        lua_pushcfunction(L, lua_UISlider_mtNewIndex);
        lua_settable(L, -3);
        lua_pop(L, 1);
        // create UITextField's metatable
        luaL_newmetatable(L, IS_TEXTFIELD);
        lua_pushstring(L, "classes");
        lua_newtable(L);
        lua_pushstring(L, IS_OBJECT);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_pushstring(L, IS_FRAME);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_pushstring(L, IS_TEXTFIELD);
        lua_pushboolean(L, 1);
        lua_settable(L, -3);
        lua_settable(L, -3);
        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, lua_UITextField_mtGC);
        lua_settable(L, -3);
        lua_pushstring(L, "__index");
        lua_pushcfunction(L, lua_UITextField_mtIndex);
        lua_settable(L, -3);
        lua_pushstring(L, "__newindex");
        lua_pushcfunction(L, lua_UITextField_mtNewIndex);
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
        lua_pushstring(luastate_, "KeypressedEventCallbacks");
        lua_pushnil(luastate_);
        lua_settable(luastate_, LUA_REGISTRYINDEX);
        lua_pushstring(luastate_, "DraggedEventCallbacks");
        lua_pushnil(luastate_);
        lua_settable(luastate_, LUA_REGISTRYINDEX);
    }

}}
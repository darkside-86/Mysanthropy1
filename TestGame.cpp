// TestGame.cpp
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

#include "TestGame.h"

#include <chrono>
#include <random>

#include <SDL/SDL.h>

#include "engine/GameEngine.h"
#include "engine/ui/LuaBindings.h"
#include "ogl/ErrorCheck.h"

static constexpr engine::ui::Color BUTTON_OFF_COLOR = {0.2f,0.2f,0.6f,1.f};
static constexpr engine::ui::Color BUTTON_ON_COLOR = {0.6f, 0.2f, 0.2f, 1.f};

TestGame::TestGame() : groundTexture("res/textures/ground.jpg", true),
                        frameTexture("res/textures/frame.png", false)
{
    engine::GameEngine::Get().GetTextureManager().LoadTexture("uiblank", "res/textures/uiblank.png");
    ogl::Texture* uiblank = engine::GameEngine::Get().GetTextureManager().GetTexture("uiblank");
    engine::TextRenderer& tr = engine::GameEngine::Get().GetTextRenderer();
    tr.LoadFont("res/fonts/UbuntuMono-Regular.ttf", "mono", 18);
    tr.SetFGColor(1.0f, 1.0f, 1.0f, 1.0f);
    tr.SetBGColor(0.0f, 0.5f, 0.0f, 1.0f);

    uiRoot_ = engine::ui::Root::Get();
    frame_ = new engine::ui::Frame(uiRoot_, 400, 300, 25, 25, uiblank, {0.f,0.f,1.f,1.f});
    blueButton_ = new engine::ui::Frame(frame_, 30, 30, 4, 5, uiblank, BUTTON_OFF_COLOR);
    helloLabel_ = new engine::ui::Label(blueButton_, "Hi", "mono", {0.4f,8.f,6.f,1.f});
    closeButton_ = new engine::ui::Button(frame_, uiblank, {0.5f,0.f,0.5f,1.f}, "X", "mono", {0.f,1.f,0.f,1.f},6);
    slider_ = new engine::ui::Slider(frame_, 200, 50, engine::ui::Slider::ORIENTATION::HORIZONTAL, uiblank);
    textField_ = new engine::ui::TextField(frame_, 300, 60, "mono", uiblank);
    
    testObject_ = new TestObject(uiblank);

    scripting_ = luaL_newstate();
    luaL_openlibs(scripting_);
    luaBindings_ = new engine::ui::LuaBindings(scripting_);
    engine::GameEngine::Get().GetTextureManager().UnloadTexture("uiblank");
    std::vector<const char*> CORE_UI_LIB = {
        "ui/keycodes.lua", "ui/fonts.lua", "ui/window.lua", "ui/main.lua"
    };
    try 
    {
        int errCode = 0;
        for(auto luaFile : CORE_UI_LIB)
        {
            errCode = luaL_dofile(scripting_, luaFile);
            if(errCode != 0) throw errCode;
        }
    }
    catch(int numErrors)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "Lua error %d: %s", numErrors, lua_tostring(scripting_, -1));
        lua_pop(scripting_, 1);
    }

    lua_pushstring(scripting_, "TestGame");
    lua_pushlightuserdata(scripting_, this);
    lua_settable(scripting_, LUA_REGISTRYINDEX);

    lua_pushcfunction(scripting_, TestGame::SetR);
    lua_setglobal(scripting_, "TestGame_SetR");
    lua_pushcfunction(scripting_, TestGame::SetG);
    lua_setglobal(scripting_, "TestGame_SetG");
    lua_pushcfunction(scripting_, TestGame::SetB);
    lua_setglobal(scripting_, "TestGame_SetB");
    lua_pushcfunction(scripting_, TestGame::SetA);
    lua_setglobal(scripting_, "TestGame_SetA");
    lua_pushcfunction(scripting_, TestGame::SetAngle);
    lua_setglobal(scripting_, "TestGame_SetAngle");
}

TestGame::~TestGame()
{
	delete helloLabel_;
    delete blueButton_;
    delete frame_;
    delete slider_;
    delete textField_;
    delete uiRoot_;

    delete testObject_;

    delete luaBindings_;
    lua_close(scripting_);
}

bool TestGame::Initialize()
{
    engine::GameEngine::Get().AddMouseButtonListener([this](const SDL_MouseButtonEvent& e){
        this->uiRoot_->ProcessMouseButtonEvent(e);
    });

    engine::GameEngine::Get().AddMouseMotionListener([this](const SDL_MouseMotionEvent& e) {
        this->uiRoot_->ProcessMouseMotionEvent(e);
    });

    engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e) {
        this->uiRoot_->ProcessKeyboardEvent(e);
    });

    frame_->SetBorderColor({0.5f,0.f,0.f,1.f});
    frame_->SetBorderSize(3);

    helloLabel_->SetXPos(blueButton_->GetWidth() / 2 - helloLabel_->GetWidth() / 2);    
    helloLabel_->SetYPos(blueButton_->GetHeight() / 2 - helloLabel_->GetHeight() / 2);

    blueButton_->SetBorderColor({0.f,0.1f,0.f,1.f});
    blueButton_->SetBorderSize(1);

    closeButton_->SetBorderSize(2);
    closeButton_->SetBorderColor({0.f,0.f,0.f,1.f});
    closeButton_->SetXPos(frame_->GetWidth() - closeButton_->GetWidth() - 5);
    closeButton_->SetYPos(5);

    slider_->SetColor({1.f, 1.f, 1.f, 0.2f});
    slider_->SetKnobColor({1.f, 0.f, 1.f, 1.f});
    slider_->SetSlideColor({1.f,0.5f,1.f,1.f});

    textField_->SetYPos(60);
    textField_->SetText("Hello \nworld\nyo");

    frame_->AddOnDragged([this](const engine::ui::DraggedEvent& e){
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
                "Dragged frame: %d, %d, %d, %d", e.x, e.y, e.xrel, e.yrel);
                this->frame_->SetXPos(this->frame_->GetXPos() + e.xrel);
                this->frame_->SetYPos(this->frame_->GetYPos() + e.yrel);
    });

    blueButton_->AddOnHover([this](const engine::ui::HoverEvent& e){
        if(e.over)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
                    "Hovered over button!");
            this->blueButton_->SetColor(BUTTON_ON_COLOR);
        }
        else
        {
            this->blueButton_->SetColor(BUTTON_OFF_COLOR);
        }
    });

    blueButton_->AddOnClicked([this](const engine::ui::ClickedEvent&) {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
                "Button was clicked!");
        this->RandomizeRectColors();
    } );

    blueButton_->AddOnKeypressed([this](const engine::ui::KeypressedEvent& e){
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
                "Key was pressed on blueButton_, %c, %d", e.keyCode, e.mod);
    });

    closeButton_->AddOnClicked([this](const engine::ui::ClickedEvent&){
        this->frame_->SetVisible(false);
    });

    slider_->AddOnClicked([this](const engine::ui::ClickedEvent& e){
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                "Value is %f", slider_->GetValue());
    });

    frame_->SetVisible(false);

    testObject_->GetVertices()[1].color = {255,255,255,255};
    testObject_->SetVerticesData();

    return true;
}

void TestGame::Cleanup()
{
}

void TestGame::Update(float dtime)
{

}

void TestGame::Render(engine::GraphicsContext& gc)
{
    static bool isInit = false;
    if(!isInit)
    {
        gc.SetUseColorBlending(true);
        gc.SetUseTexture(true);
        isInit = true;
    }

    glEnable(GL_DEPTH_TEST);
    float width = (float)engine::GameEngine::Get().GetWidth();
    float height = (float)engine::GameEngine::Get().GetHeight();
    gc.ResetMVP();
    gc.TranslateModel(0.f, 0.1f, -2.1f);
    gc.RotateModel(angle, 0.f, 1.f, 0.f);
    gc.SetPerspectiveProjection(45.f, width/height, 0.01f, 45.f);
    gc.SetMVP();
    testObject_->Render(gc);
    OGL_ERROR_CHECK();

    glDisable(GL_DEPTH_TEST);
    uiRoot_->Render(gc);
}

void TestGame::RandomizeRectColors()
{
    auto& rng = engine::GameEngine::Get().GetRNG();
    float red = (float)rng() / (float)rng.max();
    float green = (float)rng() / (float)rng.max();
    float blue = (float)rng() / (float)rng.max();
    constexpr float alpha = 1.0f;
    frame_->SetColor({red,green,blue,alpha});    
}

int TestGame::SetR(lua_State* L)
{
    double value = lua_tonumber(L, 1);
    lua_pushstring(L, "TestGame");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TestGame* tg = (TestGame*)lua_touserdata(L, -1);
    lua_pop(L,1);
    tg->testObject_->GetVertices()[0].color.r = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[1].color.r = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[2].color.r = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[3].color.r = (unsigned char)(255.f * value);
    tg->testObject_->SetVerticesData();
    return 0;
}

int TestGame::SetG(lua_State* L)
{
    double value = lua_tonumber(L, 1);
    lua_pushstring(L, "TestGame");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TestGame* tg = (TestGame*)lua_touserdata(L, -1);
    lua_pop(L,1);
    tg->testObject_->GetVertices()[0].color.g = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[1].color.g = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[2].color.g = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[3].color.g = (unsigned char)(255.f * value);
    tg->testObject_->SetVerticesData();
    return 0;
}

int TestGame::SetB(lua_State* L)
{
    double value = lua_tonumber(L, 1);
    lua_pushstring(L, "TestGame");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TestGame* tg = (TestGame*)lua_touserdata(L, -1);
    lua_pop(L,1);
    tg->testObject_->GetVertices()[0].color.b = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[1].color.b = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[2].color.b = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[3].color.b = (unsigned char)(255.f * value);
    tg->testObject_->SetVerticesData();
    return 0;
}

int TestGame::SetA(lua_State* L)
{
    double value = lua_tonumber(L, 1);
    lua_pushstring(L, "TestGame");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TestGame* tg = (TestGame*)lua_touserdata(L, -1);
    lua_pop(L,1);
    tg->testObject_->GetVertices()[0].color.a = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[1].color.a = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[2].color.a = (unsigned char)(255.f * value);
    tg->testObject_->GetVertices()[3].color.a = (unsigned char)(255.f * value);
    tg->testObject_->SetVerticesData();
    return 0;
}

int TestGame::SetAngle(lua_State* L)
{
    double value = lua_tonumber(L, 1);
    lua_pushstring(L, "TestGame");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TestGame* tg = (TestGame*)lua_touserdata(L, -1);
    lua_pop(L,1);
    tg->angle = (360.0) * value;
    return 0;
}

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

    scripting_ = luaL_newstate();
    luaL_openlibs(scripting_);
    luaBindings_ = new engine::ui::LuaBindings(scripting_);
    engine::GameEngine::Get().GetTextureManager().UnloadTexture("uiblank");
    try 
    {
        int errCode = 0;
        errCode = luaL_dofile(scripting_, "ui/keycodes.lua");
        if(errCode != 0) throw errCode;
        errCode = luaL_dofile(scripting_, "ui/test.lua");
        if(errCode != 0) throw errCode;
    }
    catch(int numErrors)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "Lua error %d: %s", numErrors, lua_tostring(scripting_, -1));
    }
}

TestGame::~TestGame()
{
	delete helloLabel_;
    delete blueButton_;
    delete frame_;
    delete uiRoot_;

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
    // 3D rendering here
    glDisable(GL_DEPTH_TEST);
    uiRoot_->Render(gc);
    gc.ResetModel();
    gc.SetMVP();
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

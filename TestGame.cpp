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
#include "ogl/ErrorCheck.h"

TestGame::TestGame() : groundTexture("res/textures/ground.jpg", true),
                        frameTexture("res/textures/frame.png", false)
{
    engine::GameEngine::Get().GetTextureManager().LoadTexture("uiblank", "res/textures/uiblank.png");
    ogl::Texture* uiblank = engine::GameEngine::Get().GetTextureManager().GetTexture("uiblank");
    uiRoot_ = new engine::ui::Root();
    frame_ = new engine::ui::Frame(uiRoot_, 400, 300, 25, 25, uiblank, {0.f,0.f,1.f,1.f});
    blueButton_ = new engine::ui::Frame(frame_, 30, 30, 4, 5, uiblank, {0.2f, 0.2f, 0.6f, 1.f});
}

TestGame::~TestGame()
{
    delete blueButton_;
    delete frame_;
    delete uiRoot_;
}

bool TestGame::Initialize()
{
    engine::GameEngine::Get().AddMouseButtonListener([this](const SDL_MouseButtonEvent& e){
        uiRoot_->ProcessMouseButtonEvent(e);
    });

    blueButton_->AddOnClicked([this](const engine::ui::ClickedEvent&) {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
                "Button was clicked!");
        this->RandomizeRectColors();
    } );

    engine::TextRenderer& tr = engine::GameEngine::Get().GetTextRenderer();
    tr.LoadFont("res/fonts/UbuntuMono-Regular.ttf", "mono", 18);
    tr.SetFGColor(1.0f, 1.0f, 1.0f, 1.0f);
    tr.SetBGColor(0.0f, 0.5f, 0.0f, 1.0f);
    messageTexture_ = tr.RenderTextShaded("mono", "Hello");
    messageRect_ = new engine::RectangleShape(0.0f, 0.0f, 
            (float)messageTexture_->GetWidth(), (float)messageTexture_->GetHeight());
    if(messageTexture_ == nullptr)
        return false;
    return true;
}

void TestGame::Cleanup()
{
    delete messageTexture_;
    delete messageRect_;
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
    glDisable(GL_DEPTH_TEST);
    uiRoot_->Render(gc);
    gc.ResetModel();
    gc.SetMVP();
    messageTexture_->Bind();
    messageRect_->Render(gc);
    glEnable(GL_DEPTH_TEST);
    // 3D rendering here
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

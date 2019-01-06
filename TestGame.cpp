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
    uiRoot_ = new engine::ui::Root();
    frame_ = new engine::ui::Frame(uiRoot_, 400, 300, 25, 25, &frameTexture, {0.f,0.f,1.f,1.f});
}

TestGame::~TestGame()
{
    delete uiRoot_;
}

bool TestGame::Initialize()
{
    RandomizeRectColors();

    engine::GameEngine::Get().AddMouseButtonListener([this](const SDL_MouseButtonEvent& e){
        uiRoot_->ProcessMouseButtonEvent(e);
    });

    frame_->AddOnClicked([this](const engine::ui::ClickedEvent&) {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
                "Frame was clicked!");
        this->frame_->SetColor({1.f,0.f,0.f,1.f});
    } );

    engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e){
        if(e.type == SDL_KEYDOWN)
        {
            if(e.keysym.sym == SDLK_r)
            {
                this->RandomizeRectColors();
            }
        }
    });

    engine::GameEngine::Get().GetTextRenderer().LoadFont("res/fonts/UbuntuMono-Regular.ttf", "mono", 18);
    engine::GameEngine::Get().GetTextRenderer().SetFGColor(1.0f, 1.0f, 1.0f, 1.0f);
    engine::GameEngine::Get().GetTextRenderer().SetBGColor(0.0f, 0.5f, 0.0f, 1.0f);
    messageTexture_ = engine::GameEngine::Get().GetTextRenderer().RenderTextShaded("mono", "Hello");
    messageRect_ = new engine::RectangleShape(0.0f, 0.0f, 
            (float)messageTexture_->GetWidth(), (float)messageTexture_->GetHeight());

    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
            "The width and height are %d, %d", 
            messageTexture_->GetWidth(), messageTexture_->GetHeight());
    return true;
}

void TestGame::Cleanup()
{
    delete messageRect_;
    delete messageTexture_;
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
    OGL_ERROR_CHECK();
    gc.ResetMVP();
    gc.SetOrthoProjection();
    gc.SetMVP();
    groundTexture.Bind();
    uiRoot_->Render(gc);
    gc.ResetModel();
    gc.SetMVP();
    messageTexture_->Bind();
    messageRect_->Render(gc);
    glEnable(GL_DEPTH_TEST);
}

void TestGame::RandomizeRectColors()
{
    /*auto& rng = engine::GameEngine::Get().GetRNG();
    for(int i=0; i < rect.GetNumVertices(); ++i)
    {
        rect.GetVertices()[i].color = {
            (unsigned char) (0xff & (rng() % 256)), 
            (unsigned char) (0xff & (rng() % 256)), 
            (unsigned char) (0xff & (rng() % 256)), 
            255
        };
    }
    rect.ModifyVertices();*/
}

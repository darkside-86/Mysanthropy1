// TileGame.cpp
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
#include "TileGame.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.h"

TileGame::TileGame()
{
    tileMap_ = new TileMap("res/tilemaps/island.bin");
    spriteImage_ = new ogl::Texture("res/textures/sprites/hr_front_stand.png", true, false);
    testSprite_ = new Sprite(spriteImage_);
}

TileGame::~TileGame()
{
    delete tileMap_;
    delete spriteImage_;
    delete testSprite_;
}

bool TileGame::Initialize()
{
    testSprite_->SetPosition({0.f,0.f,0.f});
    engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e){
        if(e.type == SDL_KEYDOWN)
        {
            glm::vec3 vel = testSprite_->GetVelocity();
            switch(e.keysym.sym)
            {
                case SDLK_LEFT: vel.x = -50.f; break;
                case SDLK_RIGHT: vel.x = 50.f; break;
                case SDLK_UP: vel.y = -50.f; break;
                case SDLK_DOWN: vel.y = 50.f; break;
            }
            testSprite_->SetVelocity(vel);
        }
        else 
        {

            testSprite_->SetVelocity({0.f,0.f,0.f});
        }
    });
    return true;
}

void TileGame::Cleanup()
{

}

void TileGame::Update(float dtime)
{
    testSprite_->Update(dtime);
}

void TileGame::Render(engine::GraphicsContext& gc)
{
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    ogl::Program& program = gc.GetProgram();
    program.Use();
    gc.SetUseTexture(true);
    gc.SetUseColorBlending(false);
    glm::mat4 projection(1.f), view(1.f), model(1.f);
    float scrW = (float)engine::GameEngine::Get().GetWidth();
    float scrH = (float)engine::GameEngine::Get().GetHeight();
    projection = glm::ortho(0.f, scrW, scrH, 0.f);
    program.SetUniform<glm::mat4>("u_projection", projection);
    program.SetUniform<glm::mat4>("u_view", view);
    program.SetUniform<glm::mat4>("u_model", model);
    tileMap_->Render(0,0, program);
    testSprite_->Render(program);
}

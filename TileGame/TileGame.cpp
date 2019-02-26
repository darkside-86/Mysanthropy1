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
    loadedEntities_ = tileMap_->GenerateEntities();
}

TileGame::~TileGame()
{
    delete tileMap_;
    CleanupLoadedEntities();
}

bool TileGame::Initialize()
{
    testSprite_ = LoadLGSpr("man1");
    testSprite_->SetPosition({
        23.f * (float)tileMap_->GetTileSet()->GetTileWidth(),
        234.f * (float)tileMap_->GetTileSet()->GetTileHeight(),
        0.f
    });
    testSprite_->SetCollisionBox(4.f, (float)testSprite_->GetWidth() / 2.f, 
            (float)testSprite_->GetWidth()-4.f, (float)testSprite_->GetHeight());

    testSprite_->SetCurrentAnim("front_stand", 0.2f);
    testSprite_->StartAnimation();
    engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e){
        if(e.type == SDL_KEYDOWN)
        {
            glm::vec3 vel = testSprite_->GetVelocity();
            switch(e.keysym.sym)
            {
                case SDLK_LEFT: 
                    vel.x = -50.f;
                    if(testSprite_->GetCurrentAnim() != "left_walk")
                        testSprite_->SetCurrentAnim("left_walk", 0.2f);
                    break;
                case SDLK_RIGHT:
                    if(testSprite_->GetCurrentAnim() != "right_walk")
                        testSprite_->SetCurrentAnim("right_walk", 0.2f); 
                    vel.x = 50.f; 
                    break;
                case SDLK_UP:
                    if(testSprite_->GetCurrentAnim() != "back_walk")
                        testSprite_->SetCurrentAnim("back_walk", 0.2f); 
                    vel.y = -50.f; 
                    break;
                case SDLK_DOWN:
                    if(testSprite_->GetCurrentAnim() != "front_walk")
                        testSprite_->SetCurrentAnim("front_walk", 0.2f); 
                    vel.y = 50.f; 
                    break;
            }
            if(vel.length() != 0)
                testSprite_->StartAnimation();
            testSprite_->SetVelocity(vel);
        }
        else 
        {
            // todo: stop movement based on key up
            testSprite_->PauseAnimation();
            testSprite_->SetVelocity({0.f,0.f,0.f});
        }
    });
    return true;
}

void TileGame::Cleanup()
{
    UnloadLGSpr(testSprite_, "man1");
}

void TileGame::Update(float dtime)
{
    testSprite_->Update(dtime);
    // tile collision detection.
    float left, top, right, bottom;
    testSprite_->GetCollisionBox(left, top, right, bottom);
    // check each corner
    // TODO: fix update to prevent walking through-trick
    bool collision = false;
    int ix = (int)(left / (float)tileMap_->GetTileSet()->GetTileWidth());
    int iy = (int)(top / (float)tileMap_->GetTileSet()->GetTileHeight());
    if(tileMap_->GetCollisionData(ix, iy))  collision = true;
    ix = (int)(right / (float)tileMap_->GetTileSet()->GetTileWidth());
    iy = (int)(top / (float)tileMap_->GetTileSet()->GetTileHeight());
    if(tileMap_->GetCollisionData(ix, iy))  collision = true;
    ix = (int)(left / (float)tileMap_->GetTileSet()->GetTileWidth());
    iy = (int)(bottom / (float)tileMap_->GetTileSet()->GetTileHeight());
    if(tileMap_->GetCollisionData(ix, iy))  collision = true;
    ix = (int)(right / (float)tileMap_->GetTileSet()->GetTileWidth());
    iy = (int)(bottom / (float)tileMap_->GetTileSet()->GetTileHeight());
    if(tileMap_->GetCollisionData(ix, iy))  collision = true;
    if(collision) 
        testSprite_->Update(-dtime);
    
    // todo: bound player to map area

    // calculate camera.
    float scrW = (float)engine::GameEngine::Get().GetWidth();
    float scrH = (float)engine::GameEngine::Get().GetHeight();
    glm::vec3 playerPos = testSprite_->GetPosition();

    camera_.x = playerPos.x - scrW / 2.f;
    camera_.y = playerPos.y - scrH / 2.f;

    // TODO: bound camera to map area
    /*if(camera_.x < 0.0f)
        camera_.x = 0.0f;
    if(camera_.y < 0.0f)
        camera_.y = 0.0f;*/
}

void TileGame::Render(engine::GraphicsContext& gc)
{
    // disable for 2D rendering with semi-transparent data
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    ogl::Program& program = gc.GetProgram();
    program.Use();
    // colors = texture data only
    gc.SetUseTexture(true);
    gc.SetUseColorBlending(false);
    // model view projection matrices
    glm::mat4 projection(1.f), view(1.f), model(1.f);
    float scrW = (float)engine::GameEngine::Get().GetWidth();
    float scrH = (float)engine::GameEngine::Get().GetHeight();
    // screen 0,0 is top left
    projection = glm::ortho(0.f, scrW, scrH, 0.f);
    program.SetUniform<glm::mat4>("u_projection", projection);
    program.SetUniform<glm::mat4>("u_view", view);
    program.SetUniform<glm::mat4>("u_model", model);
    // rendering with camera takes negative camera coordinates
    tileMap_->Render((int)-camera_.x,(int)-camera_.y, program);
    testSprite_->Render(-camera_, program);
    // render all entities. TODO: render list instead
    for(auto it=loadedEntities_.begin(); it != loadedEntities_.end(); ++it)
    {
        (*it)->Render(-camera_, program);
    }
}

Sprite* TileGame::LoadLGSpr(const std::string& name, int w, int h)
{
    auto& tm = engine::GameEngine::Get().GetTextureManager();
    const std::string path = std::string("res/textures/sprites/last-guardian-sprites/") + name;
    ogl::Texture* texture = tm.GetTexture(path + "_fr1.gif");
    Sprite* sprite = new Sprite(texture, w, h);
    sprite->AddAnimFrame("front_stand", tm.GetTexture(path + "_fr1.gif"));
    sprite->AddAnimFrame("front_walk", tm.GetTexture(path + "_fr1.gif"));
    sprite->AddAnimFrame("front_walk", tm.GetTexture(path + "_fr2.gif"));
    sprite->AddAnimFrame("left_stand", tm.GetTexture(path + "_lf2.gif"));
    sprite->AddAnimFrame("left_walk", tm.GetTexture(path + "_lf1.gif"));
    sprite->AddAnimFrame("left_walk", tm.GetTexture(path + "_lf2.gif"));
    sprite->AddAnimFrame("back_stand", tm.GetTexture(path + "_bk1.gif"));
    sprite->AddAnimFrame("back_walk", tm.GetTexture(path + "_bk1.gif"));
    sprite->AddAnimFrame("back_walk", tm.GetTexture(path + "_bk2.gif"));
    sprite->AddAnimFrame("right_stand", tm.GetTexture(path + "_rt2.gif"));
    sprite->AddAnimFrame("right_walk", tm.GetTexture(path + "_rt1.gif"));
    sprite->AddAnimFrame("right_walk", tm.GetTexture(path + "_rt2.gif"));
    return sprite;
}

void TileGame::UnloadLGSpr(Sprite*& sprite, const std::string& name)
{
    auto& tm = engine::GameEngine::Get().GetTextureManager();
    const std::string path = std::string("res/textures/sprites/last-guardian-sprites/") + name;
    tm.UnloadTexture(path + "_fr1.gif");
    tm.UnloadTexture(path + "_fr2.gif");
    tm.UnloadTexture(path + "_lf1.gif");
    tm.UnloadTexture(path + "_lf2.gif");
    tm.UnloadTexture(path + "_bk1.gif");
    tm.UnloadTexture(path + "_bk2.gif");
    tm.UnloadTexture(path + "_rt1.gif");
    tm.UnloadTexture(path + "_rt2.gif");
    delete sprite;
    sprite = nullptr;
}

void TileGame::CleanupLoadedEntities()
{
    for(auto it=loadedEntities_.begin(); it != loadedEntities_.end(); ++it)
    {
        delete (*it);
    }
    loadedEntities_.clear();
}
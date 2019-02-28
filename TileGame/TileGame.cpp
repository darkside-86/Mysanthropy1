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
#include "engine/ui/Root.h"

TileGame::TileGame()
{
    tileMap_ = new TileMap("res/tilemaps/island.bin");
    loadedEntities_ = tileMap_->GenerateEntities();
    configuration_ = new Configuration("TileGame/gameconfig.lua");
}

TileGame::~TileGame()
{
    delete tileMap_;
    delete configuration_;
    CleanupLoadedEntities();
}

bool TileGame::Initialize()
{
    // TODO: place survivalist gender or current class choice in savegame.bin
    // but for now default boy sprite
    playerSprite_ = LoadLGSpr(configuration_->GetBoySurvivalistSprite());
    int ix, iy;
    configuration_->GetTileSpawnPoint(ix, iy);
    playerSprite_->SetPosition({
        (float)ix * (float)tileMap_->GetTileSet()->GetTileWidth(),
        (float)iy * (float)tileMap_->GetTileSet()->GetTileHeight(),
        0.f
    });
    playerSprite_->SetCollisionBox(5.f, (float)playerSprite_->GetWidth() / 2.f, 
            (float)playerSprite_->GetWidth()-5.f, (float)playerSprite_->GetHeight());

    playerSprite_->SetCurrentAnim("front_stand", 0.2f);
    playerSprite_->StartAnimation();
    const float baseSpeed = configuration_->GetBasePlayerSpeed();
    engine::GameEngine::Get().AddKeyboardListener([this, baseSpeed](const SDL_KeyboardEvent& e){
        if(e.type == SDL_KEYDOWN)
        {
            glm::vec3 vel = playerSprite_->GetVelocity();
            if(casting_)
            {
                casting_ = false;
                ToggleCastBar(false);
                WriteLineToConsole("Cast interrupted by player", 1.f, 0.f, 0.f, 0.9f);
            }
            switch(e.keysym.sym)
            {
                case SDLK_LEFT: 
                    if(playerSprite_->GetCurrentAnim() != "left_walk")
                        playerSprite_->SetCurrentAnim("left_walk", 0.2f);
                    vel.x = -1.f;
                    break;
                case SDLK_RIGHT:
                    if(playerSprite_->GetCurrentAnim() != "right_walk")
                        playerSprite_->SetCurrentAnim("right_walk", 0.2f); 
                    vel.x = 1.f; 
                    break;
                case SDLK_UP:
                    if(playerSprite_->GetCurrentAnim() != "back_walk")
                        playerSprite_->SetCurrentAnim("back_walk", 0.2f); 
                    vel.y = -1.f; 
                    break;
                case SDLK_DOWN:
                    if(playerSprite_->GetCurrentAnim() != "front_walk")
                        playerSprite_->SetCurrentAnim("front_walk", 0.2f); 
                    vel.y = 1.f; 
                    break;
                default:
                    // without this the entire screen except UI goes black......
                    // TODO: fix weird velocity bug?
                    return;
            }
            if(vel.length() != 0)
            {
                playerSprite_->StartAnimation();
                if(vel.y != 0)
                    vel.y /= glm::abs(vel.y); // set to 1 or -1
                if(vel.x != 0)
                    vel.x /= glm::abs(vel.x); // set to 1 or -1
                vel = glm::normalize(vel) * baseSpeed;
            }
            playerSprite_->SetVelocity(vel);
        }
        else 
        {
            // todo: stop movement based on key up
            playerSprite_->PauseAnimation();
            playerSprite_->SetVelocity({0.f,0.f,0.f});
        }
    });

    engine::GameEngine::Get().AddMouseButtonListener([this](const SDL_MouseButtonEvent& e){
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            if(e.button == 1) // primary mouse button
            {
                int clickedX = e.x;
                int clickedY = e.y;
                engine::GameEngine::Get().SetLogicalXY(clickedX, clickedY);
                // look for an entity/mob to target.
                auto found = std::find_if(loadedEntities_.begin(), loadedEntities_.end(), 
                    [this, clickedX, clickedY](const Entity* ent) {
                        float x = camera_.x + (float)clickedX;
                        float y = camera_.y + (float)clickedY;
                        return (
                            x > ent->GetPosition().x && x < ent->GetPosition().x + (float)ent->GetWidth() &&
                            y > ent->GetPosition().y && y < ent->GetPosition().y + (float)ent->GetHeight()
                        );
                    }
                );
                if(found != loadedEntities_.end())
                {
                    if((*found) == targetedEntity_)
                    {
                        if(!casting_)
                        {
                            WriteLineToConsole("Attempting to interact with target");
                            InteractWithTarget();
                        }
                        else 
                        {
                            WriteLineToConsole("Already interacting with object...");
                        }
                    }
                    else 
                    {
                        targetedEntity_ = (*found);
                        std::string rclicks = std::to_string(targetedEntity_->GetRemainingClicks());
                        std::string mclicks = std::to_string(targetedEntity_->GetMaxClicks());
                        WriteLineToConsole(std::string("Now targeting a ") 
                            + targetedEntity_->GetName() + " (" + rclicks + "/" + mclicks + ")");
                    }
                }
                else 
                {
                    ClearTarget();
                    casting_ = false; // how else to handle the resulting null pointer?
                    WriteLineToConsole(std::string("Now targeting nothing"));
                }
            }
        }
    });

    SetupRenderList();

    engine::ui::Root::Get()->Initialize();
    SetupUIScript();

    WriteLineToConsole("Hello from C++", 1.f, 0.f, 1.f, 1.f);

    return true;
}

void TileGame::Cleanup()
{
    // TODO: unloaded sprite choice reflected by what was loaded in savegame.lua
    UnloadLGSpr(playerSprite_, configuration_->GetBoySurvivalistSprite());
    RemoveSpriteFromRenderList(playerSprite_);
    delete luaBindings_;
    lua_close(uiScript_);
}

void TileGame::Update(float dtime)
{
    playerSprite_->Update(dtime);
    // tile collision detection.
    float left, top, right, bottom;
    playerSprite_->GetCollisionBox(left, top, right, bottom);

    // check casting and update cast bar
    if(casting_)
    {
        currentCastTime_ += dtime;
        SetCastBarValue(currentCastTime_ / maxCastTime_);
        if(currentCastTime_ >= maxCastTime_)
        {
            // the cast is completed so run ability, currently only harvest.
            WriteLineToConsole("Cast complete");
            targetedEntity_->DecRemainingClicks();
            if(targetedEntity_->GetRemainingClicks() <= 0)
            {
                WriteLineToConsole("Removing...");
                RemoveSpriteFromRenderList(targetedEntity_);
                RemoveEntityFromLoaded(targetedEntity_);
                ClearTarget();
            }
            casting_ = false;
            ToggleCastBar(false);
        }
    }

    // check each corner
    // TODO: fix GameEngine update loop to prevent walking through tile hack
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
        playerSprite_->Update(-dtime);
    // test player against entity collision
    else if(EntityCollisionCheck(playerSprite_))
        playerSprite_->Update(-dtime);
    
    // todo: bound player to map area

    // calculate camera.
    float scrW = (float)engine::GameEngine::Get().GetWidth();
    float scrH = (float)engine::GameEngine::Get().GetHeight();
    glm::vec3 playerPos = playerSprite_->GetPosition();

    camera_.x = playerPos.x - scrW / 2.f;
    camera_.y = playerPos.y - scrH / 2.f;

    // TODO: bound camera to map area
    /*if(camera_.x < 0.0f)
        camera_.x = 0.0f;
    if(camera_.y < 0.0f)
        camera_.y = 0.0f;*/

    engine::ui::Root::Get()->Update(dtime);
}

void TileGame::Render(engine::GraphicsContext& gc)
{
    // disable for 2D rendering with semi-transparent data
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    ogl::Program& program = gc.GetProgram();
    program.Use();
    // colors are texture data only
    program.SetUniform<int>("u_useTexture", true);
    program.SetUniform<int>("u_useColorBlending", false);
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
    // TODO: fix glitchy tile movement
    tileMap_->Render(-camera_.x,-camera_.y, program);
    RenderSortPass();
    for(auto it=renderList_.begin(); it != renderList_.end(); ++it)
    {
        (*it)->Render(-camera_, program);
    }
    engine::ui::Root::Get()->Render(gc);
}

void TileGame::WriteLineToConsole(const std::string& line, float r, float g, float b, float a)
{
    lua_getglobal(uiScript_, "WriteLineToConsole");
    lua_pushstring(uiScript_, line.c_str());
    lua_pushnumber(uiScript_, r);
    lua_pushnumber(uiScript_, g);
    lua_pushnumber(uiScript_, b);
    lua_pushnumber(uiScript_, a);
    int ok = lua_pcall(uiScript_, 5, 0, 0);
    if(ok != LUA_OK)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
            "%s: %s", __FUNCTION__, lua_tostring(uiScript_, -1));
        lua_pop(uiScript_, 1);
    }
}

void TileGame::SetCastBarValue(float value)
{
    lua_getglobal(uiScript_, "SetCastBarValue");
    lua_pushnumber(uiScript_, value);
    int ok = lua_pcall(uiScript_, 1, 0, 0);
    if(ok != LUA_OK)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
            "%s: %s", __FUNCTION__, lua_tostring(uiScript_, -1));
        lua_pop(uiScript_, 1);
    }
}

void TileGame::ToggleCastBar(bool show)
{
    lua_getglobal(uiScript_, "ToggleCastBar");
    lua_pushboolean(uiScript_, show);
    int ok = lua_pcall(uiScript_, 1, 0, 0);
    if(ok != LUA_OK)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
            "%s: %s", __FUNCTION__, lua_tostring(uiScript_, -1));
        lua_pop(uiScript_, 1);
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

void TileGame::SetupRenderList()
{
    renderList_.clear();
    renderList_.push_back(playerSprite_);
    for(int i=0; i < loadedEntities_.size(); ++i)
    {
        renderList_.push_back(loadedEntities_[i]);
    }
}

void TileGame::RenderSortPass()
{
    // one pass of sorting. The larger the Y-base position, the later rendered
    for(int i=0; i < renderList_.size() - 1; ++i)
    {
        float y0 = renderList_[i]->GetPosition().y + (float)renderList_[i]->GetHeight();
        float y1 = renderList_[i+1]->GetPosition().y + (float)renderList_[i+1]->GetHeight();
        if(y0 > y1)
        {
            Sprite* swap = renderList_[i];
            renderList_[i] = renderList_[i+1];
            renderList_[i+1] = swap;
        }
    }
}

void TileGame::RemoveSpriteFromRenderList(const Sprite* sprite)
{
    auto it = std::find_if(renderList_.begin(), renderList_.end(), [sprite](const Sprite* spr){
        return sprite == spr;
    });
    if(it != renderList_.end())
        renderList_.erase(it);
}

void TileGame::SetupUIScript()
{
    uiScript_ = luaL_newstate();
    luaL_openlibs(uiScript_);
    luaBindings_ = new engine::ui::LuaBindings(uiScript_);
    std::vector<const char*> CORE_UI_LIB = {
        "ui/lib/fonts.lua", 
        "ui/lib/keycodes.lua", 
        "ui/lib/Window.lua", 
        "ui/TileGame.lua" // main UI code
    };
    try 
    {
        int errCode = 0;
        for(auto luaFile : CORE_UI_LIB)
        {
            errCode = luaL_dofile(uiScript_, luaFile);
            if(errCode != 0) throw errCode;
        }
    }
    catch(int err)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "Lua error %d: %s", err, lua_tostring(uiScript_, -1));
        lua_pop(uiScript_, 1);
    }
}

bool TileGame::EntityCollisionCheck(Sprite* sprite)
{
    // given a sprite check collision (for now) for all entities.
    float spriteLeft, spriteTop, spriteRight, spriteBottom;
    sprite->GetCollisionBox(spriteLeft, spriteTop, spriteRight, spriteBottom);
    for(auto it : loadedEntities_)
    {
        float l, t, r, b;
        it->GetCollisionBox(l,t,r,b);
        if(sprite != it && it->HasValidCollisionBox()) // don't check sprite against itself
        {
            if(CheckPoint(spriteLeft, spriteTop, l,t,r,b) ||
              CheckPoint(spriteRight, spriteTop, l,t,r,b) ||
              CheckPoint(spriteLeft, spriteBottom, l,t,r,b) ||
              CheckPoint(spriteRight, spriteBottom, l,t,r,b) ||
              CheckPoint(l,t, spriteLeft, spriteTop, spriteRight, spriteBottom) ||
              CheckPoint(r,t, spriteLeft, spriteTop, spriteRight, spriteBottom) || 
              CheckPoint(l,b, spriteLeft, spriteTop, spriteRight, spriteBottom) ||
              CheckPoint(r,b, spriteLeft, spriteTop, spriteRight, spriteBottom))
                return true;
        }
    }
    return false;
}

bool TileGame::CheckPoint(float x, float y, float left, float top, float right, float bottom)
{
    return (x >= left && x <= right && y >= top && y <= bottom);
}

void TileGame::InteractWithTarget()
{
    // check to see if we are in 32.f units of bottom center of targeted entity.
    glm::vec3 bottomCenter = targetedEntity_->GetPosition();
    bottomCenter.x += (float)targetedEntity_->GetWidth() / 2.f;
    bottomCenter.y += (float)targetedEntity_->GetHeight();
    glm::vec3 playerCenter = playerSprite_->GetPosition();
    playerCenter.x += (float)playerSprite_->GetWidth() / 2.f;
    playerCenter.y += (float)playerSprite_->GetHeight() / 2.f;
    float dist = glm::distance(bottomCenter, playerCenter);
    const float maxDistance = 48.f;
    if(dist > maxDistance)
    {
        WriteLineToConsole("Out of range!", 1.f, 0.f, 0.f, 1.f);
    }
    else 
    {
        maxCastTime_ = targetedEntity_->GetClickTime();
        currentCastTime_ = 0.0f;
        casting_ = true;
        WriteLineToConsole("Interacting with target...", 0.f, 1.f, 0.f, 1.f);
        ToggleCastBar(true);
    }
}

void TileGame::ClearTarget()
{   
    targetedEntity_ = nullptr;
    // TODO: when target circles are rendered, clear their state and visibility
}

void TileGame::RemoveEntityFromLoaded(Entity* ent)
{
    // first take it out of vector
    auto found = std::find_if(loadedEntities_.begin(), loadedEntities_.end(), 
        [this, ent](const Entity* enti){
            return ent == enti;
        }
    );
    if(found != loadedEntities_.end())
    {
        loadedEntities_.erase(found);
    }
    // clear target state in case it is being targeted
    if(targetedEntity_ == ent)
    {
        ClearTarget();
    }
    // finally delete the object.
    delete ent;
}
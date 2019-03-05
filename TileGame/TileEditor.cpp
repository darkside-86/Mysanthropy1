// TileEditor.cpp
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2019
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <vector>

#include "engine/GameEngine.hpp"
#include "engine/ui/Root.hpp"
#include "ogl/ErrorCheck.hpp"
#include "TileEditor.hpp"

TileEditor::TileEditor()
{
}

TileEditor::~TileEditor()
{
}

bool TileEditor::Initialize()
{
    engine::ui::Root::Get()->Initialize();
    uiScript_ = luaL_newstate();
    luaL_openlibs(uiScript_);
    luaBindings_ = new engine::ui::LuaBindings(uiScript_);

    lua_pushstring(uiScript_, "TileEditor");
    lua_pushlightuserdata(uiScript_, this);
    lua_settable(uiScript_, LUA_REGISTRYINDEX);
    lua_pushcfunction(uiScript_, TileEditor::lua_SaveMap);
    lua_setglobal(uiScript_, "TileEditor_SaveMap");
    lua_pushcfunction(uiScript_, TileEditor::lua_LoadMap);
    lua_setglobal(uiScript_, "TileEditor_LoadMap");
    lua_pushcfunction(uiScript_, TileEditor::lua_NewMap);
    lua_setglobal(uiScript_, "TileEditor_NewMap");
    lua_pushcfunction(uiScript_, TileEditor::lua_FillWithSelection);
    lua_setglobal(uiScript_, "TileEditor_FillWithSelection");
    lua_pushcfunction(uiScript_, TileEditor::lua_SetSelectedLayer);
    lua_setglobal(uiScript_, "TileEditor_SetSelectedLayer");
    lua_pushcfunction(uiScript_, TileEditor::lua_SetCollisionLayer);
    lua_setglobal(uiScript_, "TileEditor_SetCollisionLayer");
    lua_pushcfunction(uiScript_, TileEditor::lua_GetScriptPath);
    lua_setglobal(uiScript_, "TileEditor_GetScriptPath");
    lua_pushcfunction(uiScript_, TileEditor::lua_SetScriptPath);
    lua_setglobal(uiScript_, "TileEditor_SetScriptPath");
    lua_pushcfunction(uiScript_, TileEditor::lua_SelectEntity);
    lua_setglobal(uiScript_, "TileEditor_SelectEntity");
    lua_pushcfunction(uiScript_, TileEditor::lua_RemoveEntity);
    lua_setglobal(uiScript_, "TileEditor_RemoveEntity");

    tileMap_ = new TileMap("res/tilemaps/island.bin");
    tileSet_ = tileMap_->GetTileSet();
    entities_ = tileMap_->GenerateEntities();

    std::vector<const char*> CORE_UI_LIB = {
        "ui/lib/fonts.lua", "ui/lib/keycodes.lua", "ui/lib/Window.lua", "ui/TileEditor.lua"
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

    // tileSet_ = new TileSet("res/textures/tilesets/ts2.png", 32, 32);

    // tileMap_ = new TileMap(tileSet_, 32, 32);

    engine::GameEngine::Get().AddMouseButtonListener([this](const SDL_MouseButtonEvent& e){

        if(e.type == SDL_MOUSEBUTTONDOWN && e.button == 1)
        {
            int clickedX = e.x, clickedY = e.y;
            engine::GameEngine::Get().SetLogicalXY(clickedX, clickedY);

            // first check to see if we need to place an entity
            if(entityToPlace_ != nullptr && entityID_ >= 0 && !entityToRemove_)
            {
                entityToPlace_->SetPosition({
                    -(float)cameraX_ + (float)clickedX,
                    -(float)cameraY_ + (float)clickedY,
                    0.f
                });
                entities_.push_back(entityToPlace_);
                entityToPlace_ = nullptr;
                tileMap_->AddEntityLocation(entityID_, 
                    -cameraX_ + clickedX,
                    -cameraY_ + clickedY);
                return; // nothing else to do for this mouse click
            }
            else if(entityToRemove_)
            {
                tileMap_->RemoveEntityLocation(entityID_, -cameraX_ + clickedX, -cameraY_ + clickedY);
                RemoveEntity(entityID_, -cameraX_ + clickedX, -cameraY_ + clickedY);
                entityToRemove_ = false;
                return;
            }
            // what is the selected x,y index value based on what is clicked at top with tiles rendered
            // at 1/4 of original size?
            //
            int drawnTileWidth = tileSet_->GetTileWidth() / 2;
            int drawnTileHeight = tileSet_->GetTileHeight() / 2;
            int screenWidth = engine::GameEngine::Get().GetWidth();
            int screenHeight = engine::GameEngine::Get().GetHeight();
            int ntx, nty;
            tileSet_->GetNumTiles(ntx, nty);
            // e.g. J=3,1 in algorithm
            int sx = clickedX / drawnTileWidth;
            int sy = clickedY / drawnTileHeight;
            // e.g. J=9 in algorithm
            int scrnx = screenWidth / drawnTileWidth;
            int scrny = screenHeight / drawnTileHeight;
            int ndindex = sy * scrnx + sx;
            // e.g. J=1,2 in algorithm
            if(ndindex >= ntx * nty)
            {
                // change the selected Tile to the selected tileset node because a new
                // one wasn't selected.
                //
                SetTileToSelected(clickedX, clickedY);
            }
            else
            {
                selectedIX_ = ndindex % ntx;
                selectedIY_ = ndindex / ntx;
            }
        }
    });

    engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e){
        if(e.type == SDL_KEYDOWN)
        {
            int w = tileSet_->GetTileWidth();
            int h = tileSet_->GetTileHeight();
            switch(e.keysym.sym)
            {
            case SDLK_UP: cameraY_ += h; break;
            case SDLK_DOWN: cameraY_ -= h; break;
            case SDLK_LEFT: cameraX_ += w; break;
            case SDLK_RIGHT: cameraX_ -= w; break;
            }
            int x,y;
            SDL_GetMouseState(&x, &y);
            UpdateHoverData(x,y);
        }
    });

    engine::GameEngine::Get().AddMouseMotionListener([this](const SDL_MouseMotionEvent& e){
        UpdateHoverData(e.x, e.y);        
    });

    return true;
}

void TileEditor::Cleanup()
{
    // tileMap_->SaveToFile("res/tilemaps/output.bin");

    delete luaBindings_;
    lua_close(uiScript_);
    delete tileMap_;
}

void TileEditor::Update(float dtime)
{
    engine::ui::Root::Get()->Update(dtime);
}

void TileEditor::Render(engine::GraphicsContext& gc)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    ogl::Program& program = gc.GetProgram();
    glm::mat4 projection(1.f);
    glm::mat4 view(1.f);

    program.Use();
    gc.SetUseTexture(true);
    gc.SetUseColorBlending(false);
    projection = glm::ortho(0.f, (float)engine::GameEngine::Get().GetWidth(),
            (float)engine::GameEngine::Get().GetHeight(), 0.f);
    program.SetUniform<glm::mat4>("u_projection", projection);
    program.SetUniform<glm::mat4>("u_view", view);

    // draw tilemap first
    // tileMap_->Draw(cameraX_, cameraY_, program);
    tileMap_->Render((float)cameraX_, (float)cameraY_, program);
    tileMap_->RenderCollisionData((float)cameraX_, (float)cameraY_, program, 1.f, 1.f);

    // draw the entities.
    for(auto it : entities_)
    {
        it->Render(glm::vec3((float)cameraX_, (float)cameraY_, 0.f), program);
    }

    // draw all tiles from the set across the top
    int screenWidth = engine::GameEngine::Get().GetWidth();
    int screenHeight = engine::GameEngine::Get().GetHeight();
    int x = 0, y = 0, ix = 0, iy = 0;
    int numTilesX, numTilesY;
    tileSet_->GetNumTiles(numTilesX, numTilesY);
    while( ix * numTilesY + iy < numTilesX * numTilesY)
    {
        tileSet_->DrawTile(x, y, 0.5f, 0.5f, program, ix, iy);
        ++ix;
        if(ix >= numTilesX)
        {
            ix = 0;
            ++iy;
        }

        x += tileSet_->GetTileWidth() / 2 ;
        if( x >= screenWidth )
        {
            x = 0;
            y += tileSet_->GetTileHeight() / 2;
        }
    }
    // draw selected tile 2x size at bottom left of screen
    tileSet_->DrawTile(0, screenHeight - tileSet_->GetTileHeight() * 2, 2.f, 2.f, program, 
            selectedIX_, selectedIY_);

    // draw entire scaled map at bottom center
    float scaleX = 1.f / (float)tileSet_->GetTileWidth();
    float scaleY = 1.f / (float)tileSet_->GetTileHeight();
    float scaledMapWidth = (float)tileMap_->GetWidth();
    float scaledMapHeight = (float)tileMap_->GetHeight();
    if(scaledMapWidth >= 128.f)
    {
        scaledMapWidth /= 2.f;
        scaleX /= 2.f;
    }
    if(scaledMapHeight >= 128.f)
    {
        scaledMapHeight /= 2.f;
        scaleY /= 2.f;
    }
    float screenX = ((float)screenWidth / 2.f - (scaledMapWidth / 2.f));
    float screenY = ((float)screenHeight - scaledMapHeight);
    tileMap_->Render(screenX, screenY, program, scaleX, scaleY);

    engine::ui::Root::Get()->Render(gc);
}

void TileEditor::SetTileToSelected(int mouseX, int mouseY)
{
    int tileWidth = tileSet_->GetTileWidth();
    int tileHeight = tileSet_->GetTileHeight();
    int ix = (mouseX - cameraX_) / tileWidth;
    int iy = (mouseY - cameraY_) / tileHeight;
    if(collisionLayerSelected_)
    {
        tileMap_->SetCollisionData(ix, iy, tileMap_->GetCollisionData(ix,iy) ? 0 : 1);
    }
    else 
    {
        tileMap_->SetTile(ix, iy, {(unsigned short)selectedIX_, (unsigned short)selectedIY_}, selectedLayer_);
    }
}

void TileEditor::UpdateHoverData(int mouseX, int mouseY)
{
    int tileWidth = tileSet_->GetTileWidth();
    int tileHeight = tileSet_->GetTileHeight();
    hoverIX_ = (mouseX - cameraX_) / tileWidth;
    hoverIY_ = (mouseY - cameraY_) / tileHeight;
    lua_pushinteger(uiScript_, hoverIX_);
    lua_setglobal(uiScript_, "g_hoverX");
    lua_pushinteger(uiScript_, hoverIY_);
    lua_setglobal(uiScript_, "g_hoverY");
    luaL_dostring(uiScript_, "UpdateHoverData()");
}

void TileEditor::SetupSelection(int index)
{
    ENTITY_TYPE et = tileMap_->GetEntityType(index);
    if(et.name == nullptr)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "%s: Invalid entity type index %d", __FUNCTION__, index);
        return;
    }
    entityToPlace_ = new Entity(et);
    // once placed it will be added to vector of entities
}

void TileEditor::CleanupEntities()
{
    for(auto it=entities_.begin(); it != entities_.end(); ++it)
    {
        delete (*it);
    }
    entities_.clear();

    if(entityToPlace_)
    {
        delete entityToPlace_;
        entityToPlace_ = nullptr;
    }
}

void TileEditor::RemoveEntity(int index, int x, int y)
{
    auto t = tileMap_->GetEntityType(index);
    auto it = entities_.begin();
    for(; it != entities_.end(); ++it)
    {
        if( (*it)->GetName()==t.name 
            && (float)x >= (*it)->GetPosition().x 
            && (float)x <= (*it)->GetPosition().x + (float)(*it)->GetWidth()
            && (float)y >= (*it)->GetPosition().y 
            && (float)x <= (*it)->GetPosition().y + (float)(*it)->GetHeight() )
        {
            break;
        }
    }
    if(it != entities_.end())
    {
        entities_.erase(it);
    }
}

int TileEditor::lua_SaveMap(lua_State* L)
{
    // get TileEditor
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    const char* path = luaL_checkstring(L, 1);
    te->tileMap_->SaveToFile(path);

    return 0;
}

int TileEditor::lua_LoadMap(lua_State* L)
{
    // get TileEditor
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    const char* path = luaL_checkstring(L, 1);
    te->CleanupEntities();
    te->tileMap_->LoadFromFile(path);
    te->tileSet_ = te->tileMap_->GetTileSet();
    te->entities_ = te->tileMap_->GenerateEntities();
    return 0;
}

// tileWidth, tileHeight, tilesetPath, mapWidth, mapHeight
int TileEditor::lua_NewMap(lua_State* L)
{
    // get TileEditor
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    int tileWidth = (int)lua_tonumber(L, 1);
    int tileHeight = (int)lua_tonumber(L, 2);
    const char* tilesetPath = lua_tostring(L, 3);
    int mapWidth = (int)lua_tonumber(L, 4);
    int mapHeight = (int)lua_tonumber(L, 5);

    delete te->tileMap_;
    te->tileMap_ = new TileMap(tileWidth, tileHeight, tilesetPath, mapWidth, mapHeight);
    te->tileSet_ = te->tileMap_->GetTileSet();

    return 0;
}

int TileEditor::lua_FillWithSelection(lua_State* L)
{
    // get TileEditor
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    te->tileMap_->FillWithTile({(unsigned short)te->selectedIX_,(unsigned short)te->selectedIY_}, 
            te->selectedLayer_);

    return 0;
}

int TileEditor::lua_SetSelectedLayer(lua_State* L)
{
    // get TileEditor
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    int layer = (int)lua_tonumber(L, 1) == 0 ? 0 : 1;
    te->selectedLayer_ = layer;

    return 0;
}

int TileEditor::lua_SetCollisionLayer(lua_State* L)
{
    // get TileEditor
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    bool toggle = lua_toboolean(L, 1);
    te->collisionLayerSelected_ = toggle;

    return 0;
}

int TileEditor::lua_GetScriptPath(lua_State* L)
{
    // get TileEditor
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    std::string scriptPath = te->tileMap_->GetScriptPath();
    lua_pushstring(L, scriptPath.c_str());

    return 1;
}

int TileEditor::lua_SetScriptPath(lua_State* L)
{
    // get TileEditor
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    const char* scriptPath = lua_tostring(L, 1);
    te->tileMap_->SetScriptPath(scriptPath);

    return 0;
}

// TileEditor_SelectEntity(iIndex)
int TileEditor::lua_SelectEntity(lua_State* L)
{
    // get TileEditor
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    int index = (int)lua_tonumber(L, 1);
    te->entityID_ = index;
    te->SetupSelection(index);

    return 0;
}

int TileEditor::lua_RemoveEntity(lua_State* L)
{
    lua_pushstring(L, "TileEditor");
    lua_gettable(L, LUA_REGISTRYINDEX);
    TileEditor* te = (TileEditor*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    int index = (int)lua_tonumber(L, 1);
    te->entityID_ = index;
    te->entityToRemove_ = true;

    return 0;
}
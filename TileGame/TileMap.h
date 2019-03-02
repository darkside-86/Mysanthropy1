// TileMap.h
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
#pragma once

#include <string>
#include <vector>

#include <lua/lua.hpp>

#include "Entity.h"
#include "ogl/Program.h"
#include "ogl/Texture.h"
#include "ogl/VertexArray.h"
#include "ogl/VertexBuffer.h"
#include "TileSet.h"

struct Tile
{
    unsigned short ix=0;
    unsigned short iy=0;
};

struct ENTITY_LOCATION
{
    unsigned short entityID = 0;
    unsigned int x = 0, y = 0;
};

class TileMap
{
public:
    TileMap(int tileWidth, int tileHeight, const std::string& tilesetPath, int width, int height);
    TileMap(const std::string& path);
    virtual ~TileMap();
    // Render map to screen
    void Render(float x, float y, ogl::Program &program, float scaleX=1.f, float scaleY=1.f);
    void SaveToFile(const std::string& path);
    void LoadFromFile(const std::string& path);
    TileSet* GetTileSet() { return tileSet_; }
    std::string GetScriptPath() { return scriptPath_; }
    void SetScriptPath(const std::string& path) { scriptPath_ = path; }
    int GetWidth() { return width_; }
    int GetHeight() { return height_; }
    Tile GetTile(int ix, int iy, bool layer1 = false);
    void SetTile(int ix, int iy, const Tile& tile, bool layer1=false);
    void FillWithTile(const Tile& tile, bool layer1=false);
    unsigned char GetCollisionData(int ix, int iy);
    void SetCollisionData(int ix, int iy, unsigned char value);
    void RenderCollisionData(float x, float y, ogl::Program& program, float scaleX, float scaleY);
    // Generate the entity list for the caller, which will own the pointers. The ENTITY_TYPE table
    //  is destroyed so this should be called only once per map load.
    std::vector<Entity*> GenerateEntities();
    // Get an entity type from the list. tilemap retains pointer ownership
    ENTITY_TYPE GetEntityType(int index);
    // Add an entity location to map
    void AddEntityLocation(unsigned short entityID, unsigned int x, unsigned int y);
    bool RemoveEntityLocation(unsigned short entityID, unsigned int x, unsigned int y);
private:
    void SetupRender();
    void SetupScripting();
    void CleanupEntities();
    // start processing the next entity
    static int lua_BeginEntity(lua_State* L);
    static int lua_UseTexture(lua_State* L);
    static int lua_Width(lua_State* L);
    static int lua_Height(lua_State* L);
    static int lua_CollisionBox(lua_State* L);
    static int lua_MaxClicks(lua_State* L);
    static int lua_OnInteract(lua_State* L);
    static int lua_OnDestroy(lua_State* L);
    static int lua_ClickTime(lua_State* L);
    static int lua_Farmable(lua_State* L);
    static int lua_EndEntity(lua_State* L);
    TileSet* tileSet_ = nullptr;
    std::string scriptPath_ = "";
    int width_ = 0;
    int height_ = 0;
    Tile* layer0_ = nullptr;
    Tile* layer1_ = nullptr;
    unsigned char* collisionLayer_ = nullptr;
    // layer 0
    ogl::VertexArray* vao_ = nullptr;
    ogl::VertexBuffer vbo_;
    // layer 1
    ogl::VertexArray* vao1_ = nullptr;
    ogl::VertexBuffer vbo1_;
    // collision tile "X" indicator
    ogl::Texture* redTexture_ = nullptr;
    ogl::VertexArray* collisionVao_ = nullptr;
    ogl::VertexBuffer collisionVbo_;
    // for running entity scripting and map loading
    lua_State* scripting_ = nullptr;
    // entity index
    std::vector<ENTITY_TYPE> entityTypes_;
    ENTITY_TYPE currentEntityType_;
    std::vector<ENTITY_LOCATION> mapEntities_;
};
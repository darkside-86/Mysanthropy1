// TileMap.hpp
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
#pragma once

#include <string>
#include <vector>

#include <lua/lua.hpp>

#include "combat/CombatAbilityLists.hpp"
#include "game/Entity.hpp"
#include "game/MobSpawner.hpp"
#include "ogl/Program.hpp"
#include "ogl/Texture.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/VertexBuffer.hpp"
#include "TileSet.hpp"

namespace world
{

    struct TILE
    {
        unsigned short ix=0;
        unsigned short iy=0;
    };

    struct ENTITY_LOCATION
    {
        static constexpr unsigned short INVALID_ENTITY_ID = (unsigned short)(-1);
        unsigned short entityID = 0;
        unsigned int x = 0, y = 0;
    };

    struct MOBSPAWNER_LOCATION
    {
        static constexpr unsigned short INVALID_MOBTYPE_ID = (unsigned short)(-1);
        unsigned short spawnerID = 0;
        unsigned int x = 0, y = 0;
        float freq = 30.0f; // in seconds
        float chance = 100.0f; // 0.0 - 100.0
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
        TILE GetTile(int ix, int iy, bool layer1 = false);
        void SetTile(int ix, int iy, const TILE& tile, bool layer1=false);
        // return true if tile at x,y is configured as "liquid"
        bool TileIsLiquid(int x, int y, bool layer1=false);
        void FillWithTile(const TILE& tile, bool layer1=false);
        unsigned char GetCollisionData(int ix, int iy);
        void SetCollisionData(int ix, int iy, unsigned char value);
        void RenderCollisionData(float x, float y, ogl::Program& program, float scaleX, float scaleY);
        // Generate the entity list for the caller, which will own the pointers. The ENTITY_TYPE table
        //  is destroyed so this should be called only once per map load.
        std::vector<game::Entity*> GenerateEntities();
        // Get an entity type from the list. tilemap retains pointer ownership
        game::EntityType GetEntityType(int index);
        // generate mobspawner vector, caller owns pointers
        std::vector<game::MobSpawner*> GenerateSpawners();
        // Get a mobtype from the list.
        game::MobType GetMobType(int index);
        // Add an entity location to map
        void AddEntityLocation(unsigned short entityID, unsigned int x, unsigned int y);
        bool RemoveEntityLocation(unsigned short entityID, unsigned int x, unsigned int y);
        unsigned short GetEntityIDAtLocation(unsigned int x, unsigned int y);
        // Add a spawner location to map
        void AddSpawnerLocation(unsigned short spawnerID, unsigned int x, unsigned int y, float freq, float chance);
        void RemoveSpawnerLocation(unsigned short spawnerID, unsigned int x, unsigned int y);
        unsigned short GetMobTypeIDAtLocation(unsigned int x, unsigned int y, float& freq, float& chance);
    private:
        void SetupRender();
        void SetupScripting();
        void CleanupEntities();
        void CleanupSpawners();
        static TileMap* GetTileMapObject(lua_State* L);
        static int lua_Liquids(lua_State* L);
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
        static int lua_BeginMobType(lua_State* L);
        static int lua_LevelRange(lua_State* L);
        static int lua_DefaultAnimation(lua_State* L);
        static int lua_FrAnimTextureList(lua_State* L);
        static int lua_BkAnimTextureList(lua_State* L);
        static int lua_LfAnimTextureList(lua_State* L);
        static int lua_RtAnimTextureList(lua_State* L);
        static int lua_AnimSpeed(lua_State* L);
        static int lua_MobWidth(lua_State* L);
        static int lua_MobHeight(lua_State* L);
        static int lua_MobSpeed(lua_State* L);
        static int lua_MobLeash(lua_State* L);
        static int lua_MobCollisionBox(lua_State* L);
        static int lua_MobAggroType(lua_State* L);
        static int lua_CombatAbilityList(lua_State* L);
        static int lua_LootTable(lua_State* L);
        static int lua_EndMobType(lua_State* L);
        TileSet* tileSet_ = nullptr;
        std::string scriptPath_ = "";
        int width_ = 0;
        int height_ = 0;
        TILE* layer0_ = nullptr;
        TILE* layer1_ = nullptr;
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
        // tile ID configurations
        std::vector<TILE> liquids_;
        // entity index
        std::vector<game::EntityType> entityTypes_;
        game::EntityType currentEntityType_;
        std::vector<ENTITY_LOCATION> mapEntities_;
        // mob spawners
        std::vector<game::MobType> mobTypes_;
        game::MobType currentMobType_;
        std::vector<MOBSPAWNER_LOCATION> spawnerLocations_;
    };
}
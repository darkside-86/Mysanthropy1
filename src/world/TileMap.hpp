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

#include "combat/AbilityTable.hpp"
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
        // constructor. creates a completely new TileMap with the given dimensions and tile set path
        TileMap(int tileWidth, int tileHeight, const std::string& tilesetPath, int width, int height);
        // constructor. loads a tile map from existing file
        TileMap(const std::string& path);
        // destructor
        virtual ~TileMap();
        // Render map to screen
        void Render(float x, float y, ogl::Program &program, float scaleX=1.f, float scaleY=1.f);
        // saves the tile map to file
        void SaveToFile(const std::string& path);
        // discards existing tile map object's data and loads data from file. Similar to the constructor
        void LoadFromFile(const std::string& path);
        // returns the tile set in use.
        TileSet* GetTileSet() { return tileSet_; }
        // returns file path to the script associated with the tile map
        std::string GetScriptPath() { return scriptPath_; }
        // sets the script path associated with the tile map
        void SetScriptPath(const std::string& path) { scriptPath_ = path; }
        // gets the width in tiles of the tile map
        int GetWidth() { return width_; }
        // gets the height in tiles of the tile map
        int GetHeight() { return height_; }
        // returns tile data and given tile index ix,iy on the layer specified (0 or 1)
        TILE GetTile(int ix, int iy, bool layer1 = false);
        // sets tile data at given tile index on the layer specified (0 or 1)
        void SetTile(int ix, int iy, const TILE& tile, bool layer1=false);
        // return true if tile at x,y is configured as "liquid"
        bool TileIsLiquid(int x, int y, bool layer1=false);
        // returns true if tile at x,y,layer0 is buildable AND x,y,layer1 is "empty"
        bool TileIsBuildable(int x, int y);
        // returns the path of the swimming texture that should be used.
        inline std::string GetSwimmingTexture() const { return swimmingTexture_; }
        // fills an entire layer of the map with a specific tile
        void FillWithTile(const TILE& tile, bool layer1=false);
        // returns 1 if a tile at tile index on either layer is considered "solid" otherwise 0
        unsigned char GetCollisionData(int ix, int iy);
        // sets a tile at index to either solid (value=1) or walkable (value=0)
        void SetCollisionData(int ix, int iy, unsigned char value);
        // renders a red X over tiles that are solid no matter which layer they are on. It should be
        //  noted that collision information is a separate layer than the 2 tile render layers
        void RenderCollisionData(float x, float y, ogl::Program& program, float scaleX, float scaleY);
        // Generate the entity list for the caller, which will own the pointers. The EntityType table
        //  is destroyed so this should be called only once per map load.
        std::vector<game::Entity*> GenerateEntities();
        // Get an entity type from the list.
        game::EntityType GetEntityType(int index);
        // generate mobspawner vector, caller owns pointers. Should be called once per map load
        std::vector<game::MobSpawner*> GenerateSpawners();
        // Get a mobtype from the list.
        game::MobType GetMobType(int index);
        // Add an entity location to map by id and location
        void AddEntityLocation(unsigned short entityID, unsigned int x, unsigned int y);
        // Remove an entity location from the map by id and location
        bool RemoveEntityLocation(unsigned short entityID, unsigned int x, unsigned int y);
        // Returns the id of an entity at given location, and if not found, INVALID_ENTITY_ID
        unsigned short GetEntityIDAtLocation(unsigned int x, unsigned int y);
        // Add a spawner location to map with a given id, frequency of spawn, and chance of spawn
        void AddSpawnerLocation(unsigned short spawnerID, unsigned int x, unsigned int y, float freq, float chance);
        // Remove a spawner from a location of a given id (type)
        void RemoveSpawnerLocation(unsigned short spawnerID, unsigned int x, unsigned int y);
        // Returns the id (type) of spawner at a given location, or INVALID_MOBTYPE_ID if nothing is there
        unsigned short GetMobTypeIDAtLocation(unsigned int x, unsigned int y, float& freq, float& chance);
    private:
        // sets up opengl objects needed to render map
        void SetupRender();
        // sets up Lua state and lua_* globals used for parsing the map's associated script
        void SetupScripting();
        // removes list of entities
        void CleanupEntities();
        // removes list of spawners
        void CleanupSpawners();
        // each of the below static functions re-gets its lua state associated host object
        static TileMap* GetTileMapObject(lua_State* L);
        // Lua global functions ///////////////////////////////////////////////
        static int lua_Liquids(lua_State* L);
        static int lua_Buildable(lua_State* L);
        static int lua_Empty(lua_State* L);
        static int lua_Swimming(lua_State* L);
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
        static int lua_MobClass(lua_State* L);
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
        static int lua_MobAggroRadius(lua_State* L);
        static int lua_Biome(lua_State* L);
        static int lua_LootTable(lua_State* L);
        static int lua_EndMobType(lua_State* L);
        ///////////////////////////////////////////////////////////////////////
        // The tile set object used for the textures of each rendered tile
        TileSet* tileSet_ = nullptr;
        // path to the script to be run when map loads and to define entity and mob data
        std::string scriptPath_ = "";
        // width in tiles of the map
        int width_ = 0;
        // height in tiles of the map
        int height_ = 0;
        // the lowest layer
        TILE* layer0_ = nullptr;
        // the layer rendered on top of layer 0
        TILE* layer1_ = nullptr;
        // a boolean array indicated tile solidity for collision detection purposes
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
        // for reading map configuration, entity, and mob database in script file
        lua_State* scripting_ = nullptr;
        // tile ID liquid configuration
        std::vector<TILE> liquids_;
        // tile ID buildable configuration
        std::vector<TILE> buildable_; 
        // what is the upper layer filled with to represent emptiness?
        TILE empty_;
        // swimming texture configuration
        std::string swimmingTexture_ = "res/textures/swimming.png"; // a reasonable default
        // static entity types and instances
        std::vector<game::EntityType> entityTypes_;
        game::EntityType currentEntityType_;
        std::vector<ENTITY_LOCATION> mapEntities_;
        // mob spawner types and instances
        std::vector<game::MobType> mobTypes_;
        game::MobType currentMobType_;
        std::vector<MOBSPAWNER_LOCATION> spawnerLocations_;
    };
}
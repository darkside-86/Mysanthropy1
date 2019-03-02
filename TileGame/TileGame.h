// TileGame.h
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

#include <vector>
#include <unordered_map>

#include <lua/lua.hpp>

#include "Configuration.h"
#include "engine/Game.h"
#include "engine/ui/LuaBindings.h"
#include "ogl/Texture.h"
#include "Player.h"
#include "PlayerCommand.h"
#include "TileMap.h"
#include "SaveData.h"
#include "Sprite.h"

struct ENT_COORDS
{
    int x=0, y=0;
    bool operator==(const ENT_COORDS& ec) const 
        { return x==ec.x && y == ec.y; }
};

namespace std { template <> struct hash<ENT_COORDS> {
    size_t operator() (const ENT_COORDS& ec) const {
        return ((size_t)ec.x<<32)|ec.y;
    }
}; }

// TileGame class. See docs/* and README.md for details.
class TileGame : public engine::Game 
{
public:
    TileGame();
    ~TileGame();
    bool Initialize();
    void Cleanup();
    void Update(float dtime);
    void Render(engine::GraphicsContext& gc);
    // UI functions
    void WriteLineToConsole(const std::string& line, float r=1.f, float g=1.f, float b=1.f, float a=1.f);
    void SetCastBarValue(float value);
    void ToggleCastBar(bool show);
    void SetExperienceBar(float value);
private:
    // load a sprite and animations from lost guardian folder
    Sprite* LoadLGSpr(const std::string& name, int w=0, int h=0);
    // unload textures associated with lost guardian sprite and destroy sprite
    void UnloadLGSpr(Sprite*& sprite, const std::string& name);
    void CleanupLoadedEntities();
    void SetupRenderList();
    void RenderSortPass();
    void RemoveSpriteFromRenderList(const Sprite* sprite);
    void SetupUIScript();
    bool EntityCollisionCheck(Sprite* sprite);
    bool CheckPoint(float x, float y, float left, float top, float right, float bottom);
    void InteractWithTarget();
    void ClearTarget();
    void RemoveEntityFromLoaded(Entity* ent);
    Entity* FindEntityByLocation(int x, int y);
    void UpdatePlayerExperience();
    void SetHarvestCommand(int x, int y, int clicks);
    void SetFarmCommand(int x, int y, const FarmCommand& fc);
    void PrintInventory();
    // convert harvestCommands_ to a vector of HarvestCommand and return the result
    std::vector<HarvestCommand> GetHarvestCommands();
    // convert farmCommands_ to a vector of FarmCommand and return the result
    std::vector<FarmCommand> GetFarmCommands();
    // Core game configuration
    Configuration* configuration_ = nullptr;
    // The map currently loaded
    TileMap* tileMap_ = nullptr;
    // Represents the location and image of the player in the world  
    Sprite* playerSprite_ = nullptr;
    // The entity being targeted (if any) by the user
    Entity* targetedEntity_ = nullptr;
    // True if a casting sequence was started.
    bool casting_ = false;
    float maxCastTime_ = 0.f;
    float currentCastTime_ = 0.f;
    // Player data
    Player player_;
    // harvest commands. value is number of clicks
    std::unordered_map<ENT_COORDS, int> harvestCommands_;
    // farm commands
    std::unordered_map<ENT_COORDS, FarmCommand> farmCommands_;
    // Save game data.
    SaveData saveData_;

    // Camera coordinates to determine where on screen objects are rendered
    glm::vec3 camera_ = {0.f,0.f,0.f};
    // list of all sprites to try to render including player. Does not own pointers
    std::vector<Sprite*> renderList_;
    // list of loaded map entities. Owns pointers
    std::vector<Entity*> loadedEntities_;
    lua_State* uiScript_ = nullptr;
    engine::ui::LuaBindings* luaBindings_ = nullptr;
};
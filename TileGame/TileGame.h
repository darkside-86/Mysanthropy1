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
#include "SplashScreen.h"
#include "Sprite.h"
#include "SwimFilter.h"
#include "Target.h"

struct ENT_COORDS
{
    int x=0, y=0;
    bool operator==(const ENT_COORDS& ec) const 
        { return x==ec.x && y == ec.y; }
};

// needed to use ENT_COORDS as an unordered_map key
namespace std { template <> struct hash<ENT_COORDS> {
    size_t operator() (const ENT_COORDS& ec) const {
        return ((size_t)ec.x<<32)|ec.y;
    }
}; }

// TileGame class. See docs/* and README.md for details.
class TileGame : public engine::Game 
{
public:
    // constructor
    TileGame();
    // destructor
    ~TileGame();
    // initializes the main menu UI and SDL event callbacks
    bool Initialize();
    // writes the save game to file unless game is exited at main menu
    void Cleanup();
    // update the game world
    void Update(float dtime);
    // render either the main menu or game world depending on game state
    void Render(engine::GraphicsContext& gc);
    // UI functions ///////////////////////////////////////////////////////////
    // Writes an unwrapped line to the Lua UI console window in the bottom left corner
    void UIWriteLineToConsole(const std::string& line, float r=1.f, float g=1.f, float b=1.f, float a=1.f);
    // Sets the progress level of the cast bar
    void UISetCastBarValue(float value);
    // Sets whether or not cast bar should be visible
    void UIToggleCastBar(bool show);
    // Sets the percentage of progress of the experience bar
    void UISetExperienceBar(float value);
    // Shows or hides the bottom right inventory window
    void UIShowInventory(bool show);
    // Creates the UI elements for the inventory window such as each item icon
    void UIBuildInventory();
    // Sets the value of the foodstuff counter in the corner
    void UISetFoodstuffBarData(int amount);
private:
    // determines what Update and Render does
    enum GAME_STATE { SPLASH, PLAYING };

    // menu handling and starting /////////////////////////////////////////////

    // This is called once the main menu is exited due to a new game being started
    // or a game file being selected for loading. Sets up core game world data 
    void StartGame();
    // Loads a game specified by saveSlot_ and fills in world and player data
    void LoadGame();
    // Saves a game to slot specified by saveSlot_
    void SaveGame();
    // Fills game world with data from configuration file instead of loading from savegame
    void NewGame();
    // Cleans up data to prepare for return to main menu
    void EndGame();
    // Rebuilds splash screen object and sets game state back to SPLASH SCREEN
    void ReturnToMainMenu();

    // load a sprite and animations from lost guardian folder
    Sprite* LoadLGSpr(const std::string& name, int w=0, int h=0);
    // unload textures associated with lost guardian sprite and destroy sprite
    void UnloadLGSpr(Sprite*& sprite, const std::string& name);
    // Checks if a given sprite is "Swimming" that is, its base is on a "liquid" tile
    bool SpriteIsSwimming(Sprite* sprite);
    // Destroy all entities
    void CleanupLoadedEntities();
    // Sets up the renderList_ vector by filling it with entities and the player
    void SetupRenderList();
    // Perform one pass of a sort of sprites by Y value to emulate orthogonal view
    void RenderSortPass();
    // Removes a sprite from the render list.
    void RemoveSpriteFromRenderList(const Sprite* sprite);
    // Sets up the Lua UI system and loads the UI Lua files
    void SetupUIScript();
    // Check to see if a sprite is colliding with the collision box of an entity
    bool EntityCollisionCheck(Sprite* sprite);
    // Check to see if point x,y is inside of rectangle defined by left, top, right, and bottom params
    bool CheckPoint(float x, float y, float left, float top, float right, float bottom);
    // Checks the distance between player and targeted entity and begins click action
    void InteractWithTarget();
    // Clears the current target
    void ClearTarget();
    // Remove an entity from the list of loaded entities
    void RemoveEntityFromLoaded(Entity* ent);
    // Locate an entity by its location. There are not supposed to be more than one entities at any
    //  given exact pixel coordinate.
    Entity* FindEntityByLocation(int x, int y);
    // Sets the experience of the player_ object, checks for level up, and updates UI experience information
    void UpdatePlayerExperience();
    // Checks if a harvesting cast is completed and acts accordingly
    void CheckHarvestCast(float dtime);
    // Sets a new harvest command in the list of harvest commands written to saved game
    void SetHarvestCommand(int x, int y, int clicks);
    // Sets a new farm command in the list of farm commands written to saved game
    void SetFarmCommand(int x, int y, const FarmCommand& fc);
    // DEPRECATED: print inventory to the UI console
    void PrintInventory();
    // convert harvestCommands_ to a vector of HarvestCommand and return the result
    std::vector<HarvestCommand> GetHarvestCommands();
    // convert farmCommands_ to a vector of FarmCommand and return the result
    std::vector<FarmCommand> GetFarmCommands();

    // C++ functions exposed to lua ///////////////////////////////////////////

    // Returns a lua array of {name=...,count=...} data
    static int lua_GetInventory(lua_State* L);
    // Called by inventory frame, attempts to convert a given item into foodstuff
    static int lua_ConvertItemToFoodstuff(lua_State* L);
    // Gets the amount of foodstuff currency
    static int lua_GetFoodstuffCount(lua_State* L);

    // Core game configuration
    Configuration* configuration_ = nullptr;
    // The map currently loaded
    TileMap* tileMap_ = nullptr;
    // Represents the location and image of the player in the world  
    Sprite* playerSprite_ = nullptr;
    // Places a swim filter over sprites that are swimming
    SwimFilter* swimFilter_ = nullptr;
    // The entity being targeted (if any) by the user
    Entity* targetedEntity_ = nullptr;
    // The visual target info
    Target* target_ = nullptr;
    // True if a casting sequence was started.
    bool harvesting_ = false;
    // The number of seconds that a cast must occur before being completed
    float maxCastTime_ = 0.f;
    // Counts the number of seconds current cast attempt has been going
    float currentCastTime_ = 0.f;
    // Sound channel harvest sound is being played on. Needed to cancel playback of sound. -1 indicates
    //  an invalid channel and is ignored.
    int harvestSoundChannel_ = -1;
    // Player data. TODO: refactor
    Player* player_;
    // harvest commands. value is number of clicks
    std::unordered_map<ENT_COORDS, int> harvestCommands_;
    // farm commands
    std::unordered_map<ENT_COORDS, FarmCommand> farmCommands_;
    // Save game data.
    SaveData* saveData_;
    // State of game
    GAME_STATE gameState_ = GAME_STATE::SPLASH;
    // Autosave timer
    static constexpr float AUTOSAVE_FREQUENCY = 100.0f; // in seconds
    float autosaveTimer_ = 0.0f;
    // Splash screen main menu
    SplashScreen* splashScreen_ = nullptr;
    // which slot to use for current game
    std::string saveSlot_ = "slot0"; // by default
    // toggles the inventory window
    bool showingInventory_ = false;
    // Camera coordinates to determine where on screen objects are rendered. Render functions that take
    // a camera vector should receive a negated camera parameter.
    glm::vec3 camera_ = {0.f,0.f,0.f};
    // list of all sprites to try to render including player. Does not own pointers
    std::vector<Sprite*> renderList_;
    // list of loaded map entities. Owns pointers
    std::vector<Entity*> loadedEntities_;
    // lua state for running the UI system
    lua_State* uiScript_ = nullptr;
    // bindings to C++ UI library
    engine::ui::LuaBindings* luaBindings_ = nullptr;
};
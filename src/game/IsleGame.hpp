// TileGame.hpp
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

#include <vector>
#include <unordered_map>

#include "combat/BattleSystem.hpp"
#include "Configuration.hpp"
#include "engine/Game.hpp"
#include "Inventory.hpp"
#include "Keybinds.hpp"
#include "MobSpawner.hpp"
#include "MobSprite.hpp"
#include "ogl/Texture.hpp"
#include "PlayerCommand.hpp"
#include "PlayerSprite.hpp"
#include "SaveData.hpp"
#include "SplashScreen.hpp"
#include "SwimFilter.hpp"
#include "Target.hpp"
#include "UISystem.hpp"
#include "world/TileMap.hpp"

namespace game
{
    struct ENT_COORDS
    {
        int x=0, y=0;
        bool operator==(const ENT_COORDS& ec) const 
            { return x==ec.x && y == ec.y; }
    };
}

// needed to use ENT_COORDS as an unordered_map key
namespace std { template <> struct hash<game::ENT_COORDS> {
    size_t operator() (const game::ENT_COORDS& ec) const {
        return ((size_t)ec.x<<32)|ec.y;
    }
}; }

namespace game
{
    // IsleGame class. See docs/* and README.md for details.
    class IsleGame : public engine::Game 
    {
    public:
        // determines what Update and Render does
        enum GAME_STATE { SPLASH, PLAYING, RETURNING_TO_MENU };
        // constructor
        IsleGame();
        // destructor
        ~IsleGame();
        // initializes the main menu UI and SDL event callbacks
        bool Initialize();
        // writes the save game to file unless game is exited at main menu
        void Cleanup();
        // update the game world
        void Update(float dtime);
        // render either the main menu or game world depending on game state
        void Render(engine::GraphicsContext& gc);

        // returns a reference to the inventory object
        inline Inventory& GetInventory() { return inventory_; }
        // sets the game state
        inline void SetGameState(GAME_STATE gs) { gameState_ = gs; }
        // get the playerSprite
        inline PlayerSprite& GetPlayerSprite() { return *playerSprite_; }
        // get save slot name
        inline std::string GetSaveSlot() { return saveSlot_; }

    private:

        // menu handling and starting /////////////////////////////////////////////

        // This is called once the main menu is exited due to a new game being started
        // or a game file being selected for loading. Sets up core game world data 
        void StartGame();
        // Loads a game specified by saveSlot_ and fills in world and player data
        void LoadGame();
        // Saves a game to slot specified by saveSlot_
        void SaveGame();
        // Fills game world with data from configuration file instead of loading from savegame
        void NewGame(bool boy);
        // Cleans up data to prepare for return to main menu
        void EndGame();
        // Rebuilds splash screen object and sets game state back to SPLASH SCREEN
        void ReturnToMainMenu();
        // Sets up keybinds based on ???
        void SetupKeybinds();

        // load a sprite and animations from lost guardian folder
        PlayerSprite* LoadPlayerLGSpr(const std::string& name, int w, int h, bool boy, int level, int exp);
        // unload textures associated with lost guardian sprite and destroy sprite
        void UnloadPlayerLGSpr(PlayerSprite*& sprite, const std::string& name);
        // Checks if a given sprite is "Swimming" that is, its base is on a "liquid" tile
        bool SpriteIsSwimming(Sprite* sprite);
        // Destroy all entities
        void CleanupLoadedEntities();
        // Destroy mob spawners
        void CleanupMobSpawners();
        // Sets up the renderList_ vector by filling it with entities and the player
        void SetupRenderList();
        // Perform one pass of a sort of sprites by Y value to emulate orthogonal view
        void RenderSortPass();
        // Check tile collision on a sprite
        void CheckTileCollision(Sprite* sprite);
        // Check mobs for death and loot
        void CheckMobDeaths();
        // Removes a sprite from the render list.
        void RemoveSpriteFromRenderList(const Sprite* sprite);
        // Check to see if a sprite is colliding with the collision box of an entity
        bool EntityCollisionCheck(Sprite* sprite);
        // Check to see if point x,y is inside of rectangle defined by left, top, right, and bottom params
        bool CheckPoint(float x, float y, float left, float top, float right, float bottom);
        // Checks the distance between player and targeted entity and begins click action
        void InteractWithEntity(Entity* ent);
        // Stop playing harvesting sound
        void StopHarvestSound();
        // Clear target
        void ClearTarget();
        // Remove an entity from the list of loaded entities
        void RemoveEntityFromLoaded(Entity* ent);
        // Locate an entity by its location. There are not supposed to be more than one entities at any
        //  given exact pixel coordinate.
        Entity* FindEntityByLocation(int x, int y);
        // Sets the experience of the player_ object, checks for level up, and updates UI experience information
        void UpdatePlayerExperience(bool dinged);
        // Checks if a harvesting cast is completed and acts accordingly
        void CheckHarvestCast(float dtime);
        // Sets a new harvest command in the list of harvest commands written to saved game
        void SetHarvestCommand(int x, int y, int clicks);
        // Sets a new farm command in the list of farm commands written to saved game
        void SetFarmCommand(int x, int y, const FarmCommand& fc);
        // Removes an individual farm command such as when a farmable is ready for pickup
        void RemoveFarmCommand(int x, int y);
        // convert harvestCommands_ to a vector of HarvestCommand and return the result
        std::vector<HarvestCommand> GetHarvestCommands();
        // convert farmCommands_ to a vector of FarmCommand and return the result
        std::vector<FarmCommand> GetFarmCommands();

        // Core game configuration
        Configuration& configuration_;
        // The map currently loaded
        world::TileMap* tileMap_ = nullptr;
        // Represents the location and image of the player in the world  
        PlayerSprite* playerSprite_ = nullptr;
        // Inventory (shared across playable classes)
        Inventory inventory_;
        // Places a swim filter over sprites that are swimming
        SwimFilter* swimFilter_ = nullptr;
        // The complete target info
        Target target_;
        // True if a casting sequence was started.
        bool harvesting_ = false;
        // The number of seconds that a cast must occur before being completed
        float maxCastTime_ = 0.f;
        // Counts the number of seconds current cast attempt has been going
        float currentCastTime_ = 0.f;
        // Sound channel harvest sound is being played on. Needed to cancel playback of sound. -1 indicates
        //  an invalid channel and is ignored.
        int harvestSoundChannel_ = -1;
        // harvest commands. value is number of clicks
        std::unordered_map<ENT_COORDS, int> harvestCommands_;
        // farm commands
        std::unordered_map<ENT_COORDS, FarmCommand> farmCommands_;
        // Save game data.
        SaveData saveData_;
        // State of game
        GAME_STATE gameState_ = GAME_STATE::SPLASH;
        // Autosave frequency
        static constexpr float AUTOSAVE_FREQUENCY = 120.0f; // in seconds
        // Autosave timer
        float autosaveTimer_ = 0.0f;
        // Splash screen main menu
        SplashScreen* splashScreen_ = nullptr;
        // which slot to use for current game
        std::string saveSlot_ = "slot0"; // by default
        // toggles the inventory window
        bool showingInventory_ = false;
        // Camera coordinates to determine where on screen objects are rendered. Render functions that take
        // a camera vector should receive a negated camera parameter.
        glm::vec2 camera_ = {0.f,0.f};
        // list of all sprites to try to render including player. Does not own pointers
        std::vector<Sprite*> renderList_;
        // list of loaded map entities. Owns pointers
        std::vector<Entity*> loadedEntities_;
        // list of mob spawners. Owns pointers.
        std::vector<MobSpawner*> mobSpawners_;
        // list of mob sprites. Owns pointers
        std::vector<MobSprite*> mobSprites_;
        // Battle system to keep track of what is in combat and process moves
        combat::BattleSystem* battleSystem_;
        // Lua ui system object
        UISystem* uiSystem_;
        // Keybind object
        Keybinds keybinds_;
    };

}
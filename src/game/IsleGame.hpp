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

#include "combat/Battle.hpp"
#include "Configuration.hpp"
#include "Crafting.hpp"
#include "engine/Game.hpp"
#include "Harvesting.hpp"
#include "Inventory.hpp"
#include "ItemTable.hpp"
#include "Keybinds.hpp"
#include "MobSpawner.hpp"
#include "MobSprite.hpp"
#include "ogl/Texture.hpp"
#include "PlayerSprite.hpp"
#include "Persistence.hpp"
#include "SplashScreen.hpp"
#include "SwimFilter.hpp"
#include "Target.hpp"
#include "UserInterface.hpp"
#include "world/TileMap.hpp"

namespace game
{
    // IsleGame class. See docs/* and README.md for details.
    class IsleGame : public engine::Game 
    {
    public:
        // determines what Update and Render does
        enum GAME_STATE { SPLASH, PLAYING, RETURNING_TO_MENU };
        // what cast action the player is doing (only can do one at a time)
        enum class PlayerAction 
        { 
            None, // no cast bar action going on
            Harvesting, // includes "farm" type harvesting
            Crafting, // when crafting an item 
            Building, // attempting to place a building on the map
            // Spell casting has to be handled by combat system because NPCs can also
            //  cast spells. But there is only one cast bar and user can only do one.
        };

        // methods implementing engine::Game //////////////////////////////////

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

        // accessors //////////////////////////////////////////////////////////

        // returns a reference to the inventory object (not const because inventory list is always changing)
        inline Inventory& GetInventory() { return *inventory_; }
        // get the item database
        inline const ItemTable& GetItemTable() const { return itemTable_; }
        // returns a reference to the crafting system (const because crafting database never changes0)
        inline const Crafting& GetCrafting() const { return crafting_; }
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

        // Other methods //////////////////////////////////////////////////////

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
        // Sets up the renderList_ vector by filling it with entities and the player (mobs are added individually
        //  as needed later)
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
        // Stop playing action sound
        void StopActionSound();
        // Clear target
        void ClearTarget();
        // Remove an entity from the list of loaded entities
        void RemoveEntityFromLoaded(Entity* ent);
        // Locate an entity by its location. There are not supposed to be more than one entities at any
        //  given exact pixel coordinate.
        Entity* FindEntityByLocation(int x, int y);
        // Sets the experience of the player_ object, checks for level up, and updates UI experience information
        void UpdatePlayerExperience(bool dinged);
        // Checks if an action cast is completed and acts accordingly
        void CheckActionCast(float dtime);

        // Persistent objects that remain valid the entire IsleGame object life spand //

        // Core game configuration
        Configuration& configuration_;
        // Crafting system
        Crafting crafting_;
        // State of game
        GAME_STATE gameState_ = GAME_STATE::SPLASH;
        // Item database
        ItemTable itemTable_;
        // Keybind object
        Keybinds keybinds_;
        // Save game data.
        Persistence saveData_;
        // Player action enum to determine meaning of mouse clicks
        PlayerAction playerAction_;
        // The complete target info
        Target target_;

        // Dynamic objects reset between start and end game ///////////////////

        // Battle system to keep track of what is in combat and process moves
        combat::Battle* battle_;
        // Harvest system to keep track of map modifications
        Harvesting* harvesting_;
        // Inventory (shared across playable classes)
        Inventory* inventory_;
        // Represents the location and image of the player in the world  
        PlayerSprite* playerSprite_ = nullptr;
        // Splash screen main menu
        SplashScreen* splashScreen_ = nullptr;
        // Places a swim filter over sprites that are swimming.
        SwimFilter* swimFilter_ = nullptr;
        // Lua ui system object
        UserInterface* userInterface_;
        // The map currently loaded
        world::TileMap* tileMap_ = nullptr;

        // various members ////////////////////////////////////////////////////

        // The required amount of time for the current casting action
        float maxCastTime_ = 0.f;
        // Counts the number of seconds current cast attempt has been going
        float currentCastTime_ = 0.f;
        // Sound channel player action sound is being played on. Needed to cancel playback of sound. -1 indicates
        //  an invalid channel and is ignored. TODO: a class to organize playing of different types of sounds
        //  according to player action
        int actionSoundChannel_ = -1;
        // Autosave frequency
        static constexpr float AUTOSAVE_FREQUENCY = 120.0f; // in seconds
        // Autosave timer
        float autosaveTimer_ = 0.0f;
        // which slot to use for current game
        std::string saveSlot_ = "slot0"; // by default
        // toggles the inventory window
        bool showingInventory_ = false;
        // Camera coordinates to determine where on screen objects are rendered. Render functions that take
        //  a camera vector should receive a negated camera parameter.
        glm::vec2 camera_ = {0.f,0.f};
        // list of all sprites to try to render including player. Does not own pointers
        std::vector<Sprite*> renderList_;
        // list of loaded map entities. Owns pointers
        std::vector<Entity*> loadedEntities_;
        // list of mob spawners. Owns pointers.
        std::vector<MobSpawner*> mobSpawners_;
        // list of mob sprites. Owns pointers
        std::vector<MobSprite*> mobSprites_;
    };

}
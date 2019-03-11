// TileGame.cpp
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

#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.hpp"
#include "engine/ui/Root.hpp"
#include "IsleGame.hpp"

namespace game
{

    IsleGame::IsleGame() : configuration_(Configuration::Get())
    { 

    }

    IsleGame::~IsleGame()
    {

    }

    bool IsleGame::Initialize()
    {
        // initialize UIRoot first always    
        engine::ui::Root::Get()->Initialize();

        // Initialize splash screen ui
        splashScreen_ = new SplashScreen();
        splashScreen_->Initialize();

        // initialize callbacks
        engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e){
            // nothing to do here on splash screen (UIRoot handles it all)
            if(gameState_ == GAME_STATE::SPLASH)
            {
                return;
            }
            if(e.type == SDL_KEYDOWN)
            {
                std::string combatLog;
                // TODO: customize keybinds with UI (except movement keys and mouse btns)
                glm::vec2 vel = playerSprite_->velocity;
                std::string log;
                switch(e.keysym.sym)
                {
                    case SDLK_LEFT: case SDLK_a:
                        if(playerSprite_->GetCurrentAnim() != "left_walk")
                            playerSprite_->SetCurrentAnim("left_walk", 0.2f);
                        vel.x = -1.f;
                        break;
                    case SDLK_RIGHT: case SDLK_d:
                        if(playerSprite_->GetCurrentAnim() != "right_walk")
                            playerSprite_->SetCurrentAnim("right_walk", 0.2f); 
                        vel.x = 1.f; 
                        break;
                    case SDLK_UP: case SDLK_w:
                        if(playerSprite_->GetCurrentAnim() != "back_walk")
                            playerSprite_->SetCurrentAnim("back_walk", 0.2f); 
                        vel.y = -1.f; 
                        break;
                    case SDLK_DOWN: case SDLK_s:
                        if(playerSprite_->GetCurrentAnim() != "front_walk")
                            playerSprite_->SetCurrentAnim("front_walk", 0.2f); 
                        vel.y = 1.f; 
                        break;
                    default:
                        keybinds_.RunKeybind(e.keysym.sym);
                        // without this the entire screen except UI goes black......
                        // TODO: fix weird velocity bug?
                        return;
                }
                if(vel.length() != 0)
                {
                    // movement interrupts any type of cast
                    // TODO: handle spell casting separately because sound will be based on
                    //  animation and ability info.
                    if(playerAction_ != PlayerAction::None)
                    {
                        playerAction_ = PlayerAction::None;
                        userInterface_->ToggleCastBar(false);
                        userInterface_->WriteLineToConsole("Action interrupted by player", 1.f, 0.f, 0.f, 0.9f);
                        if(actionSoundChannel_ != -1)
                        {
                            engine::GameEngine::Get().GetSoundManager().HaltSound(actionSoundChannel_);
                            actionSoundChannel_ = -1;
                        }
                    }
                    float PLAYER_SPEED = 50.0f; // sane default
                    configuration_.GetVar("PLAYER_SPEED", PLAYER_SPEED);
                    playerSprite_->StartAnimation();
                    // TODO: find out if the velocity bug is caused by an almost zero non-zero here?
                    if(vel.y != 0)
                        vel.y /= glm::abs(vel.y); // set to 1 or -1
                    if(vel.x != 0)
                        vel.x /= glm::abs(vel.x); // set to 1 or -1
                    // TODO: account for buffs/debuffs (multipliers against SPD stat)
                    vel = glm::normalize(vel) * PLAYER_SPEED;
                }
                playerSprite_->velocity = vel;
                if(SpriteIsSwimming(playerSprite_))
                {
                    playerSprite_->velocity /= 2.0f;
                }
            }
            else 
            {
                // TODO: stop movement based on key up
                playerSprite_->PauseAnimation();
                playerSprite_->velocity = {0.f,0.f};
            }
        });

        engine::GameEngine::Get().AddMouseButtonListener([this](const SDL_MouseButtonEvent& e){
            // nothing to do here on main menu screen. (UIRoot handles all of that)
            if(gameState_ == GAME_STATE::SPLASH)
                return;
            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                if(e.button == 1) // primary mouse button
                {
                    int clickedX = e.x;
                    int clickedY = e.y;
                    engine::GameEngine::Get().SetLogicalXY(clickedX, clickedY);
                    // TODO: look for a mob to target first
                    auto mobFound = std::find_if(mobSprites_.begin(), mobSprites_.end(), 
                        [this, clickedX, clickedY](const MobSprite* mob) {
                            float x = camera_.x + (float)clickedX;
                            float y = camera_.y + (float)clickedY;
                            auto pos = mob->position;
                            float w = (float)mob->GetWidth();
                            float h = (float)mob->GetHeight();
                            return (x > pos.x && x < pos.x + w && y > pos.y && y < pos.y + h);
                        }
                    );
                    if(mobFound != mobSprites_.end())
                    {
                        // if we clicked a mob already being targeted, use the right hand attack on it
                        // (or at least try to)
                        //
                        if((Sprite*)*mobFound == target_.GetTargetSprite())
                        {
                            // TODO: generic right-hand weapon calls instead of direct ability name
                            std::string log = battle_->UsePlayerAbility("unarmed_right", target_);
                            userInterface_->WriteLineToConsole(log);
                        }
                        else
                        {
                            // Set up the target_ object data
                            auto aggro = (*mobFound)->GetAggroType();
                            Target::TARGET_TYPE tt = Target::TARGET_TYPE::FRIENDLY; // default value
                            switch(aggro) 
                            { 
                                case MobType::AGGRO_TYPE::HOSTILE: tt=Target::TARGET_TYPE::HOSTILE; break; 
                                case MobType::AGGRO_TYPE::NEUTRAL: tt=Target::TARGET_TYPE::NEUTRAL; break;
                            }
                            target_.SetTargetSprite(*mobFound, tt, Target::SPRITE_TYPE::MOBSPR);
                            // player actions are interrupted by switching targets except for spell casting
                            playerAction_ = PlayerAction::None;
                            // Set the unit frame data.
                            //  not possible to use existing enums due to circular dependencies generating
                            //  arcane compile errors so we just send strings as argument
                            userInterface_->TargetUnitFrame_SetNameAndLevel((*mobFound)->GetCombatUnit().GetName(), 
                                (*mobFound)->GetCombatUnit().GetAttributeSheet().GetLevel());
                            std::string hostility = "friendly"; // default value
                            switch(tt)
                            {
                                case Target::TARGET_TYPE::HOSTILE: hostility = "hostile"; break;
                                case Target::TARGET_TYPE::NEUTRAL: hostility = "neutral"; break;
                            }
                            userInterface_->TargetUnitFrame_SetHealth(
                                (*mobFound)->GetCombatUnit().GetCurrentHealth(), 
                                (*mobFound)->GetCombatUnit().GetMaxHealth(), hostility);
                            userInterface_->TargetUnitFrame_Toggle(true);
                        }
                    }
                    else
                    {
                        // clear unit frame in case switching from a mob to entity target
                        ClearTarget();
                        // look for an entity to target.
                        auto found = std::find_if(loadedEntities_.begin(), loadedEntities_.end(), 
                            [this, clickedX, clickedY](const Entity* ent) {
                                float x = camera_.x + (float)clickedX;
                                float y = camera_.y + (float)clickedY;
                                return (
                                    x > ent->position.x && x < ent->position.x + (float)ent->GetWidth() &&
                                    y > ent->position.y && y < ent->position.y + (float)ent->GetHeight()
                                );
                            }
                        );
                        // if we found one, set it as the target and begin interacting with it
                        if(found != loadedEntities_.end())
                        {
                            target_.SetTargetSprite(*found, Target::TARGET_TYPE::FRIENDLY, 
                                Target::SPRITE_TYPE::ENTSPR);
                            // TODO: print more information about the entity to console
                            if((*found)->GetMaxClicks() != -1)
                            {
                                std::string info = (*found)->GetName() + " (" +
                                    std::to_string((*found)->GetRemainingClicks()) +
                                    "/" + std::to_string((*found)->GetMaxClicks()) + ")";
                                userInterface_->WriteLineToConsole(info);
                            }
                            if((*found)->IsFarmable())
                            {
                                std::string info = "Ready for pickup ";
                                if(!(*found)->IsReadyForPickup())
                                {
                                    time_t timeRemaining = (*found)->FarmTimeRemaining();
                                    info += "in " +
                                        engine::GameEngine::Get().FormatTimeInSeconds((int)timeRemaining);
                                }
                                userInterface_->WriteLineToConsole(info);
                            }
                            InteractWithEntity(*found);
                            return;
                        }
                        // TODO: else-if statement here to handle buildings?
                        else // no target was found so cancel action and clear existing target
                        {
                            playerAction_ = PlayerAction::None;
                            ClearTarget();
                        }
                    }
                }
                else if(e.button == 3)
                {
                    // use our left hand ability on the target. (Targeting can only be done with
                    // primary mouse button)
                    // TODO: implement function that uses generic left hand ability of equipped weapon 
                    std::string log = battle_->UsePlayerAbility("unarmed_left", target_);
                    userInterface_->WriteLineToConsole(log);
                }
            }
        });

        engine::GameEngine::Get().AddQuitListener([this](const SDL_QuitEvent& e)->bool{
            // if returning to menu or on splash, nothing to check
            if(gameState_ == GAME_STATE::RETURNING_TO_MENU || gameState_ == GAME_STATE::SPLASH)
                return true;
            // otherwise have to check for things like in combat
            if(gameState_ == GAME_STATE::PLAYING)
            {
                if(playerSprite_ != nullptr)
                {
                    if(playerSprite_->GetPlayerCombatUnit().IsInCombat())
                    {
                        userInterface_->WriteLineToConsole("Cannot exit game while in combat.", 
                            1.0f, 0.0f, 0.0f, 1.0f);
                        return false;
                    }
                }
            }
            return true;
        });

        return true;
    }

    void IsleGame::Cleanup()
    {
        EndGame();
    }

    void IsleGame::Update(float dtime)
    {
        // transition splash states if needed
        if(splashScreen_)
        {
            if(splashScreen_->GetScreenState() == SplashScreen::SCREEN_STATE::ENDSPLASH)
            {
                StartGame();
            }
        }
        // check state
        if(gameState_ == GAME_STATE::SPLASH)
        {
            splashScreen_->Update(dtime);
        }
        else if(gameState_ == GAME_STATE::RETURNING_TO_MENU)
        {
            EndGame();
            ReturnToMainMenu();
        }
        else
        {
            // check autosave timer
            autosaveTimer_ += dtime;
            if(autosaveTimer_ >= AUTOSAVE_FREQUENCY)
            {
                autosaveTimer_ -= AUTOSAVE_FREQUENCY;
                userInterface_->WriteLineToConsole("Autosaving...", 0.6f, 0.6f, 0.6f, 1.0f);
                SaveGame();
            }

            // update player movement
            playerSprite_->Update(dtime);
            
            // update health on UI
            userInterface_->PlayerUnitFrame_SetHealth(playerSprite_->GetPlayerCombatUnit().GetCurrentHealth(), 
                playerSprite_->GetPlayerCombatUnit().GetMaxHealth());

            // update entities.
            for(auto each : loadedEntities_)
            {
                each->Update(dtime);
                // discard previous farm commands once an entity becomes ready for pick up.
                if(each->IsFarmable() && each->IsReadyForPickup())
                {
                    harvesting_->RemoveFarmCommand((int)each->position.x, (int)each->position.y);
                }
            }

            // check player action casting and update cast bar
            CheckActionCast(dtime);

            // check collisions with tiles
            CheckTileCollision(playerSprite_);
            CheckTileCollision(playerSprite_);
            // check entity collisions
            if (EntityCollisionCheck(playerSprite_))
                playerSprite_->ReverseMovement(dtime);
            // update mobs
            for(auto mob : mobSprites_)
            {
                mob->Update(dtime);
                CheckTileCollision(mob);
            }
            // update mob spawners.
            for(auto it : mobSpawners_)
            {
                MobSprite* mobSprite = nullptr;
                it->Update(dtime, mobSprite, configuration_, mobSprites_);
                if(mobSprite)
                {
                    mobSprites_.push_back(mobSprite);
                    renderList_.push_back(mobSprite);
                }
            }

            // update target information
            if(target_.GetTargetSpriteType() == Target::SPRITE_TYPE::MOBSPR)
            {
                MobSprite* target = (MobSprite*)target_.GetTargetSprite();
                userInterface_->TargetUnitFrame_SetHealth(
                    target->GetCombatUnit().GetCurrentHealth(),
                    target->GetCombatUnit().GetMaxHealth(), "");
            }

            // run one pass of despawning at least one out of range mobsprite.
            // TODO: overworld despawn distance should be configuration data
            const float MAX_DISTANCE = 2048.0f;
            auto eachMobIt = mobSprites_.begin();
            for (; eachMobIt != mobSprites_.end(); ++eachMobIt)
            {
                auto mobPos = (*eachMobIt)->position;
                auto playerPos = playerSprite_->position;
                float distance = glm::distance(mobPos, playerPos);
                if(distance > MAX_DISTANCE)
                {
                    break;
                }
            }
            if(eachMobIt != mobSprites_.end())
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                    "Despawning mob because player is too far away");
                // remove from render list
                RemoveSpriteFromRenderList(*eachMobIt);
                // remove from battle system
                battle_->RemoveMob(*eachMobIt);
                // delete the pointer
                delete *eachMobIt;
                // finally remove from iterator list
                mobSprites_.erase(eachMobIt);
            }

            // update spell animations made last cycle
            battle_->UpdateAnimations(dtime);

            // calculate battle AI systems
            auto logs = battle_->CalculateMoves();
            for(const auto& eachEntry : logs)
            {
                // TODO: different combat text color than white?
                userInterface_->WriteLineToConsole(eachEntry);
            }

            // pass through once to check for dead mobs and give EXP based on scale. 
            CheckMobDeaths();

            // if player died reset to spawn point for now
            if(playerSprite_->GetPlayerCombatUnit().GetCurrentHealth() == 0)
            {
                userInterface_->WriteLineToConsole("You died.", 0.85f, 0.f, 0.f, 1.f);
                int ix, iy;
                std::vector<int> SPAWN_POINT;
                configuration_.GetVar("SPAWN_POINT", SPAWN_POINT);
                ix = SPAWN_POINT[0];
                iy = SPAWN_POINT[1];
                playerSprite_->position = { (float)(tileMap_->GetTileSet()->GetTileWidth() * ix),
                    (float)(tileMap_->GetTileSet()->GetTileHeight() * iy) };
                // and set health to 1/3 of max
                playerSprite_->GetPlayerCombatUnit().SetCurrentHealth((int)(0.33 * 
                    (double)playerSprite_->GetPlayerCombatUnit().GetMaxHealth()));
                // clear target
                ClearTarget();
            }

            // update cooldown indicators in UserInterface. For now left side = LMB, right side=RMB
            //  even though that is slightly backwards. Maybe change the UI file?
            userInterface_->LeftHandFrame_SetValue( 1.0f -
                playerSprite_->GetPlayerCombatUnit().GetRemainingRightCooldownAsValue());
            userInterface_->RightHandFrame_SetValue( 1.0f -
                playerSprite_->GetPlayerCombatUnit().GetRemainingLeftCooldownAsValue());

            // TODO: bound player to map area

            // calculate camera.
            float scrW = (float)engine::GameEngine::Get().GetWidth();
            float scrH = (float)engine::GameEngine::Get().GetHeight();
            glm::vec2 playerPos = playerSprite_->position;

            camera_.x = playerPos.x - scrW / 2.f;
            camera_.y = playerPos.y - scrH / 2.f;

            // TODO: bound camera to map area if map dimensions are bigger than screen else center map
            /*if(camera_.x < 0.0f)
                camera_.x = 0.0f;
            if(camera_.y < 0.0f)
                camera_.y = 0.0f;*/
        }

        engine::ui::Root::Get()->Update(dtime);
    }

    void IsleGame::Render(engine::GraphicsContext& gc)
    {
        // disable for 2D rendering with semi-transparent data
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_DEPTH_TEST);

        ogl::Program& program = gc.GetProgram();

        if(gameState_ == GAME_STATE::SPLASH)
        {
            splashScreen_->Render(program);
        }
        else 
        {
            program.Use();

            // colors are from texture data only
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
            // NOTE: unpolished tile edges look glitchy
            tileMap_->Render(-camera_.x,-camera_.y, program);
            // render the visual target before the actual sprites being targeted
            target_.Render(-camera_, program);
            RenderSortPass();
            for(auto it=renderList_.begin(); it != renderList_.end(); ++it)
            {
                (*it)->Render(-camera_, program);
                if(SpriteIsSwimming(*it))
                {
                    swimFilter_->SetSpriteData(*it);
                    swimFilter_->Render(-camera_, program);
                }
            }
            battle_->RenderAnimations(-(int)(camera_.x), -(int)(camera_.y), program);
        }
        engine::ui::Root::Get()->Render(gc);
    }

    void IsleGame::StartGame()
    {
        // Change the game state
        gameState_ = GAME_STATE::PLAYING;
        // Load map and set up entities
        std::string STARTING_MAP;
        configuration_.GetVar("STARTING_MAP", STARTING_MAP);
        tileMap_ = new world::TileMap(STARTING_MAP);
        loadedEntities_ = tileMap_->GenerateEntities();
        mobSpawners_ = tileMap_->GenerateSpawners();
        // create harvest system
        harvesting_ = new Harvesting();
        // clear target
        target_.SetTargetSprite(nullptr, Target::TARGET_TYPE::NEUTRAL, Target::SPRITE_TYPE::NONE);
        // get menu information to determine save or load game data then destroy 
        // the splash screen
        auto gameLoadState = splashScreen_->GetGameLoadState();
        splashScreen_->Cleanup();
        delete splashScreen_;
        splashScreen_ = nullptr;
        // name of save slot and determine
        // survivalist gender when spawning new player sprite.
        saveSlot_ = gameLoadState.saveName;
        // Setup lua UI
        userInterface_ = new UserInterface(*this);
        // either start a new game or load new game according to menu option
        if(gameLoadState.newGame)
            NewGame(gameLoadState.boyCharacter);
        else 
            LoadGame();
        // Set player sprite common info
        playerSprite_->SetCollisionBox(5.f, (float)playerSprite_->GetWidth() / 2.f, 
            (float)playerSprite_->GetWidth()-5.f, (float)playerSprite_->GetHeight());
        playerSprite_->SetCurrentAnim("front_stand", 0.2f);
        playerSprite_->StartAnimation();
        // Setup swim filter
        swimFilter_ = new SwimFilter(tileMap_->GetSwimmingTexture());
        // Setup render list
        SetupRenderList();
        // Update experience bar to reflect current xp levels
        UpdatePlayerExperience(false);
        // load sounds
        auto& sm = engine::GameEngine::Get().GetSoundManager();
        //  TODO: differentiate action types and sounds in configuration
        std::string ACTION_SOUND;
        configuration_.GetVar("ACTION_SOUND", ACTION_SOUND);
        sm.LoadSound(ACTION_SOUND);
        // start looping background music. TODO: fix SoundManager to get more song variety
        sm.PlayMusic("res/music/island1.ogg", -1);
        // setup battle system
        battle_ = new combat::Battle();
        battle_->AddPlayer(playerSprite_);
        // set UI information
        userInterface_->PlayerUnitFrame_SetNameAndLevel(saveSlot_, 
            playerSprite_->GetPlayerCombatUnit().GetAttributeSheet().GetLevel());
        userInterface_->PlayerUnitFrame_SetHealth(playerSprite_->GetPlayerCombatUnit().GetCurrentHealth(), 
            playerSprite_->GetPlayerCombatUnit().GetMaxHealth());
        userInterface_->BuildInventory();
        userInterface_->SetFoodstuffBarData(inventory_.GetItemAmount("foodstuff"));
        // setup keybinds
        SetupKeybinds();
    }

    void IsleGame::LoadGame()
    {
        bool loaded = saveData_.ReadFromFile(saveSlot_);
        if(!loaded) // the main menu should generally prevent loading invalid files
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "%s: Attempt to load invalid save file!", __FUNCTION__);
        }
        else
        {
            // create the sprite
            auto playerData = saveData_.GetPlayerData();
            std::string BOY_SURV, GIRL_SURV;
            configuration_.GetVar("BOY_SURV", BOY_SURV);
            configuration_.GetVar("GIRL_SURV", GIRL_SURV);
            playerSprite_ = LoadPlayerLGSpr( playerData.boy ? BOY_SURV :
                GIRL_SURV, 32, 32, playerData.boy, 
                playerData.level, playerData.experience );
            // process locate command
            playerSprite_->position = {(float)saveData_.GetLocationCommand().locationX,
                (float)saveData_.GetLocationCommand().locationY};
        }
        // process harvest commands
        saveData_.ForEachHarvestCommand([this](const HarvestCommand& cmd){
            Entity* ent = FindEntityByLocation(cmd.targetX, cmd.targetY);
            if(ent == nullptr)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                    "%s: Unable to locate entity for harvest command at %d, %d", __FUNCTION__, 
                    cmd.targetX, cmd.targetY);
            }
            else
            {
                // TODO: make a constant for -1 indicating that it means non-harvestable
                if(ent->GetMaxClicks() != -1)
                {
                    ent->SetRemainingClicks(ent->GetMaxClicks() - cmd.count);
                    if(ent->GetRemainingClicks() <= 0)
                    {
                        RemoveSpriteFromRenderList(ent);
                        RemoveEntityFromLoaded(ent);
                    }
                }
                harvesting_->SetHarvestCommand(cmd.targetX, cmd.targetY, cmd.count);
            }
        });
        // process farm commands
        saveData_.ForEachFarmCommand([this](const FarmCommand& cmd){
            Entity* ent = FindEntityByLocation(cmd.targetX, cmd.targetY);
            if(ent == nullptr)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                    "%s: Unable to locate entity for farm command at %d, %d", __FUNCTION__,
                    cmd.targetX, cmd.targetY);
            }
            else
            {
                ent->SetFarmData(cmd);
                harvesting_->SetFarmCommand(cmd.targetX, cmd.targetY, FarmCommand(cmd.targetX, 
                    cmd.targetY, cmd.readyToFarm, cmd.farmedTime));
            }
        });
        // process inventory.
        inventory_.ClearItems();
        auto& items = saveData_.GetItemData();
        for(const auto& eachItemData : items)
        {
            inventory_.AddItemByName(eachItemData.name, eachItemData.count);
        }

        // log the timestamp of the last save time
        time_t currentTime = time(nullptr);
        time_t timeStamp = saveData_.GetTimeStamp();
        time_t secondsElapsed = currentTime - timeStamp;
        // TODO: format this time string
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
            "%ld seconds since last file save", (long)secondsElapsed);

        // discard save data
        saveData_.ClearData();
    }

    void IsleGame::SaveGame()
    {
        auto pos = playerSprite_->position;
        saveData_.SetLocationCommand(LocationCommand((int)pos.x, (int)pos.y));
        auto harvestCmds = harvesting_->GetHarvestCommands();
        for(auto each : harvestCmds)
        {
            saveData_.AddHarvestCommand(each);
        }
        auto farmCmds = harvesting_->GetFarmCommands();
        for(auto each : farmCmds)
        {
            saveData_.AddFarmCommand(each);
        }
        auto& itemData = saveData_.GetItemData();
        inventory_.ForEachItemEntry([&itemData](const std::string& name, const ItemEntry& ie){
            if(ie.count != 0)
                itemData.push_back({name, ie.count});
        });
        saveData_.SavePlayerData({
            // experience, level, gender
            playerSprite_->GetPlayerCombatUnit().GetCurrentExperience(),
            playerSprite_->GetPlayerCombatUnit().GetAttributeSheet().GetLevel(),
            playerSprite_->IsBoy()
        });
        saveData_.WriteToFile(saveSlot_);
    }

    void IsleGame::NewGame(bool boy)
    {
        int ix, iy;
        std::vector<int> SPAWN_POINT;
        configuration_.GetVar("SPAWN_POINT", SPAWN_POINT);
        ix = SPAWN_POINT[0];
        iy = SPAWN_POINT[1];
        std::string BOY_SURV, GIRL_SURV;
        configuration_.GetVar("BOY_SURV", BOY_SURV);
        configuration_.GetVar("GIRL_SURV", GIRL_SURV);
        playerSprite_ = LoadPlayerLGSpr(boy ? BOY_SURV :
            GIRL_SURV, 32, 32, boy, 1, 0);
        playerSprite_->position = {
            (float)ix * (float)tileMap_->GetTileSet()->GetTileWidth(),
            (float)iy * (float)tileMap_->GetTileSet()->GetTileHeight() };
        // clear inventory data
        inventory_.ClearItems();
    }

    void IsleGame::EndGame()
    {
        auto &sm = engine::GameEngine::Get().GetSoundManager();
        if(gameState_ != GAME_STATE::SPLASH)
        {
            delete battle_;
            battle_ = nullptr;
            delete userInterface_;
            userInterface_ = nullptr;
            keybinds_.Clear();
            SaveGame();
            // destroy Harvesting object
            delete harvesting_;
            harvesting_ = nullptr;
            std::string BOY_SURV, GIRL_SURV;
            configuration_.GetVar("BOY_SURV", BOY_SURV);
            configuration_.GetVar("GIRL_SURV", GIRL_SURV);
            UnloadPlayerLGSpr(playerSprite_, playerSprite_->IsBoy() ? 
                BOY_SURV: GIRL_SURV);
            delete swimFilter_;
            swimFilter_ = nullptr;
            CleanupLoadedEntities();
            CleanupMobSpawners();
            for(auto eachMob : mobSprites_)
            {
                delete eachMob;
            }
            mobSprites_.clear();
            delete tileMap_;
            tileMap_ = nullptr;
            sm.UnloadSound("res/sounds/chopping.wav");
        }
        inventory_.ClearItems();
        sm.StopMusic();
    }

    void IsleGame::ReturnToMainMenu()
    {
        splashScreen_ = new SplashScreen();
        splashScreen_->Initialize();
        gameState_ = GAME_STATE::SPLASH;
    }

    void IsleGame::SetupKeybinds()
    {
        keybinds_.AddKeybind(SDLK_i, [this](){
            showingInventory_ = !showingInventory_;
            userInterface_->ShowInventory(showingInventory_);
        });

        keybinds_.AddKeybind(SDLK_ESCAPE, [this](){
            userInterface_->ShowMMPopup(true);
        });

        keybinds_.AddKeybind(SDLK_c, [this]() {
            userInterface_->CraftingWindow_Toggle();
        });
    }

    // TODO: make a Lua utility function for this in the configuration file and make the 
    // player sprite selections fully data driven not hard coded like this
    PlayerSprite* IsleGame::LoadPlayerLGSpr(const std::string& name, int w, int h, bool boy, int level, int exp)
    {
        auto& tm = engine::GameEngine::Get().GetTextureManager();
        const std::string path = std::string("res/textures/sprites/last-guardian-sprites/") + name;
        ogl::Texture* texture = tm.GetTexture(path + "_fr1.gif");
        PlayerSprite* sprite = new PlayerSprite(texture, w, h, level, exp, boy);
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

    // TODO: See above function
    void IsleGame::UnloadPlayerLGSpr(PlayerSprite*& sprite, const std::string& name)
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
        RemoveSpriteFromRenderList(sprite);
        delete sprite;
        sprite = nullptr;
    }

    bool IsleGame::SpriteIsSwimming(Sprite* sprite)
    {
        // calculate the bottom center of sprite in world coordinates.
        auto pos = sprite->position;
        pos.x += (float)sprite->GetWidth() / 2.f;
        pos.y += (float)sprite->GetHeight();

        // determine tile x and y index.
        int tileX = (int)pos.x / tileMap_->GetTileSet()->GetTileWidth();
        int tileY = (int)pos.y / tileMap_->GetTileSet()->GetTileHeight();

        bool isSwimming = tileMap_->TileIsLiquid(tileX, tileY, false);
        isSwimming = isSwimming || tileMap_->TileIsLiquid(tileX, tileY, true);
        return isSwimming;
    }

    void IsleGame::CleanupLoadedEntities()
    {
        for(auto it=loadedEntities_.begin(); it != loadedEntities_.end(); ++it)
        {
            delete (*it);
        }
        loadedEntities_.clear();
    }

    void IsleGame::CleanupMobSpawners()
    {
        for(auto it=mobSpawners_.begin(); it != mobSpawners_.end(); ++it)
        {
            delete *it;
        }
        mobSpawners_.clear();
    }

    void IsleGame::SetupRenderList()
    {
        renderList_.clear();
        renderList_.push_back(playerSprite_);
        for(int i=0; i < loadedEntities_.size(); ++i)
        {
            renderList_.push_back(loadedEntities_[i]);
        }
    }

    void IsleGame::RenderSortPass()
    {
        // one pass of sorting. The larger the Y-base position, the later rendered
        for(int i=0; i < renderList_.size() - 1; ++i)
        {
            float y0 = renderList_[i]->position.y + (float)renderList_[i]->GetHeight();
            float y1 = renderList_[i+1]->position.y + (float)renderList_[i+1]->GetHeight();
            if(y0 > y1)
            {
                Sprite* swap = renderList_[i];
                renderList_[i] = renderList_[i+1];
                renderList_[i+1] = swap;
            }
        }
    }

    void IsleGame::CheckTileCollision(Sprite* sprite)
    {
        auto pos = sprite->position;
        float left, top, right, bottom;
        sprite->GetCollisionBox(left, top, right, bottom);
        float tileWidth = (float)tileMap_->GetTileSet()->GetTileWidth();
        float tileHeight = (float)tileMap_->GetTileSet()->GetTileHeight();
        //  check top left of sprite
        int ix = (int)(left / tileWidth);
        int iy = (int)(top / tileHeight);
        // is sprite's top left in a solid tile? check bottom right point of tile
        if(tileMap_->GetCollisionData(ix, iy))
        {
            float tileRight = (float)ix * tileWidth + tileWidth;
            float tileBottom = (float)iy * tileHeight + tileHeight;
            if(tileBottom - top >= tileRight - left) // x axis movement is smaller so do that
                pos.x += tileRight - left;
            else   // move along y axis because it's smaller difference
                pos.y += tileBottom - top;
            sprite->position = pos;
        }
        // check bottom left corner of sprite
        ix = (int)(left / tileWidth);
        iy = (int)(bottom / tileHeight);
        // is sprite's bottom left in a solid tile? check top right of tile
        if(tileMap_->GetCollisionData(ix, iy))
        {
            float tileRight = (float)ix * tileWidth + tileWidth;
            float tileTop = (float)iy * tileHeight + 0;
            if(tileRight - left >= bottom - tileTop) // y is smaller
                pos.y -= bottom - tileTop; // move above tile
            else 
                pos.x += tileRight - left; // move right
            sprite->position = pos;
        }
        // check top right of sprite
        ix = (int)(right / tileWidth);
        iy = (int)(top / tileHeight);
        // is sprite's top right in a solid tile? check bottom left of tile
        if(tileMap_->GetCollisionData(ix, iy))
        {
            float tileLeft = (float)ix * tileWidth + 0;
            float tileBottom = (float)iy * tileHeight + tileHeight;
            if(tileBottom - top >= right - tileLeft) // x is smaller
                pos.x -= right - tileLeft; // so move left
            else 
                pos.y -= tileBottom - top; // move up
            sprite->position = pos;
        }
        // check top right of sprite
        ix = (int)(right / tileWidth);
        iy = (int)(top / tileHeight);
        // is sprite's top right in a solid tile? check bottom left of tile
        if(tileMap_->GetCollisionData(ix, iy))
        {
            float tileLeft = (float)ix * tileWidth + 0;
            float tileBottom = (float)iy * tileHeight + tileHeight;
            if( right - tileLeft >= tileBottom - top) // y is smaller
                pos.y += tileBottom - top;
            else  
                pos.x -= right - tileLeft;
            sprite->position = pos;
        }
    }

    void IsleGame::CheckMobDeaths()
    {
        auto mobIt = mobSprites_.begin();
        for(;mobIt != mobSprites_.end(); ++mobIt)
        {
            if((*mobIt)->GetCombatUnit().GetCurrentHealth() == 0)
            {
                if((*mobIt)->KilledByPlayer())
                {
                    int level = (*mobIt)->GetCombatUnit().GetAttributeSheet().GetLevel();
                    int levelDiff = playerSprite_->GetPlayerCombatUnit().GetAttributeSheet().GetLevel() - level;
                    int BASE_MOB_EXP;
                    configuration_.GetVar("BASE_MOB_EXP", BASE_MOB_EXP);
                    float BASE_MOB_EXP_SCALE;
                    configuration_.GetVar("BASE_MOB_EXP_SCALE", BASE_MOB_EXP_SCALE);
                    float mult = pow(BASE_MOB_EXP_SCALE, (float)(level - 1));
                    int expEarned = (int)(mult * BASE_MOB_EXP);
                    // apply level difference penalty if mob is lower level
                    if(levelDiff > 0)
                    {
                        float MOB_EXP_PENALTY;
                        configuration_.GetVar("MOB_EXP_PENALTY", MOB_EXP_PENALTY);
                        float penaltyMult = pow(MOB_EXP_PENALTY, levelDiff);
                        expEarned = (int)((float)expEarned * penaltyMult);
                    }
                    bool dinged = playerSprite_->GetPlayerCombatUnit().AddExperience(expEarned);
                    userInterface_->WriteLineToConsole(std::string("You gained ") + std::to_string(expEarned) +
                        " exp!", 1.0f, 0.0f, 1.0f, 1.0f);
                    UpdatePlayerExperience(dinged);
                    // check loot table for possible loot
                    const auto& lootTable = (*mobIt)->GetLootTable();
                    for(const auto& entry : lootTable)
                    {
                        // TODO: handle "exp" as a special case item for mobs that drop bonus exp
                        bool itemDrops = engine::GameEngine::Get().PercentChance(entry.chance);
                        if(itemDrops)
                        {
                            userInterface_->WriteLineToConsole(std::string("You looted ") +
                                std::to_string(entry.count) + " " + entry.item);
                            inventory_.AddItemByName(entry.item, entry.count);
                            userInterface_->BuildInventory();
                        }
                    }
                }
                break;
            }
        }
        if(mobIt != mobSprites_.end())
        {
            RemoveSpriteFromRenderList(*mobIt);
            if(target_.GetTargetSprite() == (Sprite*)(*mobIt))
            {
                ClearTarget();
            }
            battle_->RemoveMob(*mobIt);
            delete *mobIt;
            mobSprites_.erase(mobIt);
        }
    }

    void IsleGame::RemoveSpriteFromRenderList(const Sprite* sprite)
    {
        auto it = std::find_if(renderList_.begin(), renderList_.end(), [sprite](const Sprite* spr){
            return sprite == spr;
        });
        if(it != renderList_.end())
            renderList_.erase(it);
    }

    Entity* IsleGame::FindEntityByLocation(int x, int y)
    {
        for(auto ent : loadedEntities_)
        {
            auto pos = ent->position;
            if((int)pos.x == x && (int)pos.y == y)
                return ent;
        }
        return nullptr;
    }

    bool IsleGame::EntityCollisionCheck(Sprite* sprite)
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

    bool IsleGame::CheckPoint(float x, float y, float left, float top, float right, float bottom)
    {
        return (x >= left && x <= right && y >= top && y <= bottom);
    }

    void IsleGame::InteractWithEntity(Entity* ent)
    {
        if(ent->IsFarmable() && !ent->IsReadyForPickup() 
        && ent->GetMaxClicks() == -1)
        {
            // the target is not interactive except when farmable or clickable
            StopActionSound();
            return; 
        }

        // check to see if we are in MAX_DISTANCE units of bottom center of targeted entity.
        const float MAX_DISTANCE = 48.f;
        glm::vec2 bottomCenter = ent->position;
        bottomCenter.x += (float)ent->GetWidth() / 2.f;
        bottomCenter.y += (float)ent->GetHeight();
        glm::vec2 playerCenter = playerSprite_->position;
        playerCenter.x += (float)playerSprite_->GetWidth() / 2.f;
        playerCenter.y += (float)playerSprite_->GetHeight() / 2.f;
        float dist = glm::distance(bottomCenter, playerCenter);
        if(dist > MAX_DISTANCE)
        {
            userInterface_->WriteLineToConsole("Out of range!", 1.f, 0.f, 0.f, 1.f);
            StopActionSound();
        }
        else 
        {
            // if already harvesting and clicked, then just ignore
            if(playerAction_ == PlayerAction::Harvesting)
                return;
            // in range so we can play the sound and set the harvesting flag for checking
            // during update intervals as well as show the harvesting cast bar
            maxCastTime_ = ent->GetClickTime();
            currentCastTime_ = 0.0f;
            playerAction_ = PlayerAction::Harvesting;
            auto& sm = engine::GameEngine::Get().GetSoundManager();
            StopActionSound();
            // configuration determins what to play here. for now use the generic action sound
            std::string ACTION_SOUND;
            configuration_.GetVar("ACTION_SOUND", ACTION_SOUND);
            actionSoundChannel_ = sm.PlaySound(ACTION_SOUND);
            userInterface_->ToggleCastBar(true);
        }
    }

    void IsleGame::StopActionSound()
    {
        if(actionSoundChannel_ != -1)
        {
            engine::GameEngine::Get().GetSoundManager().HaltSound(actionSoundChannel_);
            actionSoundChannel_ = -1;
        }
    }

    void IsleGame::ClearTarget()
    {   
        target_.SetTargetSprite(nullptr, Target::TARGET_TYPE::NEUTRAL, Target::SPRITE_TYPE::NONE);
        userInterface_->TargetUnitFrame_Toggle(false);
    }

    void IsleGame::RemoveEntityFromLoaded(Entity* ent)
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
        // clear target state if it is being targeted
        if(target_.GetTargetSprite() == (Sprite*)ent)
        {
            ClearTarget();
        }
        // finally delete the object.
        delete ent;
    }

    void IsleGame::CheckActionCast(float dtime)
    {
        if(playerAction_ == PlayerAction::None)
        {
            userInterface_->ToggleCastBar(false);
            return;
        }

        currentCastTime_ += dtime;
        userInterface_->SetCastBarValue(currentCastTime_ / maxCastTime_);
        // handle completion of action
        if(currentCastTime_ >= maxCastTime_)
        {
            // cast time is complete so act accordingly
            bool dinged = false; // level up flag
            StopActionSound();
            if(playerAction_ == PlayerAction::Harvesting)
            {
                // if the current target isn't of type Entity then something has gone horribly wrong because
                // target switching should have cancelled the harvest
                if(target_.GetTargetSpriteType() != Target::SPRITE_TYPE::ENTSPR)
                {
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                        "%s: Target is not an entity (this point should have never been reached!)", 
                        __FUNCTION__);
                    playerAction_ = PlayerAction::None;
                    return;
                }
                // now it is safe to cast targeted sprite to the Entity
                Entity* targetedEntity = (Entity*)target_.GetTargetSprite();
                if(targetedEntity->IsFarmable() && targetedEntity->IsReadyForPickup())
                {
                    // check farming
                    auto itemToAdd = targetedEntity->Farm();
                    harvesting_->SetFarmCommand((int)targetedEntity->position.x, 
                        (int)targetedEntity->position.y, 
                        FarmCommand((int)targetedEntity->position.x, (int)targetedEntity->position.y,
                            false, time(nullptr)));
                    inventory_.AddItemByName(itemToAdd.name, itemToAdd.num);
                    userInterface_->WriteLineToConsole(std::string("You harvested ") + std::to_string(itemToAdd.num) 
                        + " " + itemToAdd.name);
                    // add experience based on number of items farmed
                    dinged = playerSprite_->GetPlayerCombatUnit().AddExperience(itemToAdd.num);
                    userInterface_->WriteLineToConsole(std::string(" And gained ") + std::to_string(itemToAdd.num) 
                        + " exp", 1.0f, 0.f, 1.0f, 1.0f);
                    userInterface_->BuildInventory();
                    UpdatePlayerExperience(dinged);
                    ClearTarget(); // to prevent accidentally harvesting instead of farming item
                }
                else 
                {
                    // check harvest-clicking
                    targetedEntity->DecRemainingClicks();
                    harvesting_->SetHarvestCommand((int)targetedEntity->position.x, 
                        (int)targetedEntity->position.y, 
                        targetedEntity->GetMaxClicks() - targetedEntity->GetRemainingClicks());
                    // get the item(s) to add.
                    auto itemsToAdd = targetedEntity->OnInteract();
                    for(auto each : itemsToAdd)
                    {
                        if(each.name != "exp")
                        {
                            userInterface_->WriteLineToConsole(std::string("You receive ") + std::to_string(each.num) 
                                + " " + each.name);
                            inventory_.AddItemByName(each.name, each.num);
                            userInterface_->BuildInventory();
                        }
                        else 
                        {   // item exp is a special handled case, not added to inventory
                            userInterface_->WriteLineToConsole(std::string("You gain ") 
                                + std::to_string(each.num) + " experience.", 
                                1.0f, 0.0f, 1.0f, 1.0f);
                            dinged = playerSprite_->GetPlayerCombatUnit().AddExperience(each.num);
                            UpdatePlayerExperience(dinged);
                        }
                    }
                    if(targetedEntity->GetRemainingClicks() == 0)
                    {
                        auto items = targetedEntity->OnDestroy();
                        for(auto each : items)
                        {
                            if(each.name != "exp")
                            {
                                userInterface_->WriteLineToConsole(std::string("You received ") 
                                    + std::to_string(each.num) + " " + each.name);
                                inventory_.AddItemByName(each.name, each.num);
                                userInterface_->BuildInventory();
                            }
                            else 
                            {
                                userInterface_->WriteLineToConsole(std::string("You gain ") + std::to_string(each.num) 
                                    + " experience", 1.0f, 0.0f, 1.0f, 1.0f);
                                dinged = playerSprite_->GetPlayerCombatUnit().AddExperience(each.num);
                                UpdatePlayerExperience(dinged);
                            }
                        }
                        RemoveSpriteFromRenderList(targetedEntity);
                        RemoveEntityFromLoaded(targetedEntity);
                        ClearTarget();
                    }
                }
            }
            // TODO: else if to check crafting, etc.

            // in all cases the cast time completion indicates that the action is finished
            playerAction_ = PlayerAction::None;
            userInterface_->ToggleCastBar(false);
            StopActionSound();
        }
    }

    void IsleGame::UpdatePlayerExperience(bool dinged)
    {
        if(dinged) // TODO: play a sound when player gets a level increase
        {
            userInterface_->WriteLineToConsole(std::string("You are now level ") + std::to_string(
                playerSprite_->GetPlayerCombatUnit().GetAttributeSheet().GetLevel()) 
                + "!", 1.0f, 1.0f, 0.0f, 1.0f);
            // update UI to reflect new level
            userInterface_->PlayerUnitFrame_SetNameAndLevel(saveSlot_, 
                playerSprite_->GetPlayerCombatUnit().GetAttributeSheet().GetLevel());
        }
        int experience = playerSprite_->GetPlayerCombatUnit().GetCurrentExperience();
        int maxExperience = playerSprite_->GetPlayerCombatUnit().GetMaxExperience();
        float value = (float)experience / (float)maxExperience;
        userInterface_->SetExperienceBar(value);
        // print information for debugging purposes.
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
            "Experience: %d/%d (%f%%)", experience, maxExperience, value*100.f);
    }
}

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
#include "TileGame.hpp"

TileGame::TileGame() : configuration_("TileGame/gameconfig.lua")
{ 
    // process inventory configuration
    configuration_.AddItemEntries(inventory_);
}

TileGame::~TileGame()
{

}

bool TileGame::Initialize()
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
            // TODO: create a customizable keybind system using unordered_map and UI commands
            glm::vec3 vel = playerSprite_->GetVelocity();
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
                case SDLK_i:
                    showingInventory_ = !showingInventory_;
                    uiSystem_->ShowInventory(showingInventory_);
                    return;
                case SDLK_ESCAPE:
                    uiSystem_->ShowMMPopup(true);
                    return;
                default:
                    // without this the entire screen except UI goes black......
                    // TODO: fix weird velocity bug?
                    return;
            }
            if(vel.length() != 0)
            {
                // first check harvesting cast. TODO: check other casts.
                // because vel.length is 0, indicates movement so cancel cast.
                if(harvesting_)
                {
                    harvesting_ = false;
                    uiSystem_->ToggleCastBar(false);
                    uiSystem_->WriteLineToConsole("Harvest interrupted by player", 1.f, 0.f, 0.f, 0.9f);
                    if(harvestSoundChannel_ != -1)
                    {
                        engine::GameEngine::Get().GetSoundManager().HaltSound(harvestSoundChannel_);
                        harvestSoundChannel_ = -1;
                    }
                }
                const float baseSpeed = configuration_.GetBasePlayerSpeed();
                playerSprite_->StartAnimation();
                if(vel.y != 0)
                    vel.y /= glm::abs(vel.y); // set to 1 or -1
                if(vel.x != 0)
                    vel.x /= glm::abs(vel.x); // set to 1 or -1
                vel = glm::normalize(vel) * baseSpeed;
                // determine if player sprite is swimming and cut speed in half if so
            }
            playerSprite_->SetVelocity(vel);
            if(SpriteIsSwimming(playerSprite_))
            {
                playerSprite_->SetVelocity(playerSprite_->GetVelocity() / 2.0f);
            }
        }
        else 
        {
            // todo: stop movement based on key up
            playerSprite_->PauseAnimation();
            playerSprite_->SetVelocity({0.f,0.f,0.f});
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
                        auto pos = mob->GetPosition();
                        float w = (float)mob->GetWidth();
                        float h = (float)mob->GetHeight();
                        return (x > pos.x && x < pos.x + w && y > pos.y && y < pos.y + h);
                    }
                );
                if(mobFound != mobSprites_.end())
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
                    harvesting_ = false; // make sure harvest is interrupted upon aquiring new target
                    // Set the unit frame data.
                    //  not possible to use existing enums due to circular dependencies generating
                    //  arcane compile errors
                    uiSystem_->TargetUnitFrame_SetNameAndLevel((*mobFound)->GetCombatUnit().GetName(), 
                        (*mobFound)->GetCombatUnit().GetStatSheet().GetLevel());
                    std::string hostility = "friendly"; // default value
                    switch(tt)
                    {
                        case Target::TARGET_TYPE::HOSTILE: hostility = "hostile"; break;
                        case Target::TARGET_TYPE::NEUTRAL: hostility = "neutral"; break;
                    }
                    uiSystem_->TargetUnitFrame_SetHealth((*mobFound)->GetCombatUnit().GetCurrentHealth(), 
                        (*mobFound)->GetCombatUnit().GetMaxHealth(), hostility);
                    uiSystem_->TargetUnitFrame_Toggle(true);
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
                                x > ent->GetPosition().x && x < ent->GetPosition().x + (float)ent->GetWidth() &&
                                y > ent->GetPosition().y && y < ent->GetPosition().y + (float)ent->GetHeight()
                            );
                        }
                    );
                    // if we found one, set it as the target and begin interacting with it
                    if(found != loadedEntities_.end())
                    {
                        target_.SetTargetSprite(*found, Target::TARGET_TYPE::FRIENDLY, Target::SPRITE_TYPE::ENTSPR);
                        // TODO: print information about the entity to console
                        InteractWithEntity(*found);
                        return;
                    }
                    else // no target was found so cancel harvest and clear existing target
                    {
                        harvesting_ = false;
                        ClearTarget();
                    }
                }
            }
        }
    });

    return true;
}

void TileGame::Cleanup()
{
    EndGame();
}

void TileGame::Update(float dtime)
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
            uiSystem_->WriteLineToConsole("Autosaving...", 0.6f, 0.6f, 0.6f, 1.0f);
            SaveGame();
        }

        // update player movement
        playerSprite_->Update(dtime);
        
        // update health on UI
        uiSystem_->PlayerUnitFrame_SetHealth(playerSprite_->GetPlayerCombatUnit().GetCurrentHealth(), 
            playerSprite_->GetPlayerCombatUnit().GetMaxHealth());

        // tile collision detection.
        float left, top, right, bottom;
        playerSprite_->GetCollisionBox(left, top, right, bottom);

        // update entities.
        for(auto each : loadedEntities_)
        {
            each->Update(dtime);
            // discard previous farm commands once an entity becomes ready for pick up.
            if(each->IsFarmable() && each->IsReadyForPickup())
            {
                RemoveFarmCommand((int)each->GetPosition().x, (int)each->GetPosition().y);
            }
        }

        // check casting and update cast bar
        CheckHarvestCast(dtime);

        // check each corner of sprite for being in a solid tile
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
        // on collision just reset the sprite position. TODO: something a bit smoother
        if(collision) 
            playerSprite_->Update(-dtime);
        // test player against entity collision
        else if(EntityCollisionCheck(playerSprite_))
            playerSprite_->Update(-dtime);
        // update mobs
        for(auto it : mobSprites_)
        {
            it->Update(dtime);
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

        // run one pass of despawning at least one out of range mobsprite.
        const float MAX_DISTANCE = 2048.0f;
        auto eachMobIt = mobSprites_.begin();
        for (; eachMobIt != mobSprites_.end(); ++eachMobIt)
        {
            auto mobPos = (*eachMobIt)->GetPosition();
            auto playerPos = playerSprite_->GetPosition();
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
            RemoveSpriteFromRenderList(*eachMobIt);
            delete *eachMobIt;
            mobSprites_.erase(eachMobIt);
        }

        // todo: bound player to map area

        // calculate camera.
        float scrW = (float)engine::GameEngine::Get().GetWidth();
        float scrH = (float)engine::GameEngine::Get().GetHeight();
        glm::vec3 playerPos = playerSprite_->GetPosition();

        camera_.x = playerPos.x - scrW / 2.f;
        camera_.y = playerPos.y - scrH / 2.f;

        // TODO: bound camera to map area if map dimensions are bigger than screen
        /*if(camera_.x < 0.0f)
            camera_.x = 0.0f;
        if(camera_.y < 0.0f)
            camera_.y = 0.0f;*/
    }

    engine::ui::Root::Get()->Update(dtime);
}

void TileGame::Render(engine::GraphicsContext& gc)
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
    }
    engine::ui::Root::Get()->Render(gc);
}

void TileGame::StartGame()
{
    // Change the game state
    gameState_ = GAME_STATE::PLAYING;
    // Load map and set up entities
    tileMap_ = new TileMap("res/tilemaps/island.bin");
    loadedEntities_ = tileMap_->GenerateEntities();
    mobSpawners_ = tileMap_->GenerateSpawners();
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
    uiSystem_ = new UISystem(*this);
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
    swimFilter_ = new SwimFilter();
    // Setup render list
    SetupRenderList();
    // Update experience bar to reflect current xp levels
    UpdatePlayerExperience();
    // load sounds
    auto& sm = engine::GameEngine::Get().GetSoundManager();
    sm.LoadSound("res/sounds/chopping.wav");
    // start looping background music. TODO: fix SoundManager to get more song variety
    sm.PlayMusic("res/music/island1.ogg", -1);
    // set UI information
    uiSystem_->PlayerUnitFrame_SetNameAndLevel(saveSlot_, 
        playerSprite_->GetPlayerCombatUnit().GetStatSheet().GetLevel());
    uiSystem_->PlayerUnitFrame_SetHealth(playerSprite_->GetPlayerCombatUnit().GetCurrentHealth(), 
        playerSprite_->GetPlayerCombatUnit().GetMaxHealth());
    uiSystem_->BuildInventory();
    uiSystem_->SetFoodstuffBarData(inventory_.GetItemAmount("foodstuff"));
}

void TileGame::LoadGame()
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
        auto playerData = saveData_.LoadPlayerData();
        playerSprite_ = LoadPlayerLGSpr( playerData.boy ? configuration_.GetBoySurvivalistSprite() :
            configuration_.GetGirlSurvivalistSprite(), 32, 32, playerData.boy, 
            playerData.level, playerData.experience );
        // process locate command
        playerSprite_->SetPosition({(float)saveData_.GetLocationCommand().locationX,
            (float)saveData_.GetLocationCommand().locationY,0.f});
    }
    // process harvest commands
    harvestCommands_.clear();
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
            if(ent->GetMaxClicks() != -1)
            {
                ent->SetRemainingClicks(ent->GetMaxClicks() - cmd.count);
                if(ent->GetRemainingClicks() <= 0)
                {
                    RemoveSpriteFromRenderList(ent);
                    RemoveEntityFromLoaded(ent);
                }
            }
            SetHarvestCommand(cmd.targetX, cmd.targetY, cmd.count);
        }
    });
    // process farm commands
    farmCommands_.clear();
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
            SetFarmCommand(cmd.targetX, cmd.targetY, FarmCommand(cmd.targetX, 
                cmd.targetY, cmd.readyToFarm, cmd.farmedTime));
        }
    });
    // process inventory.
    inventory_.ClearItems();
    saveData_.LoadInventoryData(inventory_);

    // log the timestamp of the last save time
    time_t currentTime = time(nullptr);
    time_t timeStamp = saveData_.GetTimeStamp();
    time_t secondsElapsed = currentTime - timeStamp;
    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
        "%ld seconds since last file save", (long)secondsElapsed);

    // discard save data
    saveData_.ClearData();
}

void TileGame::SaveGame()
{
    auto pos = playerSprite_->GetPosition();
    saveData_.SetLocationCommand(LocationCommand((int)pos.x, (int)pos.y));
    auto harvestCmds = GetHarvestCommands();
    for(auto each : harvestCmds)
    {
        saveData_.AddHarvestCommand(each);
    }
    auto farmCmds = GetFarmCommands();
    for(auto each : farmCmds)
    {
        saveData_.AddFarmCommand(each);
    }
    saveData_.SaveInventoryData(inventory_);
    saveData_.SavePlayerData({
        // experience, level, gender
        playerSprite_->GetPlayerCombatUnit().GetCurrentExperience(),
        playerSprite_->GetPlayerCombatUnit().GetStatSheet().GetLevel(),
        playerSprite_->IsBoy()
    });
    saveData_.WriteToFile(saveSlot_);
}

void TileGame::NewGame(bool boy)
{
    int ix, iy;
    configuration_.GetTileSpawnPoint(ix, iy);
    playerSprite_ = LoadPlayerLGSpr(boy ? configuration_.GetBoySurvivalistSprite() :
        configuration_.GetGirlSurvivalistSprite(), 32, 32, boy, 1, 0);
    playerSprite_->SetPosition({
        (float)ix * (float)tileMap_->GetTileSet()->GetTileWidth(),
        (float)iy * (float)tileMap_->GetTileSet()->GetTileHeight(),
        0.f
    });
    // clear harvest and farm data
    harvestCommands_.clear();
    farmCommands_.clear();
}

void TileGame::EndGame()
{
    auto &sm = engine::GameEngine::Get().GetSoundManager();
    if(gameState_ != GAME_STATE::SPLASH)
    {
        delete uiSystem_;
        uiSystem_ = nullptr;
        SaveGame();
        UnloadPlayerLGSpr(playerSprite_, playerSprite_->IsBoy() ? 
            configuration_.GetBoySurvivalistSprite() : configuration_.GetGirlSurvivalistSprite());
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

void TileGame::ReturnToMainMenu()
{
    splashScreen_ = new SplashScreen();
    splashScreen_->Initialize();
    gameState_ = GAME_STATE::SPLASH;
}

PlayerSprite* TileGame::LoadPlayerLGSpr(const std::string& name, int w, int h, bool boy, int level, int exp)
{
    auto& tm = engine::GameEngine::Get().GetTextureManager();
    const std::string path = std::string("res/textures/sprites/last-guardian-sprites/") + name;
    ogl::Texture* texture = tm.GetTexture(path + "_fr1.gif");
    PlayerSprite* sprite = new PlayerSprite(texture, w, h, level, exp, boy, configuration_);
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

void TileGame::UnloadPlayerLGSpr(PlayerSprite*& sprite, const std::string& name)
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

bool TileGame::SpriteIsSwimming(Sprite* sprite)
{
    // calculate the bottom center of sprite in world coordinates.
    auto pos = sprite->GetPosition();
    pos.x += (float)sprite->GetWidth() / 2.f;
    pos.y += (float)sprite->GetHeight();

    // determine tile x and y index.
    int tileX = (int)pos.x / tileMap_->GetTileSet()->GetTileWidth();
    int tileY = (int)pos.y / tileMap_->GetTileSet()->GetTileHeight();

    bool isSwimming = tileMap_->TileIsLiquid(tileX, tileY, false);
    isSwimming = isSwimming || tileMap_->TileIsLiquid(tileX, tileY, true);
    return isSwimming;
}

void TileGame::CleanupLoadedEntities()
{
    for(auto it=loadedEntities_.begin(); it != loadedEntities_.end(); ++it)
    {
        delete (*it);
    }
    loadedEntities_.clear();
}

void TileGame::CleanupMobSpawners()
{
    for(auto it=mobSpawners_.begin(); it != mobSpawners_.end(); ++it)
    {
        delete *it;
    }
    mobSpawners_.clear();
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

Entity* TileGame::FindEntityByLocation(int x, int y)
{
    for(auto ent : loadedEntities_)
    {
        auto pos = ent->GetPosition();
        if((int)pos.x == x && (int)pos.y == y)
            return ent;
    }
    return nullptr;
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

void TileGame::InteractWithEntity(Entity* ent)
{
    if(ent->IsFarmable() && !ent->IsReadyForPickup() 
      && ent->GetMaxClicks() == -1)
    {
        // the target is not interactive except when farmable or clickable
        StopHarvestSound();
        return; 
    }

    // check to see if we are in MAX_DISTANCE units of bottom center of targeted entity.
    const float MAX_DISTANCE = 48.f;
    glm::vec3 bottomCenter = ent->GetPosition();
    bottomCenter.x += (float)ent->GetWidth() / 2.f;
    bottomCenter.y += (float)ent->GetHeight();
    glm::vec3 playerCenter = playerSprite_->GetPosition();
    playerCenter.x += (float)playerSprite_->GetWidth() / 2.f;
    playerCenter.y += (float)playerSprite_->GetHeight() / 2.f;
    float dist = glm::distance(bottomCenter, playerCenter);
    if(dist > MAX_DISTANCE)
    {
        uiSystem_->WriteLineToConsole("Out of range!", 1.f, 0.f, 0.f, 1.f);
        StopHarvestSound();
    }
    else 
    {
        // if already harvesting and clicked, then just ignore
        if(harvesting_)
            return;
        // in range so we can play the sound and set the harvesting flag for checking
        // during update intervals as well as show the harvesting cast bar
        maxCastTime_ = ent->GetClickTime();
        currentCastTime_ = 0.0f;
        harvesting_ = true;
        auto& sm = engine::GameEngine::Get().GetSoundManager();
        StopHarvestSound();
        harvestSoundChannel_ = sm.PlaySound("res/sounds/chopping.wav");
        uiSystem_->ToggleCastBar(true);
    }
}

void TileGame::StopHarvestSound()
{
    if(harvestSoundChannel_ != -1)
    {
        engine::GameEngine::Get().GetSoundManager().HaltSound(harvestSoundChannel_);
        harvestSoundChannel_ = -1;
    }
}

void TileGame::ClearTarget()
{   
    target_.SetTargetSprite(nullptr, Target::TARGET_TYPE::NEUTRAL, Target::SPRITE_TYPE::NONE);
    uiSystem_->TargetUnitFrame_Toggle(false);
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
    // clear target state if it is being targeted
    if(target_.IsTargetEntity(ent))
    {
        ClearTarget();
    }
    // finally delete the object.
    delete ent;
}

void TileGame::CheckHarvestCast(float dtime)
{
    if(harvesting_)
    {
        currentCastTime_ += dtime;
        uiSystem_->SetCastBarValue(currentCastTime_ / maxCastTime_);
        // handle completion of harvest
        if(currentCastTime_ >= maxCastTime_)
        {
            bool dinged = false; // level up flag
            StopHarvestSound();
            // if the current target isn't of type Entity then something has gone horribly wrong because
            // target switching should have cancelled the harvest
            if(target_.GetTargetSpriteType() != Target::SPRITE_TYPE::ENTSPR)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                    "%s: Target is not an entity (this point should have never been reached!)", __FUNCTION__);
                harvesting_ = false;
                return;
            }
            // now it is safe to cast targeted sprite to the Entity
            Entity* targetedEntity = (Entity*)target_.GetTargetSprite();
            if(targetedEntity->IsFarmable() && targetedEntity->IsReadyForPickup())
            {
                // check farming
                auto itemToAdd = targetedEntity->Farm();
                SetFarmCommand((int)targetedEntity->GetPosition().x, (int)targetedEntity->GetPosition().y, 
                    FarmCommand((int)targetedEntity->GetPosition().x, (int)targetedEntity->GetPosition().y,
                        false, time(nullptr)));
                inventory_.AddItemByName(itemToAdd.name, itemToAdd.num);
                uiSystem_->WriteLineToConsole(std::string("You harvested ") + std::to_string(itemToAdd.num) 
                    + " " + itemToAdd.name);
                // add experience based on number of items farmed
                dinged = playerSprite_->GetPlayerCombatUnit().AddExperience(itemToAdd.num);
                uiSystem_->WriteLineToConsole(std::string(" And gained ") + std::to_string(itemToAdd.num) + " exp",
                    1.0f, 0.f, 1.0f, 1.0f);
                uiSystem_->BuildInventory();
                UpdatePlayerExperience();
                ClearTarget(); // to prevent accidentally harvesting instead of farming item
            }
            else 
            {
                // check harvest-clicking
                targetedEntity->DecRemainingClicks();
                SetHarvestCommand((int)targetedEntity->GetPosition().x, (int)targetedEntity->GetPosition().y, 
                    targetedEntity->GetMaxClicks() - targetedEntity->GetRemainingClicks());
                // get the item(s) to add.
                auto itemsToAdd = targetedEntity->OnInteract();
                for(auto each : itemsToAdd)
                {
                    if(each.name != "exp")
                    {
                        uiSystem_->WriteLineToConsole(std::string("You receive ") + std::to_string(each.num) 
                            + " " + each.name);
                        inventory_.AddItemByName(each.name, each.num);
                        uiSystem_->BuildInventory();
                    }
                    else 
                    {   // item exp is a special handled case, not added to inventory
                        uiSystem_->WriteLineToConsole(std::string("You gain ") 
                            + std::to_string(each.num) + " experience.", 
                            1.0f, 0.0f, 1.0f, 1.0f);
                        dinged = playerSprite_->GetPlayerCombatUnit().AddExperience(each.num);
                        UpdatePlayerExperience();
                    }
                }
                if(targetedEntity->GetRemainingClicks() == 0)
                {
                    auto items = targetedEntity->OnDestroy();
                    for(auto each : items)
                    {
                        if(each.name != "exp")
                        {
                            uiSystem_->WriteLineToConsole(std::string("You received ") 
                                + std::to_string(each.num) + " " + each.name);
                            inventory_.AddItemByName(each.name, each.num);
                            uiSystem_->BuildInventory();
                        }
                        else 
                        {
                            uiSystem_->WriteLineToConsole(std::string("You gain ") + std::to_string(each.num) 
                                + " experience", 1.0f, 0.0f, 1.0f, 1.0f);
                            dinged = playerSprite_->GetPlayerCombatUnit().AddExperience(each.num);
                            UpdatePlayerExperience();
                        }
                    }
                    RemoveSpriteFromRenderList(targetedEntity);
                    RemoveEntityFromLoaded(targetedEntity);
                    ClearTarget();
                }
            }
            harvesting_ = false;
            uiSystem_->ToggleCastBar(false);
            StopHarvestSound();
            if(dinged)
            {
                uiSystem_->WriteLineToConsole(std::string("You are now level ") + std::to_string(
                    playerSprite_->GetPlayerCombatUnit().GetStatSheet().GetLevel()) 
                    + "!", 1.0f, 1.0f, 0.0f, 1.0f);
                // update UI to reflect new level
                uiSystem_->PlayerUnitFrame_SetNameAndLevel(saveSlot_, 
                    playerSprite_->GetPlayerCombatUnit().GetStatSheet().GetLevel());
            }
        }
    }
}

void TileGame::UpdatePlayerExperience()
{
    int experience = playerSprite_->GetPlayerCombatUnit().GetCurrentExperience();
    int maxExperience = playerSprite_->GetPlayerCombatUnit().GetMaxExperience();
    float value = (float)experience / (float)maxExperience;
    uiSystem_->SetExperienceBar(value);
    // print information for debugging purposes.
    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
        "Experience: %d/%d (%f%%)", experience, maxExperience, value*100.f);
}

void TileGame::SetHarvestCommand(int x, int y, int clicks)
{
    auto found = harvestCommands_.find({x,y});
    if(found != harvestCommands_.end())
    {
        found->second = clicks;
    }
    else 
    {
        harvestCommands_[{x,y}] = clicks;
    }
}

void TileGame::SetFarmCommand(int x, int y, const FarmCommand& fc)
{
    auto found = farmCommands_.find({x,y});
    if(found != farmCommands_.end())
    {
        found->second = fc;
    }
    else
    {
        farmCommands_[{x,y}] = fc;
    }
}

void TileGame::RemoveFarmCommand(int x, int y)
{
    auto found = farmCommands_.find({x,y});
    if(found != farmCommands_.end())
        farmCommands_.erase(found);
}
    
std::vector<HarvestCommand> TileGame::GetHarvestCommands()
{
    std::vector<HarvestCommand> hc;
    for(auto it = harvestCommands_.begin(); it != harvestCommands_.end(); ++it)
    {
        hc.push_back(HarvestCommand(it->first.x, it->first.y, it->second));
    }
    return hc;
}

std::vector<FarmCommand> TileGame::GetFarmCommands()
{
    std::vector<FarmCommand> commands;
    for(auto it = farmCommands_.begin(); it != farmCommands_.end(); ++it)
    {
        commands.push_back(it->second);
    }
    return commands;
}

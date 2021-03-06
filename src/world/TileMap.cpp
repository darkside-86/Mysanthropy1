// TileMap.cpp
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

#include <algorithm>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.hpp"
#include "TileMap.hpp"

namespace world
{

    TileMap::TileMap(int tileWidth, int tileHeight, const std::string& tilesetPath, int width, int height)
        : width_(width), height_(height)
    {
        tileSet_ = new TileSet(tilesetPath, tileWidth, tileHeight);
        layer0_ = new TILE [width_ * height_];
        layer1_ = new TILE [width_ * height_];
        collisionLayer_ = new unsigned char [width_ * height_];
        memset(collisionLayer_, 0, width_ * height_);
        SetupRender();
        SetupScripting();
    }

    TileMap::TileMap(const std::string& path)
    {
        SetupScripting();
        LoadFromFile(path);
    }

    TileMap::~TileMap()
    {
        delete [] layer0_;
        delete [] layer1_;
        delete [] collisionLayer_;
        delete tileSet_;
        delete vao_;
        delete vao1_;
        delete collisionVao_;
        delete redTexture_;
        CleanupEntities();
        CleanupSpawners();
        lua_close(scripting_);
    }

    TILE TileMap::GetTile(int ix, int iy, bool layer1)
    {
        int index = iy*width_ + ix;
        if(index < 0 || ix >= width_ || iy >= height_)
            return {0,0};
        return !layer1? layer0_[index] : layer1_[index];
    }

    void TileMap::SetTile(int ix, int iy, const TILE& tile, bool layer1)
    {
        int index = iy * width_ + ix;
        if(index < 0 || ix >= width_ || iy >= height_)
            return;
        if(!layer1)
            layer0_[index] = tile;
        else
            layer1_[index] = tile;
        SetupRender();
    }

    bool TileMap::TileIsLiquid(int x, int y, bool layer1)
    {
        TILE t = GetTile(x, y, layer1);
        // check to see if t's ID value is in list of liquids
        for(auto each : liquids_)
        {
            if(each.ix == t.ix && each.iy == t.iy)
                return true;
        }
        return false;
    }

    bool TileMap::TileIsBuildable(int x, int y)
    {
        // first check tile at layer1 is "empty"
        TILE t = GetTile(x, y, 1);
        // if not, can't build here
        if(!(t.ix == empty_.ix && t.iy == empty_.iy))
            return false;
        // check tile at layer0 is "buildable"
        t = GetTile(x, y, 0);
        // determine if it is in "buildable"
        for(auto each : buildable_)
        {
            if(each.ix == t.ix && each.iy == t.iy)
                return true;
        }
        // reached end of list so it's not in buildable 
        return false;
    }

    void TileMap::SaveToFile(const std::string& path)
    {
        std::ofstream out;
        out.open(path, std::ios::binary);
        if(!out.is_open())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                    "%s: Unable to open `%s' for writing", __FUNCTION__, path.c_str());
            return;
        }

        const unsigned char MAJOR_VERSION = 0;
        const unsigned char MINOR_VERSION = 6;
        const std::string TILESET_PATH = tileSet_->GetPathToTexture();
        out.write((char*)&MAJOR_VERSION, sizeof(unsigned char));
        out.write((char*)&MINOR_VERSION, sizeof(unsigned char));
        auto size = TILESET_PATH.size();
        out.write((char*)&size, sizeof(size));
        out.write(TILESET_PATH.c_str(), TILESET_PATH.size());
        int tileWidth=tileSet_->GetTileWidth(), tileHeight=tileSet_->GetTileHeight();
        out.write((char*)&tileWidth, sizeof(tileWidth));
        out.write((char*)&tileHeight, sizeof(tileHeight));
        out.write((char*)&width_, sizeof(width_));
        out.write((char*)&height_, sizeof(height_));
        // TODO: write each array all at once
        for(int i=0; i < width_*height_; ++i)
        {
            out.write((char*)&layer0_[i], sizeof(TILE));
        }
        for(int i=0; i < width_ * height_; ++i)
        {
            out.write((char*)&layer1_[i], sizeof(TILE));
        }
        for(int i=0; i < width_ * height_; ++i)
        {
            out.write((char*)&collisionLayer_[i], sizeof(unsigned char));
        }
        // write the script path
        size = scriptPath_.size();
        out.write((char*)&size, sizeof(size));
        out.write(scriptPath_.c_str(), size);
        // write the entities.
        size = mapEntities_.size();
        out.write((char*)&size, sizeof(size));
        out.write((char*)&mapEntities_[0], sizeof(ENTITY_LOCATION) * size);
        // write the spawners
        size = spawnerLocations_.size();
        out.write((char*)&size, sizeof(size));
        out.write((char*)&spawnerLocations_[0], sizeof(MOBSPAWNER_LOCATION) * size);
        out.close();
    }

    void TileMap::LoadFromFile(const std::string& path)
    {
        // cleanup old data
        delete tileSet_;
        delete [] layer0_;
        delete [] layer1_;
        delete [] collisionLayer_;
        tileSet_ = nullptr;
        layer0_ = nullptr;
        layer1_ = nullptr;
        collisionLayer_ = nullptr;
        width_ = 0;
        height_ = 0;
        scriptPath_ = "";
        CleanupEntities();
        CleanupSpawners();
        
        std::ifstream in;
        in.open(path, std::ios::binary);
        if(!in.is_open())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "%s: Unable to open `%s'", __FUNCTION__, path.c_str());
            return;
        }
        // first read major, minor version
        unsigned char majorV, minorV;
        in.read((char*)&majorV, sizeof(majorV));
        in.read((char*)&minorV, sizeof(minorV));
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
                "%s: TileMap version %d.%d", __FUNCTION__, majorV, minorV);
        // read path to tileset
        size_t size;
        in.read((char*)&size, sizeof(size));
        char* szPathToTileset = new char[size+1];
        szPathToTileset[size] = 0;
        in.read(szPathToTileset, size);
        std::string pathToTileset = szPathToTileset;
        delete [] szPathToTileset;
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
                "%s: Path to tileset: `%s'", __FUNCTION__, pathToTileset.c_str());
        // read tile width and height for tileset
        int tileWidth, tileHeight;
        in.read((char*)&tileWidth, sizeof(tileWidth));
        in.read((char*)&tileHeight, sizeof(tileHeight));
        // create tileset with info
        tileSet_ = new TileSet(pathToTileset, tileWidth, tileHeight);
        // read width and height of map
        in.read((char*)&width_, sizeof(width_));
        in.read((char*)&height_, sizeof(height_));
        layer0_ = new TILE[width_*height_];
        layer1_ = new TILE[width_*height_];
        collisionLayer_ = new unsigned char[width_ * height_];
        // TODO: read each array all at once
        for(int i=0; i < width_*height_; ++i)
        {
            TILE tile;
            in.read((char*)&tile, sizeof(tile));
            layer0_[i] = tile;
        }
        for(int i=0; i < width_*height_; ++i)
        {
            TILE tile;
            in.read((char*)&tile, sizeof(tile));
            layer1_[i] = tile;
        }
        for(int i=0; i < width_ * height_; ++i)
        {
            unsigned char c;
            in.read((char*)&c, sizeof(c));
            collisionLayer_[i] = c;
        }
        // read script path
        in.read((char*)&size, sizeof(size));
        char* szScriptPath = new char[size+1];
        szScriptPath[size] = 0;
        in.read(szScriptPath, size);
        scriptPath_ = szScriptPath;
        delete [] szScriptPath;
        // read map entity locations
        in.read((char*)&size, sizeof(size));
        mapEntities_.resize(size);
        in.read((char*)&mapEntities_[0], size * sizeof(ENTITY_LOCATION));
        // read spawner locations
        if(minorV >= 6)
        {
            in.read((char*)&size, sizeof(size));
            spawnerLocations_.resize(size);
            in.read((char*)&spawnerLocations_[0], size * sizeof(MOBSPAWNER_LOCATION));
        }
        in.close();

        // run the script at scriptPath_
        int ok = luaL_dofile(scripting_, scriptPath_.c_str());
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                    "%s: Error loading map - %s", __FUNCTION__, lua_tostring(scripting_, -1));
            lua_pop(scripting_, 1);
        }

        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                "%s: Finished loading", __FUNCTION__);
        SetupRender();
    }

    void TileMap::SetupRender()
    {
        ogl::Vertex* vertices;
        ogl::Vertex* l1Verts;
        // 6 vertices per tile
        int numVertices = 6 * width_ * height_;
        vertices = new ogl::Vertex[numVertices];
        l1Verts = new ogl::Vertex[numVertices];
        int vi = 0;
        int ti = 0;
        float w = (float)tileSet_->GetTileWidth();
        float h = (float)tileSet_->GetTileHeight();
        int ntx, nty;
        tileSet_->GetNumTiles(ntx, nty);
        float tw = 1.f / (float)ntx;
        float th = 1.f / (float)nty;
        float x = 0.0f;
        float y = 0.0f;
        while(vi < numVertices)
        {
            float s0 = (float)layer0_[ti].ix / (float)ntx;
            float s1 = s0 + tw;
            float t0 = (float)layer0_[ti].iy / (float)nty;
            float t1 = t0 + th;
            // layer 0
            vertices[0+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
            vertices[1+vi] = {{x,y+h,0.f}, {255,255,255,255}, {s0, t1}, {0.f, 0.f, 1.f}}; 
            vertices[2+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
            vertices[3+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
            vertices[4+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
            vertices[5+vi] = {{x+w,y,0.f}, {255,255,255,255}, {s1, t0}, {0.f, 0.f, 1.f}}; 
            // layer 1
            s0 = (float)layer1_[ti].ix / (float)ntx;
            s1 = s0 + tw;
            t0 = (float)layer1_[ti].iy / (float)nty;
            t1 = t0 + th;
            l1Verts[0+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
            l1Verts[1+vi] = {{x,y+h,0.f}, {255,255,255,255}, {s0, t1}, {0.f, 0.f, 1.f}}; 
            l1Verts[2+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
            l1Verts[3+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
            l1Verts[4+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
            l1Verts[5+vi] = {{x+w,y,0.f}, {255,255,255,255}, {s1, t0}, {0.f, 0.f, 1.f}}; 
            vi += 6;
            ti += 1;
            x += w;
            if(ti > 0 && ti % width_ == 0)
            {
                y += h;
                x = 0.f;
            }
        }
        vbo_.SetData(sizeof(ogl::Vertex)*numVertices, vertices, GL_STATIC_DRAW);
        vbo1_.SetData(sizeof(ogl::Vertex)*numVertices, l1Verts, GL_STATIC_DRAW);
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        delete vao_;
        delete vao1_;
        vao_ = new ogl::VertexArray();
        vao_->AddBuffer(vbo_, vbl);
        vao1_ = new ogl::VertexArray();
        vao1_->AddBuffer(vbo1_, vbl);
        delete [] vertices;
        delete [] l1Verts;

        // set up "X" indicator tile (only needs to be done one time)
        if(collisionVao_ == nullptr)
        {
            redTexture_ = new ogl::Texture("res/textures/red.png");
            ogl::Vertex xverts[] = { // line coords
                {{0.f,0.f,0.f},{255,255,255,255},{0.f,0.f},{0.f,0.f,1.f}},
                {{  w,  h,0.f},{255,255,255,255},{1.f,1.f},{0.f,0.f,1.f}},
                {{0.f,  h,0.f},{255,255,255,255},{0.f,1.f},{0.f,0.f,1.f}},
                {{w,  0.f,0.f},{255,255,255,255},{1.f,0.f},{0.f,0.f,1.f}},
            };
            collisionVbo_.SetData(sizeof(ogl::Vertex)*4, xverts, GL_STATIC_DRAW);
            collisionVao_ = new ogl::VertexArray();
            ogl::VertexBufferLayout vbl;
            ogl::Vertex::PushLayout(vbl);
            collisionVao_->AddBuffer(collisionVbo_, vbl);
        }
    }

    void TileMap::Render(float x, float y, ogl::Program& program, float scaleX, float scaleY)
    {
        if(vao_ == nullptr)
            return;
        program.Use();
        vao_->Bind();
        tileSet_->GetTexture()->Bind();
        glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));    
        model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.f));
        program.SetUniform<glm::mat4>("u_model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6 * width_ * height_);
        vao1_->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6 * width_ * height_);
        // RenderCollisionData(x,y,program,scaleX,scaleY);
    }

    void TileMap::RenderCollisionData(float x, float y, ogl::Program& program, float scaleX, float scaleY)
    {
        int ix, iy;
        float w = (float)tileSet_->GetTileWidth() * scaleX;
        float h = (float)tileSet_->GetTileHeight() * scaleY;
        glm::vec3 location = {x, y, 0.f};
        collisionVao_->Bind();
        redTexture_->Bind();
        program.Use();
        glm::mat4 model(1.f);
        for(iy = 0; iy < height_; ++iy)
        {
            for(ix = 0; ix < width_; ++ix)
            {
                if(GetCollisionData(ix, iy) != 0)
                {
                    model = glm::translate(glm::mat4(1.f), location);
                    model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.f));
                    program.SetUniform("u_model", model);
                    glDrawArrays(GL_LINES, 0, 4);
                }
                location.x += w;
            }
            location.x = (float)x;
            location.y += h;
        }
    }

    std::vector<game::Entity*> TileMap::GenerateEntities()
    {
        std::vector<game::Entity*> entities;
        for(auto it=mapEntities_.begin(); it != mapEntities_.end(); ++it)
        {
            game::Entity* e = new game::Entity(GetEntityType(it->entityID));
            e->position = {(float)(it->x), (float)(it->y)};
            entities.push_back(e);
        }
        // CleanupEntities(); keep these entries so that map can be modified
        return entities;
    }

    std::vector<game::MobSpawner*> TileMap::GenerateSpawners()
    {
        std::vector<game::MobSpawner*> spawners;
        for(auto it=spawnerLocations_.begin(); it != spawnerLocations_.end(); ++it)
        {
            // TODO: fix spawner location struct to include information provided by const literals here
            game::MobSpawner* ms = new game::MobSpawner(mobTypes_[it->spawnerID], it->freq, 
                {(float)it->x, (float)it->y}, it->chance);
            spawners.push_back(ms);
        }
        return spawners;
    }

    void TileMap::FillWithTile(const TILE& tile, bool layer1)
    {
        for(int i=0; i < width_*height_; ++i)
        {
            if(!layer1)
                layer0_[i] = tile;
            else
                layer1_[i] = tile;
        }
        SetupRender();
    }

    unsigned char TileMap::GetCollisionData(int ix, int iy)
    {
        int index = iy * width_ + ix;
        if(index < 0 || ix >= width_ || iy >= height_)
            return 0;
        else
            return collisionLayer_[index];
    }

    void TileMap::SetCollisionData(int ix, int iy, unsigned char value)
    {
        int index = iy * width_ + ix;
        if(index >= 0 && ix < width_ && iy < height_)
            collisionLayer_[index] = value;
    }

    game::EntityType TileMap::GetEntityType(int index)
    {
        if(index >= 0 && index < entityTypes_.size())
        {
            return entityTypes_[index];
        }
        else 
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                    "%s: index out of range (%d)", __FUNCTION__, index);
            game::EntityType blank;
            blank.name = ""; // indicates invalid
            return blank;
        }
    }

    game::MobType TileMap::GetMobType(int index)
    {
        if(index >= 0 && index < mobTypes_.size())
        {
            return mobTypes_[index];
        }
        else 
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                "%s: index out of range (%d)", __FUNCTION__, index);
            game::MobType blank;
            blank.name = "";
            return blank;
        }
    }

    void TileMap::AddEntityLocation(unsigned short entityID, unsigned int x, unsigned int y)
    {
        ENTITY_LOCATION location = { entityID, x, y };
        mapEntities_.push_back(location);
    }

    bool TileMap::RemoveEntityLocation(unsigned short entityID, unsigned int x, unsigned int y)
    {
        auto it = std::find_if(mapEntities_.begin(), mapEntities_.end(), 
            [this, entityID, x, y](const ENTITY_LOCATION& loc){
                game::EntityType t = GetEntityType(entityID);
                return loc.entityID == entityID && 
                    x >= loc.x && x <= loc.x + t.width &&
                    y >= loc.y && y <= loc.y + t.height;
            }
        );
        if(it != mapEntities_.end())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                    "%s: Located for removal", __FUNCTION__);
            mapEntities_.erase(it);
            return true;
        }
        else
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                    "%s: Wasn't found", __FUNCTION__);
            return false;
        }
        
    }

    unsigned short TileMap::GetEntityIDAtLocation(unsigned int x, unsigned int y)
    {
        auto it = std::find_if(mapEntities_.begin(), mapEntities_.end(), 
            [this, x, y](const ENTITY_LOCATION& loc){
                game::EntityType t = GetEntityType(loc.entityID);
                return x >= loc.x  && x <= loc.x + t.width &&
                    y >= loc.y && y <= loc.y + t.height;
            }
        );
        if(it != mapEntities_.end())
        {
            return it->entityID;
        }
        else 
        {
            return ENTITY_LOCATION::INVALID_ENTITY_ID;
        }
    }

    void TileMap::AddSpawnerLocation(unsigned short spawnerID, unsigned int x, unsigned int y, float freq, float chance)
    {
        MOBSPAWNER_LOCATION loc = { spawnerID, x, y, freq, chance };
        spawnerLocations_.push_back(loc);
    }

    void TileMap::RemoveSpawnerLocation(unsigned short spawnerID, unsigned int x, unsigned int y)
    {
        auto it = std::find_if(spawnerLocations_.begin(), spawnerLocations_.end(), 
            [this, spawnerID, x, y](const MOBSPAWNER_LOCATION& loc){
                return loc.spawnerID == spawnerID && 
                    x >= loc.x && x <= loc.x + 16 &&
                    y >= loc.y && y <= loc.y + 16;
            }
        );
        if(it != spawnerLocations_.end())
        {
            spawnerLocations_.erase(it);
        }
    }

    unsigned short TileMap::GetMobTypeIDAtLocation(unsigned int x, unsigned int y, float& freq, float& chance)
    {
        auto it = std::find_if(spawnerLocations_.begin(), spawnerLocations_.end(), 
            [this, x, y](const MOBSPAWNER_LOCATION& loc){
                return x >= loc.x && x <= loc.x + 16 && 
                    y >= loc.y && y <= loc.y + 16;
            }
        );
        if(it != spawnerLocations_.end())
        {
            freq = it->freq;
            chance = it->chance;
            return it->spawnerID;
        }
        return MOBSPAWNER_LOCATION::INVALID_MOBTYPE_ID;
    }

    void TileMap::SetupScripting()
    {
        scripting_ = luaL_newstate();
        luaL_openlibs(scripting_);
        // store "this" in lua registry since each TileMap has its own lua state
        lua_pushstring(scripting_, "TileMap");
        lua_pushlightuserdata(scripting_, this);
        lua_settable(scripting_, LUA_REGISTRYINDEX);

        lua_pushcfunction(scripting_, TileMap::lua_Liquids);
        lua_setglobal(scripting_, "LIQUIDS");
        lua_pushcfunction(scripting_, TileMap::lua_Buildable);
        lua_setglobal(scripting_, "BUILDABLE");
        lua_pushcfunction(scripting_, TileMap::lua_Empty);
        lua_setglobal(scripting_, "EMPTY");
        lua_pushcfunction(scripting_, TileMap::lua_Swimming);
        lua_setglobal(scripting_, "SWIMMING");
        lua_pushcfunction(scripting_, TileMap::lua_BeginEntity);
        lua_setglobal(scripting_, "BEGIN_ENTITY");
        lua_pushcfunction(scripting_, TileMap::lua_UseTexture);
        lua_setglobal(scripting_, "USE_TEXTURE");
        lua_pushcfunction(scripting_, TileMap::lua_EndEntity);
        lua_setglobal(scripting_, "END_ENTITY");
        lua_pushcfunction(scripting_, TileMap::lua_Width);
        lua_setglobal(scripting_, "WIDTH");
        lua_pushcfunction(scripting_, TileMap::lua_Height);
        lua_setglobal(scripting_, "HEIGHT");
        lua_pushcfunction(scripting_, TileMap::lua_CollisionBox);
        lua_setglobal(scripting_, "COLLISION_BOX");
        lua_pushcfunction(scripting_, TileMap::lua_MaxClicks);
        lua_setglobal(scripting_, "MAX_CLICKS");
        lua_pushcfunction(scripting_, TileMap::lua_ClickTime);
        lua_setglobal(scripting_, "CLICK_TIME");
        lua_pushcfunction(scripting_, TileMap::lua_OnInteract);
        lua_setglobal(scripting_, "ON_INTERACT");
        lua_pushcfunction(scripting_, TileMap::lua_Farmable);
        lua_setglobal(scripting_, "FARMABLE");
        lua_pushcfunction(scripting_, TileMap::lua_OnDestroy);
        lua_setglobal(scripting_, "ON_DESTROY");
        lua_pushcfunction(scripting_, TileMap::lua_BeginMobType);
        lua_setglobal(scripting_, "BEGIN_MOB_TYPE");
        lua_pushcfunction(scripting_, TileMap::lua_MobClass);
        lua_setglobal(scripting_, "MOB_CLASS");
        lua_pushcfunction(scripting_, TileMap::lua_LevelRange);
        lua_setglobal(scripting_, "LEVEL_RANGE");
        lua_pushcfunction(scripting_, TileMap::lua_DefaultAnimation);
        lua_setglobal(scripting_, "DEFAULT_ANIMATION");
        lua_pushcfunction(scripting_, TileMap::lua_FrAnimTextureList);
        lua_setglobal(scripting_, "FR_ANIM_TEXTURE_LIST");
        lua_pushcfunction(scripting_, TileMap::lua_BkAnimTextureList);
        lua_setglobal(scripting_, "BK_ANIM_TEXTURE_LIST");
        lua_pushcfunction(scripting_, TileMap::lua_LfAnimTextureList);
        lua_setglobal(scripting_, "LF_ANIM_TEXTURE_LIST");
        lua_pushcfunction(scripting_, TileMap::lua_RtAnimTextureList);
        lua_setglobal(scripting_, "RT_ANIM_TEXTURE_LIST");
        lua_pushcfunction(scripting_, TileMap::lua_AnimSpeed);
        lua_setglobal(scripting_, "ANIM_SPEED");
        lua_pushcfunction(scripting_, TileMap::lua_MobWidth);
        lua_setglobal(scripting_, "MOB_WIDTH");
        lua_pushcfunction(scripting_, TileMap::lua_MobHeight);
        lua_setglobal(scripting_, "MOB_HEIGHT");
        lua_pushcfunction(scripting_, TileMap::lua_MobSpeed);
        lua_setglobal(scripting_, "MOB_SPEED");
        lua_pushcfunction(scripting_, TileMap::lua_MobLeash);
        lua_setglobal(scripting_, "MOB_LEASH");
        lua_pushcfunction(scripting_, TileMap::lua_MobCollisionBox);
        lua_setglobal(scripting_, "MOB_COLLISION_BOX");
        lua_pushcfunction(scripting_, TileMap::lua_MobAggroType);
        lua_setglobal(scripting_, "MOB_AGGRO_TYPE");
        lua_pushcfunction(scripting_, TileMap::lua_MobAggroRadius);
        lua_setglobal(scripting_, "MOB_AGGRO_RADIUS");
        lua_pushcfunction(scripting_, TileMap::lua_Biome);
        lua_setglobal(scripting_, "BIOME");
        lua_pushcfunction(scripting_, TileMap::lua_LootTable);
        lua_setglobal(scripting_, "LOOT_TABLE");
        lua_pushcfunction(scripting_, TileMap::lua_EndMobType);
        lua_setglobal(scripting_, "END_MOB_TYPE");
    }

    void TileMap::CleanupEntities()
    {
        entityTypes_.clear();
    }

    void TileMap::CleanupSpawners()
    {
        mobTypes_.clear();
    }

    TileMap* TileMap::GetTileMapObject(lua_State* L)
    {
        // retrieve "this" object
        lua_pushstring(L, "TileMap");
        lua_gettable(L, LUA_REGISTRYINDEX);
        TileMap* tileMap = (TileMap*)lua_touserdata(L, -1);
        lua_pop(L, 1);
        return tileMap;
    }

    // lua : LIQUIDS ( {ix,iy}... )
    int TileMap::lua_Liquids(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        int nargs = lua_gettop(L);
        for(int i = 1; i <= nargs; ++i)
        {
            lua_rawgeti(L, i, 1);
            int ix = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            lua_rawgeti(L, i, 2);
            int iy = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            tileMap->liquids_.push_back({(unsigned short)ix,(unsigned short)iy});
        }

        return 0;
    }

    // lua - BUILDABLE ( {ix,iy}... )
    int TileMap::lua_Buildable(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);
        int nargs = lua_gettop(L);
        for(int i=1; i <= nargs; ++i)
        {
            lua_rawgeti(L, i, 1);
            int ix = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            lua_rawgeti(L, i, 2);
            int iy = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            tileMap->buildable_.push_back({(unsigned short)ix, (unsigned short)iy});
        }
        return 0;
    }

    // lua - EMPTY ( ix, iy )
    int TileMap::lua_Empty(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);
        int ix = (int)lua_tointeger(L, 1);
        int iy = (int)lua_tointeger(L, 2);
        tileMap->empty_.ix = (unsigned short)ix;
        tileMap->empty_.iy = (unsigned short)iy;
        return 0;
    }

    // lua : SWIMMING ( string_pathToSwimmingTexture )
    int TileMap::lua_Swimming(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);
        tileMap->swimmingTexture_ = lua_tostring(L, 1);
        return 0;
    }

    // lua : BEGIN_ENTITY(string_nameOfEnt)
    int TileMap::lua_BeginEntity(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentEntityType_.name = lua_tostring(L, 1);
        return 0;
    }

    // lua  : USE_TEXTURE(path)
    int TileMap::lua_UseTexture(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentEntityType_.texturePath = lua_tostring(L, 1);
        return 0;
    }

    // lua : END_ENTITY ()
    int TileMap::lua_EndEntity(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->entityTypes_.push_back(tileMap->currentEntityType_);
        tileMap->currentEntityType_ = game::EntityType();
        return 0;
    }

    int TileMap::lua_Width(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentEntityType_.width = (int)lua_tonumber(L, 1);
        return 0;
    }

    int TileMap::lua_Height(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentEntityType_.height = (int)lua_tonumber(L, 1);
        return 0;
    }

    int TileMap::lua_CollisionBox(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        game::BOX box;
        box.left = (float)lua_tonumber(L, 1);
        box.top = (float)lua_tonumber(L, 2);
        box.right = (float)lua_tonumber(L, 3);
        box.bottom = (float)lua_tonumber(L, 4);
        tileMap->currentEntityType_.collision = box;

        return 0;
    }

    int TileMap::lua_MaxClicks(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentEntityType_.maxClicks = (int)lua_tonumber(L, 1);

        return 0;
    }

    int TileMap::lua_OnInteract(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        game::EntityType& et = tileMap->currentEntityType_; // easier to type
        for(int i=1; i < lua_gettop(L); i += 3)
        {
            game::EntityType::ItemDrop itemDrop;
            itemDrop.percentChance = (float)lua_tonumber(L, i);
            itemDrop.amount = (int)lua_tointeger(L, i+1);
            itemDrop.name = lua_tostring(L, i+2);
            et.drops.push_back(itemDrop);
        }

        return 0;
    }

    int TileMap::lua_ClickTime(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentEntityType_.clickTime = (float)lua_tonumber(L, 1);

        return 0;
    }

    // FARMABLE ( chance, amount, name, timer, pendingTexture )
    int TileMap::lua_Farmable(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        game::EntityType& et = tileMap->currentEntityType_;
        et.farmable = true;
        et.farmInfo.drop.percentChance = (float)lua_tonumber(L, 1);
        et.farmInfo.drop.amount = (int)lua_tonumber(L, 2);
        et.farmInfo.drop.name = lua_tostring(L, 3);
        et.farmInfo.respawnTime = (int)lua_tonumber(L, 4);
        et.farmInfo.pendingTexture = lua_tostring(L, 5);

        return 0;
    }

    // ON_DESTROY ( chance, num, name, ... )
    int TileMap::lua_OnDestroy(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        game::EntityType& et = tileMap->currentEntityType_;
        for(int i=1; i <= lua_gettop(L); i += 3)
        {
            game::EntityType::ItemDrop itemDrop;
            itemDrop.percentChance = (float)lua_tonumber(L, i);
            itemDrop.amount = (int)lua_tointeger(L, i+1);
            itemDrop.name = lua_tostring(L, i+2);
            et.onDestroy.push_back(itemDrop);
        }

        return 0;
    }

    int TileMap::lua_BeginMobType(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        const char* name = lua_tostring(L, 1);
        tileMap->currentMobType_.name = name;
        return 0;
    }

    int TileMap::lua_MobClass(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        const char* name = lua_tostring(L, 1);
        tileMap->currentMobType_.combatClass = name;
        return 0;
    }

    int TileMap::lua_LevelRange(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        int lower = (int)lua_tointeger(L, 1);
        int upper = (int)lua_tointeger(L, 2);
        tileMap->currentMobType_.levelLower = lower;
        tileMap->currentMobType_.levelUpper = upper;
        return 0;   
    }

    int TileMap::lua_DefaultAnimation(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        const char* defaultAnimation = lua_tostring(L, 1);
        tileMap->currentMobType_.defaultAnimation = defaultAnimation;
        return 0;  
    }

    int TileMap::lua_FrAnimTextureList(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentMobType_.frAnimTextureList.clear();
        for(int i=1; i <= lua_gettop(L); ++i)
        {
            const char* frame = lua_tostring(L, i);
            tileMap->currentMobType_.frAnimTextureList.push_back(frame);
        }
        return 0;
    }

    int TileMap::lua_BkAnimTextureList(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentMobType_.bkAnimTextureList.clear();
        for(int i=1; i <= lua_gettop(L); ++i)
        {
            const char* frame = lua_tostring(L, i);
            tileMap->currentMobType_.bkAnimTextureList.push_back(frame);
        }
        return 0;     
    }

    int TileMap::lua_LfAnimTextureList(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentMobType_.lfAnimTextureList.clear();
        for(int i=1; i <= lua_gettop(L); ++i)
        {
            const char* frame = lua_tostring(L, i);
            tileMap->currentMobType_.lfAnimTextureList.push_back(frame);
        }
        return 0;  
    }

    int TileMap::lua_RtAnimTextureList(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        tileMap->currentMobType_.rtAnimTextureList.clear();
        for(int i=1; i <= lua_gettop(L); ++i)
        {
            const char* frame = lua_tostring(L, i);
            tileMap->currentMobType_.rtAnimTextureList.push_back(frame);
        }
        return 0;  
    }

    int TileMap::lua_AnimSpeed(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        float animSpeed = (float)lua_tonumber(L, 1);
        tileMap->currentMobType_.animSpeed = animSpeed;
        return 0;
    }

    int TileMap::lua_MobWidth(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        int width = (int)lua_tointeger(L, 1);
        tileMap->currentMobType_.width = width;
        return 0;
    }

    int TileMap::lua_MobHeight(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        int height = (int)lua_tointeger(L, 1);
        tileMap->currentMobType_.height = height;
        return 0;
    }

    int TileMap::lua_MobSpeed(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        float speed = (float)lua_tonumber(L, 1);
        tileMap->currentMobType_.speed = speed;

        return 0;
    }

    int TileMap::lua_MobLeash(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        float leash = (float)lua_tonumber(L, 1);
        tileMap->currentMobType_.leash = leash;
        return 0;
    }

    int TileMap::lua_MobCollisionBox(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        game::BOX box = {
            (float)lua_tonumber(L, 1), (float)lua_tonumber(L, 2),
            (float)lua_tonumber(L, 3), (float)lua_tonumber(L, 4)
        };
        tileMap->currentMobType_.collisionBox = box;
        return 0;
    }

    int TileMap::lua_MobAggroType(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        const char* aggro = lua_tostring(L, 1);
        if(strcmp(aggro,"hostile")==0)
        {
            tileMap->currentMobType_.aggroType = game::MobType::AGGRO_TYPE::HOSTILE;
        }
        else 
        {
            tileMap->currentMobType_.aggroType = game::MobType::AGGRO_TYPE::NEUTRAL;
        }
        return 0;
    }

    int TileMap::lua_MobAggroRadius(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);
        tileMap->currentMobType_.aggroRadius = (int)lua_tointeger(L, 1);
        return 0;
    }

    int TileMap::lua_Biome(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);
        const char* biomeStr = lua_tostring(L, 1);
        if(strcmp(biomeStr,"both")==0)
            tileMap->currentMobType_.biome = game::MobType::BIOME::BOTH;
        else if(strcmp(biomeStr,"land")==0)
            tileMap->currentMobType_.biome = game::MobType::BIOME::LAND;
        else if(strcmp(biomeStr,"water")==0)
            tileMap->currentMobType_.biome = game::MobType::BIOME::WATER;
        return 0;
    }

    int TileMap::lua_LootTable(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);

        // arguments are "name", count, chance...
        for(int i=1; i <= lua_gettop(L); i += 3)
        {
            game::LootEntry le;
            le.item = lua_tostring(L, i);
            le.count = (int)lua_tointeger(L, i+1);
            le.chance = (float)lua_tonumber(L, i+2);
            tileMap->currentMobType_.lootTable.push_back(le);
        }
        return 0;
    }

    int TileMap::lua_EndMobType(lua_State* L)
    {
        TileMap* tileMap = GetTileMapObject(L);
        
        // push back the current object.
        tileMap->mobTypes_.push_back(tileMap->currentMobType_);
        return 0;
    }
}
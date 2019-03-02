// SoundManager.cpp
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

#include "SoundManager.h"

#include "GameEngine.h"

namespace engine
{

    SoundManager::SoundManager()
    {

    }

    SoundManager::~SoundManager()
    {
        /*if(music_)
        {
            Mix_HaltMusic();
            Mix_FreeMusic(music_);
        }

        for(auto it = sounds_.begin(); it != sounds_.end(); ++it)
        {
            Mix_FreeChunk(it->second);
        }*/
    }

    void SoundManager::LoadSound(const std::string& path)
    {
        auto found = sounds_.find(path);
        if(found == sounds_.end())
        {
            // not found so load and store
            Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
            if(chunk == nullptr)
            {
                GameEngine::Get().GetLogger().Logf(Logger::Severity::ERROR, "%s: %s",
                    __FUNCTION__, Mix_GetError());
                return;
            }
            else 
            {
                sounds_[path] = chunk;
            }
        }
        else 
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, "%s: `%s' already loaded.",
                __FUNCTION__, path.c_str());
        }
    }

    int SoundManager::PlaySound(const std::string& path)
    {
        auto found = sounds_.find(path);
        if(found == sounds_.end())
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::ERROR, "%s: `%s' not loaded.",
                __FUNCTION__, path.c_str());
            return -1;
        }
        else 
        {
            return Mix_PlayChannel(-1, found->second, 0);
        }
    }

    void SoundManager::HaltSound(int channel)
    {
        Mix_HaltChannel(channel);
    }

    void SoundManager::UnloadSound(const std::string& path)
    {
        auto found = sounds_.find(path);
        if(found != sounds_.end())
        {
            Mix_FreeChunk(found->second);
            sounds_.erase(found);
            return;
        }
        else 
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, "%s: `%s' was not loaded",
                __FUNCTION__, path.c_str());
        }
    }

    void SoundManager::PlayMusic(const std::string& path, int loops)
    {
        if(music_)
        {
            Mix_HaltMusic();
            Mix_FreeMusic(music_);
            music_ = nullptr;
        }
        music_ = Mix_LoadMUS(path.c_str());
        Mix_PlayMusic(music_, loops);
    }

    void SoundManager::StopMusic()
    {
        if(music_)
        {
            Mix_HaltMusic();
        }
    }

}
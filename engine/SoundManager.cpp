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

namespace engine
{

    SoundManager::SoundManager()
    {

    }

    SoundManager::~SoundManager()
    {
    
    }

    void SoundManager::PlaySound(const std::string& path)
    {

    }

    void SoundManager::PlayMusic(const std::string& path)
    {
        if(music_)
        {
            Mix_HaltMusic();
            Mix_FreeMusic(music_);
            music_ = nullptr;
        }
        music_ = Mix_LoadMUS(path.c_str());
        Mix_PlayMusic(music_, -1);
    }

    void SoundManager::StopMusic()
    {
        if(music_)
        {
            Mix_HaltMusic();
        }
    }

}
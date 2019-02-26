// Entity.cpp
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

#include "Entity.h"

#include "engine/GameEngine.h"

Entity::Entity(const ENTITY_TYPE& etype)
    : Sprite(nullptr, etype.width, etype.height), name_(etype.name), maxClicks_(etype.maxClicks), 
      remainingClicks_(etype.maxClicks)
{
    anim0_ = engine::GameEngine::Get().GetTextureManager().GetTexture(etype.texturePath);

}

Entity::~Entity()
{

}
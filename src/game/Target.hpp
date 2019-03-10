// Target.hpp
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

#include <glm/glm.hpp>

#include "ogl/Program.hpp"
#include "ogl/Texture.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/VertexBuffer.hpp"
#include "ogl/Vertex.hpp"
#include "Sprite.hpp"

namespace game
{

    class Target
    {
    public:
        // target types
        enum TARGET_TYPE { FRIENDLY, NEUTRAL, HOSTILE };
        // light RTTI data for what exactly to render above the sprite for the "healthbar"
        enum SPRITE_TYPE { MOBSPR, ENTSPR, NONE };
        Target();
        virtual ~Target();
        void SetTargetSprite(Sprite* sprite, const TARGET_TYPE tt, const SPRITE_TYPE st);
        inline Sprite* GetTargetSprite() { return target_; }
        inline SPRITE_TYPE GetTargetSpriteType() const { return targetSpriteType_; }
        inline TARGET_TYPE GetTargetType() const { return targetType_; }
        void Render(const glm::vec2& camera, ogl::Program& prog);
    private:
        // vertex data of the baseVbo_. Changes often enough to keep it in RAM
        ogl::Vertex vertices_[6];
        // ogl objects for rendering the base on the ground
        ogl::VertexArray baseVao_;
        ogl::VertexBuffer baseVbo_;
        // one of: (red/yellow/green)BaseTexture_
        ogl::Texture* currentBaseTexture_;
        // for coloring target base on hostile mob sprites
        ogl::Texture* redBaseTexture_;
        // for coloring target base on neutral mob sprites
        ogl::Texture* yellowBaseTexture_;
        // for coloring target base on friendly/static sprites
        ogl::Texture* greenBaseTexture_;
        // used to determine location of target on ground
        Sprite* target_;
        // if a sprite is an "Entity" the health bar is clicks / max clicks
        //  or if it is a mob then the health bar above represents health / max health
        SPRITE_TYPE targetSpriteType_;
        // Target type assigned to this target
        TARGET_TYPE targetType_;
        // ogl objects for rendering the health bar above the object
        ogl::Vertex fullHealthVerts_[6];
        ogl::VertexArray fullHealthVao_;
        ogl::VertexBuffer fullHealthVbo_;
        ogl::Vertex blankHealthVerts_[6];
        ogl::VertexArray blankHealthVao_;
        ogl::VertexBuffer blankHealthVbo_;
        ogl::Texture* blankHealthTexture_;
        ogl::Texture* redHealthTexture_;
        ogl::Texture* yellowHealthTexture_;
        ogl::Texture* greenHealthTexture_;
        ogl::Texture* colorHealthTexture_; // one of the above 3
    };

}
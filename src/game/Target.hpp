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
        // target hostility determines target colors: green, yellow, or red
        enum TARGET_TYPE { FRIENDLY, NEUTRAL, HOSTILE };
        // light RTTI data for what exactly to render above the sprite for the "healthbar"
        enum SPRITE_TYPE { BUILDSPR, MOBSPR, ENTSPR, NONE };

        // ctor
        Target();
        // dtor
        virtual ~Target();
        // Sets a sprite as the target
        void SetTargetSprite(Sprite* sprite, const TARGET_TYPE tt, const SPRITE_TYPE st);
        // Gets the sprite being targeted or NULL if none
        inline Sprite* GetTargetSprite() { return target_; }
        // Gets the type of sprite being targeted.
        inline SPRITE_TYPE GetTargetSpriteType() const { return targetSpriteType_; }
        // Gets the hostility of the target
        inline TARGET_TYPE GetTargetType() const { return targetType_; }
        // Draw the target base marker and bar
        void Render(const glm::vec2& camera, ogl::Program& prog);
    private:
        // vertex data of the baseVbo_. Changes often enough to keep it in RAM
        ogl::Vertex vertices_[6];
        // ogl objects for rendering the base on the ground
        ogl::VertexArray baseVao_;
        // see above
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
        // ogl objects for rendering the health bar above the object. Changes width depending on Sprite's data
        // the red green or yellow foreground bar above the target
        ogl::Vertex fullHealthVerts_[6];
        // see above
        ogl::VertexArray fullHealthVao_;
        // see above
        ogl::VertexBuffer fullHealthVbo_;
        // the black backdrop that is the same size of the colored health bar. Remains the same size
        ogl::Vertex blankHealthVerts_[6];
        // see above
        ogl::VertexArray blankHealthVao_;
        // see above
        ogl::VertexBuffer blankHealthVbo_;
        // a solid black texture indicated amounts of missing health or fewer clicks remaining. Class
        //  owns the pointer.
        ogl::Texture* blankHealthTexture_;
        // a solid red texture indicating health of hostile targets. This owns the pointer.
        ogl::Texture* redHealthTexture_;
        // a solid yellow texture indicating health of neutral targets. This owns the pointer.
        ogl::Texture* yellowHealthTexture_;
        // A solid green texture indicating health of friendly targets or remaining clicks of an entity.
        //  This owns the pointer.
        ogl::Texture* greenHealthTexture_;
        // The selection of one of the above 3 texture objects.
        ogl::Texture* colorHealthTexture_; 
    };

}
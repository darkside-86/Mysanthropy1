// AnimationEntry.hpp
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

#include <string>
#include <vector>

#include "CombatUnit.hpp"
#include "ogl/Program.hpp"
#include "ogl/Texture.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/VertexBuffer.hpp"

namespace combat
{

    // defines an database entry to construct individual animation objects as well as 
    //  the opengl objects needed to render an animation
    class AnimationEntry
    {
    public:
        
        // Enumerates the type of animation, determining where it is rendered on screen
        enum class Type 
        { 
            OnTarget, // directly in the center of target of spell
            OnSelf, // directly in the center of ability user
            AtTarget, // on the edge of user from center toward target
            Projectile // moves across screen until reaching center of target
            // possible future support for full screen filters like red edges around screen
            //  as seen in many games. This would easily be implemented by simply drawing a
            //  semi-transparent texture at 0,0 that fills the screen.
        };

        // Constructor
        AnimationEntry(const std::string& name, const int width, const int height, 
                const std::vector<std::string>& frames, 
                const Type type, const float speed, const float duration);
        
        // Destructor
        virtual ~AnimationEntry();

        // these parameters should be kept up with by an individual Animation object. To save
        //  memory we only need one animation entry object that gets rendered in different animation
        //  states at different locations as needed.
        void Render(int camX, int camY, ogl::Program& prog, 
                    float timeElapsed, int x, int y) const;

        // utility method for enum. Converts Lua database string representation of Type into
        //  the equivalent Type enum.
        static Type StringToType(const std::string& str);

        // Get the type of entry this is
        inline Type GetType() const { return type_; }

        // Get duration
        inline float GetDuration() const { return duration_; }

    private:
    // database entry fields //
        // database name
        std::string name_;
        // width in logical pixels
        int width_;
        // height in logical pixels
        int height_;
        // texture paths for each frame
        //  represented by frameTextures_
        // see enum Type
        Type type_;
        // time in seconds between frames
        float speed_; 
        // duration in seconds for how long animation should run
        float duration_;

    // objects to be built //
        std::vector<ogl::Texture*> frameTextures_;
        ogl::VertexArray vao_;
        ogl::VertexBuffer vbo_;
    };

}
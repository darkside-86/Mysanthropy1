// Sprite.hpp
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
#include <unordered_map>
#include <vector>

#include "engine/GameObject2D.hpp"
#include "ogl/Texture.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/VertexBuffer.hpp"

namespace game
{
    // A simple way of representing rectangle data on a screen. Used for sprite collision
    //  detection
    struct BOX
    {
        float   left=0.f, top=0.f, 
                right=0.f, bottom=0.f;
    };

    // An updatable and renderable object created from a textured quad. Movement can be
    //  calculated according to velocity and acceleration. Base class for most important
    //  on screen game objects. Also is capable of animation
    class Sprite : public engine::GameObject2D
    {
    public:
        // ctor
        Sprite(ogl::Texture* img, int width=0, int height=0);
        // dtor
        virtual ~Sprite();
        // Updates the position and velocity according to velocity and acceleration
        virtual void Update(float dtime) override;
        // Renders the sprite at its position and uses a camera parameter to determine where
        //  on screen to render. TODO: CULL OFF SCREEN!!!
        virtual void Render(const glm::vec2& camPos, ogl::Program& program) override;
        // Gets the width of the sprite in logical pixels
        int GetWidth() const { return width_; }
        // Gets the height of the sprite in logical pixels
        int GetHeight() const { return height_; }
        // Sets the current animation but does not start it. 
        //  maxTime is the number of seconds between each frame.
        void SetCurrentAnim(const std::string& name, float maxTime);
        // Gets the alias of the current animation or "" if not set
        std::string GetCurrentAnim() { return currentAnim_; }
        // Adds a frame to an existing animation set or creates the set and then adds it.
        void AddAnimFrame(const std::string& animName, ogl::Texture* texture);
        // Start running an animation that was previously made and set. Animation runs until paused or
        //  another animation alias is set with SetCurrentAnim
        void StartAnimation() { animating_ = true; }
        // Pause the animation currently running if any
        void PauseAnimation() { animating_ = false;}
        // set relative (0,0,w,h e.g) values for collision box
        void SetCollisionBox(float left, float top, float right, float bottom);
        // return absolute position of collision box based on position_
        void GetCollisionBox(float &left, float& top, float &right, float &bottom);
        // return true if collision box is valid for checking anything
        bool HasValidCollisionBox();
        // reverse movement based on time
        void ReverseMovement(float dtime);
    protected:
        // The dimensions of the sprite in logical pixels
        int width_ = 0, height_ = 0;
        // The default texture to use when no animation data is present
        ogl::Texture* anim0_;
        // The opengl objects for rendering the quad
        ogl::VertexArray vao_;
        // See above
        ogl::VertexBuffer vbo_;
        // The current animation (controlled with adding and setting animations). A value of ""
        //  indicates that there is no animating to be done and anim0_ is the texture to use
        std::string currentAnim_ = "";
        // The current animation frame
        int currentFrame_ = 0;
        // The timer for the current animation frame. When this reaches maxFrameTime_ the next
        //  frame in the animation is used.
        float currentTime_ = 0.f;
        // The amount of time in seconds between each frame
        float maxFrameTime_ = 1.f;
        // Whether or not animation is running or paused.
        bool animating_ = true;
        // A table of animations with alias as keys and texture/frame list as value
        std::unordered_map<std::string, std::vector<ogl::Texture*> > animFrames_;
        // The collision box of the sprite. Numbers are relative (e.g. 0,0, 32, 32)
        BOX collisionBox_;
    };

}
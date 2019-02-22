// Sprite.h
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
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "engine/GameObject.h"
#include "ogl/Texture.h"
#include "ogl/VertexArray.h"
#include "ogl/VertexBuffer.h"

class Sprite : public engine::GameObject
{
public:
    Sprite(ogl::Texture* img);
    virtual ~Sprite();
    void Update(float dtime) override;
    void Render(const glm::vec3& camPos, ogl::Program& program) override;
    void SetCurrentAnim(const std::string& name, float maxTime);
    std::string GetCurrentAnim() { return currentAnim_; }
    void AddAnimFrame(const std::string& animName, ogl::Texture* texture);
    void StartAnimation() { animating_ = true; }
    void PauseAnimation() { animating_ = false;}
protected:
    int width_ = 0, height_ = 0;
    ogl::Texture* anim0_;
    ogl::VertexArray vao_;
    ogl::VertexBuffer vbo_;
    std::string currentAnim_ = "";
    int currentFrame_ = 0;
    float currentTime_ = 0.f;
    float maxFrameTime_ = 1.f;
    bool animating_ = true;
    std::unordered_map<std::string, std::vector<ogl::Texture*> > animFrames_;
};
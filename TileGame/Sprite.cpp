// Sprite.cpp
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ogl/ErrorCheck.hpp"
#include "ogl/Vertex.hpp"
#include "Sprite.hpp"

Sprite::Sprite(ogl::Texture* img, int width, int height) :
    anim0_(img)
{
    if(img == nullptr)
    {
        width_ = 0;
        height_ = 0;
    }
    else 
    {
        width_ = width == 0 ? anim0_->GetWidth() : width;
        height_ = height == 0 ? anim0_->GetHeight() : height;
    }
    float w = (float)width_;
    float h = (float)height_;
    ogl::Vertex vertices[6] = {
        {{0.f,0.f,0.f},{255,255,255,255},{0.f,0.f},{0.f,0.f,1.f}},
        {{0.f,  h,0.f},{255,255,255,255},{0.f,1.f},{0.f,0.f,1.f}},
        {{  w,  h,0.f},{255,255,255,255},{1.f,1.f},{0.f,0.f,1.f}},
        {{0.f,0.f,0.f},{255,255,255,255},{0.f,0.f},{0.f,0.f,1.f}},
        {{  w,  h,0.f},{255,255,255,255},{1.f,1.f},{0.f,0.f,1.f}},
        {{  w,0.f,0.f},{255,255,255,255},{1.f,0.f},{0.f,0.f,1.f}}
    };
    vbo_.SetData(6*sizeof(ogl::Vertex), vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_.AddBuffer(vbo_, vbl);
}

Sprite::~Sprite()
{

}

void Sprite::Update(float dtime)
{
    position_ += dtime * velocity_;
    velocity_ += dtime * acceleration_;
    if(animating_)
    {
        currentTime_ += dtime;
        if(currentTime_ >= maxFrameTime_)
        {
            currentFrame_++;
            currentTime_ -= maxFrameTime_;
            if(animFrames_.find(currentAnim_) != animFrames_.end())
            {
                if(currentFrame_ >= animFrames_[currentAnim_].size())
                {
                    currentFrame_ = 0;
                }
            }
        }
    }
}

void Sprite::Render(const glm::vec3& camPos, ogl::Program& program)
{
    glm::mat4 model(1.f);
    model = glm::translate(model, position_ + camPos);
    program.Use();
    program.SetUniform("u_model", model);
    vao_.Bind();
    if(currentAnim_ == "")
    {
        anim0_->Bind();
    }
    else 
    {
        animFrames_[currentAnim_][currentFrame_]->Bind();
    }
    OGL_ERROR_CHECK();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    OGL_ERROR_CHECK();
}


void Sprite::SetCurrentAnim(const std::string& name, float timer)
{
    currentAnim_ = name;
    maxFrameTime_ = timer;
    currentTime_ = 0.f;
    currentFrame_ = 0;
}

void Sprite::AddAnimFrame(const std::string& animName, ogl::Texture* texture)
{
    auto& findTable = animFrames_.find(animName);
    if(findTable == animFrames_.end())
    {
        animFrames_[animName] = std::vector<ogl::Texture*>();
    }
    animFrames_[animName].push_back(texture);
}

void Sprite::SetCollisionBox(float left, float top, float right, float bottom)
{
    collisionBox_ = { left, top, right, bottom };
}

void Sprite::GetCollisionBox(float &left, float& top, float &right, float &bottom)
{
    left = collisionBox_.left + position_.x;
    top = collisionBox_.top + position_.y;
    right = collisionBox_.right + position_.x;
    bottom = collisionBox_.bottom + position_.y;
}

bool Sprite::HasValidCollisionBox()
{
    return collisionBox_.left != -1;
}
// AnimationEntry.cpp
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

#include <glm/gtc/matrix_transform.hpp>

#include "AnimationEntry.hpp"
#include "engine/GameEngine.hpp"
#include "ogl/Vertex.hpp"

namespace combat
{

    AnimationEntry::AnimationEntry(const std::string& name, const int width, const int height, 
            const std::vector<std::string>& frames, 
            const Type type, const float speed, const float duration)
    : name_(name), width_(width), height_(height), type_(type), 
        speed_(speed), duration_(duration)
    {
        // construct opengl objects
        float w = (float)width_;
        float h = (float)height_;
        ogl::Vertex vertices[6] = {
            {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f,0.f}, {0.f,0.f,1.f}},
            {{0.f,   h, 0.f}, {255,255,255,255}, {0.f,1.f}, {0.f,0.f,1.f}},
            {{  w,   h, 0.f}, {255,255,255,255}, {1.f,1.f}, {0.f,0.f,1.f}},
            {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f,0.f}, {0.f,0.f,1.f}},
            {{  w,   h, 0.f}, {255,255,255,255}, {1.f,1.f}, {0.f,0.f,1.f}},
            {{  w, 0.f, 0.f}, {255,255,255,255}, {1.f,0.f}, {0.f,0.f,1.f}}
        };
        vbo_.SetData(6*sizeof(ogl::Vertex), vertices, GL_STATIC_DRAW);
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        vao_.AddBuffer(vbo_, vbl);
        // load the animation textures.
        for(const auto& fr : frames)
        {
            frameTextures_.push_back(new ogl::Texture(fr));
        }
    }

    AnimationEntry::~AnimationEntry()
    {
        for(auto& texture : frameTextures_)
        {
            delete texture;
        }
    }

    void AnimationEntry::Render(int camX, int camY, ogl::Program& prog,
                                float timeElapsed, int x, int y) const
    {
        // select the program
        prog.Use();
        // center the quad around x,y parameters and create and set the model matrix
        float w = (float)width_;
        float h = (float)height_;
        glm::vec3 position((float)camX + (float)x - w/2.f, (float)camY + (float)y - h/2.f, 0.0f);
        // glm::vec3 position((float)x - w/2.0f, (float)y - h/2.0f, 0.0f);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        prog.SetUniform("u_model", model);
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
            "I want to render the animation at {%f,%f,%f}", position.x, position.y, position.z);
        // select the frame texture to bind based on elapsed time.
        int maxFrames = (int)frameTextures_.size();
        int numFramesPassed = (int)(timeElapsed / speed_);
        int frame = numFramesPassed % maxFrames;
        frameTextures_[frame]->Bind();
        // bind the vao
        vao_.Bind();
        // draw the quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    AnimationEntry::Type AnimationEntry::StringToType(const std::string& str)
    {
        if(str == "at_target") return Type::AtTarget;
        else if(str == "on_self") return Type::OnSelf;
        else if(str == "on_target") return Type::OnTarget;
        else if(str == "projectile") return Type::Projectile;
        else return Type::AtTarget; // sane default
    }

}

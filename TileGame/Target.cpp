// Target.cpp
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

#include "Entity.hpp"
#include "MobSprite.hpp"
#include "ogl/ErrorCheck.hpp"
#include "Target.hpp"

Target::Target()
{
    // load textures
    redBaseTexture_ = new ogl::Texture("res/textures/target_red.png");
    yellowBaseTexture_ = new ogl::Texture("res/textures/target_yellow.png");
    greenBaseTexture_ = new ogl::Texture("res/textures/target_green.png");
    // fill in vertex data
    vertices_[0] = {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}; // top left
    vertices_[1] = {{0.0f, 1.0f, 0.0f}, {255,255,255,255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}; // btm left
    vertices_[2] = {{1.0f, 1.0f, 0.0f}, {255,255,255,255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}; // btm right
    vertices_[3] = {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}; // top left
    vertices_[4] = {{1.0f, 1.0f, 0.0f}, {255,255,255,255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}; // btm right
    vertices_[5] = {{1.0f, 0.0f, 0.0f}, {255,255,255,255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}; // top right
    // setup vao and vbo
    baseVbo_.SetData(sizeof(ogl::Vertex)*6, vertices_, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    baseVao_.AddBuffer(baseVbo_, vbl);

    // setup healthbar info
    redHealthTexture_ = new ogl::Texture("res/textures/bar_red.png");
    yellowHealthTexture_ = new ogl::Texture("res/textures/bar_yellow.png");
    greenHealthTexture_ = new ogl::Texture("res/textures/bar_green.png");
    blankHealthTexture_ = new ogl::Texture("res/textures/bar_black.png");
    fullHealthVerts_[0] = {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
    fullHealthVerts_[1] = {{0.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
    fullHealthVerts_[2] = {{1.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
    fullHealthVerts_[3] = {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
    fullHealthVerts_[4] = {{1.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
    fullHealthVerts_[5] = {{1.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
    memcpy(blankHealthVerts_, fullHealthVerts_, 6 * sizeof(ogl::Vertex));
    // vbl is the same--using same ogl::Vertex class layout
    fullHealthVbo_.SetData(6 * sizeof(ogl::Vertex), fullHealthVerts_, GL_STATIC_DRAW);
    fullHealthVao_.AddBuffer(fullHealthVbo_, vbl);
    blankHealthVbo_.SetData(6 * sizeof(ogl::Vertex), blankHealthVerts_, GL_STATIC_DRAW);
    blankHealthVao_.AddBuffer(blankHealthVbo_, vbl);
}

Target::~Target()
{
    delete redBaseTexture_;
    delete yellowBaseTexture_;
    delete greenBaseTexture_;

    delete redHealthTexture_;
    delete yellowHealthTexture_;
    delete greenHealthTexture_;
    delete blankHealthTexture_;
}

void Target::SetTargetSprite(Sprite* sprite, const Target::TARGET_TYPE tt, const Target::SPRITE_TYPE st)
{
    target_ = sprite;
    targetSpriteType_ = st;
    if(target_ != nullptr)
    {
        // set vbo data to reflect size of sprite. The target base is drawn at the bottom 1/4 height of
        // the sprite and the width of the target is the width of the sprite. The origin(0,0) to be
        // rendered is calculated during each render cycle by the camera and sprite location.
        float w = (float)target_->GetWidth();
        float h = (float)target_->GetHeight() / 4.f;
        vertices_[0] = {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}; // top left
        vertices_[1] = {{0.0f,    h, 0.0f}, {255,255,255,255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}; // btm left
        vertices_[2] = {{   w,    h, 0.0f}, {255,255,255,255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}; // btm right
        vertices_[3] = {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}; // top left
        vertices_[4] = {{   w,    h, 0.0f}, {255,255,255,255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}; // btm right
        vertices_[5] = {{   w, 0.0f, 0.0f}, {255,255,255,255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}; // top right
        baseVbo_.SetData(6*sizeof(ogl::Vertex), vertices_, GL_STATIC_DRAW);
        // The texture selected depends on the target type.
        switch(tt)
        {
            case TARGET_TYPE::FRIENDLY: 
                currentBaseTexture_ = greenBaseTexture_; 
                colorHealthTexture_ = greenHealthTexture_;
                break;
            case TARGET_TYPE::NEUTRAL: 
                currentBaseTexture_ = yellowBaseTexture_; 
                colorHealthTexture_ = yellowHealthTexture_;
                break;
            case TARGET_TYPE::HOSTILE: 
                currentBaseTexture_ = redBaseTexture_; 
                colorHealthTexture_ = redHealthTexture_;
                break;
        }
        // Set the floating health/progress bar data to the base size. The fill vertex data changes
        // render but the black health backdrop should be set once here. The health bars are "h"]
        // pixels tall and the (already set) width of the Sprite, "w"
        h = 2.0f;
        blankHealthVerts_[0] = {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
        blankHealthVerts_[1] = {{0.0f,    h, 0.0f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
        blankHealthVerts_[2] = {{   w,    h, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
        blankHealthVerts_[3] = {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
        blankHealthVerts_[4] = {{   w,    h, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
        blankHealthVerts_[5] = {{   w, 0.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
        memcpy(fullHealthVerts_, blankHealthVerts_, 6 * sizeof(ogl::Vertex));
        blankHealthVbo_.SetData(6*sizeof(ogl::Vertex), blankHealthVerts_, GL_STATIC_DRAW);
        fullHealthVbo_.SetData(6*sizeof(ogl::Vertex), fullHealthVerts_, GL_STATIC_DRAW);
    }
}

void Target::Render(const glm::vec3 camera, ogl::Program& prog)
{
    if(target_ == nullptr)
        return;

    // first render the base circle
    glm::vec3 pos = target_->GetPosition();
    pos.y = pos.y + 3.f * (float)target_->GetHeight() / 4.f;
    pos += camera;
    glm::mat4 model = glm::translate(glm::mat4(1.f), pos);
    prog.Use();
    prog.SetUniform("u_model", model);
    baseVao_.Bind();
    currentBaseTexture_->Bind();
    OGL_ERROR_CHECK();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    OGL_ERROR_CHECK();

    // next render the health bar above based on type of Sprite and given info.
    // Start with fresh position and go 2*2 pixels up for where to render bar
    float newX = 0.0f; // for calculating the width of fill bar
    if(targetSpriteType_ == SPRITE_TYPE::ENTSPR)
    {
        Entity* ent = (Entity*)target_;
        if(ent->GetMaxClicks() == -1)
        {
            // health bar is not valid to show in this case.
            return;
        }
        float value = (float)ent->GetRemainingClicks() / (float)ent->GetMaxClicks();
        newX = value * (float)ent->GetWidth();
    }
    else if(targetSpriteType_ == SPRITE_TYPE::MOBSPR)
    {
        MobSprite* mob = (MobSprite*)target_;
        float value = (float)mob->GetCombatUnit().GetCurrentHealth() /
            (float)mob->GetCombatUnit().GetMaxHealth();
        newX = value * (float)mob->GetWidth();
    }
    pos = target_->GetPosition();
    pos.y -= 4.0f;
    pos += camera;
    model = glm::translate(glm::mat4(1.f), pos);
    prog.SetUniform("u_model", model);
    blankHealthVao_.Bind();
    blankHealthTexture_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // modify fullHealthVerts_[2,4,5]'s position.x values based on Sprite data
    fullHealthVerts_[2].position.x = newX;
    fullHealthVerts_[4].position.x = newX;
    fullHealthVerts_[5].position.x = newX;
    fullHealthVbo_.SetData(6*sizeof(ogl::Vertex), fullHealthVerts_, GL_STATIC_DRAW);
    fullHealthVao_.Bind();
    colorHealthTexture_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool Target::IsTargetEntity(const Entity* entity)
{
    if(targetSpriteType_ == SPRITE_TYPE::ENTSPR)
    {
        return entity == (Entity*)target_;
    }
    return false;
}
// Root.cpp
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

#include "Root.h"

namespace engine { namespace ui {

    void Root::ProcessMouseButtonEvent(const SDL_MouseButtonEvent& mbe)
    {
        int x = mbe.x, y = mbe.y;
        GameEngine::Get().SetLogicalXY(x,y);
        if(mbe.type == SDL_MOUSEBUTTONDOWN)
        {
            mousePressed_ = CheckPoint(x,y);
            mouseDown_ = true;
        }
        else
        {
            mouseDown_ = false;
            Object* unpressed = CheckPoint(x,y);
            if(mousePressed_ == unpressed && unpressed != nullptr)
            {
                GetNodePosition(unpressed, x, y);
                unpressed->OnClicked(ClickedEvent(x, y, mbe.button));
            }
        }
    }

    void Root::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& mme)
    {
        int x = mme.x, y = mme.y;
        int dx = mme.xrel, dy = mme.yrel;
        GameEngine::Get().SetLogicalXY(x,y);
        GameEngine::Get().SetLogicalXY(dx,dy);
        Object* over =  CheckPoint(x,y);
        // drag
        if(mouseDown_ && mousePressed_ != nullptr)
        {
            GetNodePosition(mousePressed_, x, y);
            mousePressed_->OnDragged(DraggedEvent(x,y,dx,dy));
        }
        // hover
        if(over != nullptr)
        {
            if(over != mouseOver_)
            {
                over->OnHover(HoverEvent(x,y,dx,dy,true));
                if(mouseOver_ != nullptr)
                {
                    if(mouseOver_ != over->GetParent())
                    {
                        GetNodePosition(mouseOver_, x, y);
                        mouseOver_->OnHover(HoverEvent(x,y,dx,dy,false));
                    }
                }
                mouseOver_ = over;
            }
        }
    }

    void Root::ProcessKeyboardEvent(const SDL_KeyboardEvent& kbe)
    {
        if(kbe.type == SDL_KEYDOWN)
        {
            if(mousePressed_ != nullptr)
            {
                mousePressed_->OnKeypressed(KeypressedEvent(kbe.keysym.sym, kbe.keysym.scancode, kbe.keysym.mod, 
                    kbe.repeat));
            }
        }
    }

    void Root::GetNodePosition(Object* node, int& x, int &y)
    {
        Object* p = node;
        while(p != nullptr)
        {
            x -= p->GetXPos();
            y -= p->GetYPos();
            p = p->GetParent();
        }
    }

    void Root::Render(GraphicsContext& gc)
    {
        // TODO: preserve and restore original states
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        // glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        gc.GetProgram().Use();
        gc.GetProgram().SetUniform<int>("u_useTexture", true);
        gc.GetProgram().SetUniform<int>("u_useColorBlending", true);
        gc.ResetMVP();
        gc.SetOrthoProjection();
        gc.SetMVP();
        gc.TranslateModel((float)xPos_, (float)yPos_, 0.f);
        gc.SetMVP();
        for(auto child : children_)
        {
            child->Render(gc);
        }
        // glDisable(GL_BLEND);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_DEPTH_TEST);
    }

    void Root::Initialize()
    {
        GameEngine::Get().AddMouseButtonListener([this](const SDL_MouseButtonEvent& e){
            this->ProcessMouseButtonEvent(e);
        });

        GameEngine::Get().AddMouseMotionListener([this](const SDL_MouseMotionEvent& e) {
            this->ProcessMouseMotionEvent(e);
        });

        GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e) {
            this->ProcessKeyboardEvent(e);
        });
    }

    void Root::RegisterObjectDeletion(Object* obj)
    {
        if(mousePressed_ == obj)
            mousePressed_ = nullptr;
        if(mouseOver_ == obj)
            mouseOver_ = nullptr;
    }

}}
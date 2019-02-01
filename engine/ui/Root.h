// Root.h
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

#include <SDL/SDL.h>

#include "../GameEngine.h"

#include "Object.h"

namespace engine { namespace ui {
    // handles the UI event system as the root of UI Object tree
    class Root : public Object 
    {
    public:
        virtual ~Root() {}
        virtual void Render(GraphicsContext& gc) override;
        void ProcessMouseButtonEvent(const SDL_MouseButtonEvent& mbe);
        void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& mme);
        void ProcessKeyboardEvent(const SDL_KeyboardEvent& kbe);
        void GetNodePosition(Object* node, int& x, int &y);
        static Root* Get() 
        {
            static Root* root = nullptr;
            if(root == nullptr)
                root = new Root();
            return root;
        }
    private:
        Root() : Object(nullptr)
        {
            xPos_ = 0;
            yPos_ = 0;
            width_ = GameEngine::Get().GetWidth();
            height_ = GameEngine::Get().GetHeight();
        }
        Object*     mousePressed_ = nullptr;
        Object*     mouseOver_ = nullptr;
        bool        mouseDown_ = false;
    };

}}
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
    // Defines the root parent of the entire UI object tree. Manages the callback lists by translating
    // SDL events into the equivalent UI event for the appropriate object in the tree.
    class Root : public Object 
    {
    public:
        // Destructor
        virtual ~Root() {}
        // Simply begins the render cycle of rendering from the base of the tree downward. The Root itself
        // has no pixels to render.
        virtual void Render(GraphicsContext& gc) override;
        // Converts an SDL_MouseButtonEvent into the appropriate event for an object in the tree
        void ProcessMouseButtonEvent(const SDL_MouseButtonEvent& mbe);
        // Converts an SDL_MouseMotionEvent into the appropriate UI event for an object in the tree
        void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& mme);
        // Converts an SDL_KeyboardEvent into the appropriate UI event for an object in the tree
        void ProcessKeyboardEvent(const SDL_KeyboardEvent& kbe);
        // ...(TODO: remember what this does and document it)
        void GetNodePosition(Object* node, int& x, int &y);
        // Set up SDL_Event handlers to start the Proces...Event functions
        void Initialize();
        // Register object deletion (in case we need to clear mouse data pointers)
        void RegisterObjectDeletion(Object* obj);
        // Returns the singleton instance
        static Root* Get() 
        {
            static Root* root = nullptr;
            if(root == nullptr)
                root = new Root();
            return root;
        }
    private:
        // Constructor. The width and height of this object are the logical screen dimensions
        Root() : Object(nullptr)
        {
            xPos_ = 0;
            yPos_ = 0;
            width_ = GameEngine::Get().GetWidth();
            height_ = GameEngine::Get().GetHeight();
        }
        // Keeps track of what object was targeted by a mouse press
        Object*     mousePressed_ = nullptr;
        // Keeps track of what object is currently being hovered
        Object*     mouseOver_ = nullptr;
        // Keeps track of whether a mouse button is up or down.
        // TODO: which button??? May be the source of "unrepeatable" "random" crashes
        bool        mouseDown_ = false;
    };

}}
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
        if(mbe.type != SDL_MOUSEBUTTONDOWN)
            return;

        int x = mbe.x;
        int y = mbe.y;

        GameEngine::Get().SetLogicalXY(x,y);

        Object* clicked = CheckPoint(x,y);
        if(clicked != nullptr)
        {
            clicked->OnClicked(x,y, mbe.button);
        }
    }

    void Root::Render(GraphicsContext& gc)
    {
        gc.ResetModel();
        gc.TranslateModel((float)xPos_, (float)yPos_, 0.f);
        gc.SetMVP();
        for(auto child : children_)
        {
            child->Render(gc);
        }
    }

}}
// Item.hpp
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

#include "ogl/Texture.hpp"

namespace game
{

    class Item
    {
    public:
        Item(const std::string& name, ogl::Texture* icon, bool hidden=false);
        virtual ~Item();
        inline std::string GetName() { return name_; }
        inline ogl::Texture* GetIcon() { return icon_; }
        // TODO: Render?
        inline bool IsHiddenFromInventory() { return hiddenFromInventory_; }
    private:
        std::string name_;
        ogl::Texture* icon_;
        bool hiddenFromInventory_ = false;
    };

}
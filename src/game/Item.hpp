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
    // defines information about an inventory item
    class Item
    {
    public:
        // ctor
        Item(const std::string& name, const std::string& texturePath, ogl::Texture* icon, bool hidden=false)
            : name_(name), texturePath_(texturePath), icon_(icon), hiddenFromInventory_(hidden) {}
        // Name of item, serves as a database id
        inline std::string GetName() const { return name_; }
        // Path to the texture associated with this item
        inline std::string GetTexturePath() const { return texturePath_; }
        // The texture that is associated with the texture path. Item does not create this pointer
        inline ogl::Texture* GetIcon() { return icon_; }
        // Gets whether or not an inventory UI system should display the item
        inline bool IsHiddenFromInventory() { return hiddenFromInventory_; }
    private:
        // database primary key
        std::string name_;
        // path to the icon associated with the item
        std::string texturePath_;
        // Graphical representation of item for UI purposes. Item does not own this pointer!
        ogl::Texture* icon_;
        // Whether or not item should be shown in an inventory UI
        bool hiddenFromInventory_ = false;
    };

}
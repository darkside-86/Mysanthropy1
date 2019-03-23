// Equipment.hpp
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

#include "EquipmentEntry.hpp"

namespace combat
{
    // Represents an item that becomes a piece of equipment when placed in slot
    class Equipment
    {
    public:
        Equipment(const EquipmentEntry&, int remainingDurability);
        
        inline const EquipmentEntry& GetEntry() const { return equipmentEntry_; }
        inline int GetRemainingDurability() const { return remainingDurability_; }
        void SetRemainingDurability(int dur);
        inline bool IsBroken() const { return remainingDurability_ == 0; }
        int GetAttribute(const Attribute attr) const;

    private:
        const EquipmentEntry& equipmentEntry_;
        int remainingDurability_;
    };
}
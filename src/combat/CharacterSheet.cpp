// CharacterSheet.cpp
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

#include "CharacterSheet.hpp"

namespace combat
{
    CharacterSheet::CharacterSheet(int level, bool player, const CombatClassEntry& cce)
        : combatClassEntry_(cce), player_(player)
    {
        SetLevel(level);
    }

    CharacterSheet::~CharacterSheet()
    {
        delete headSlot_;
        delete neckSlot_;
        delete shoulderSlot_;
        delete chestSlot_;
        delete beltSlot_;
        delete handsSlot_;
        delete leggingsSlot_;
        delete shoesSlot_;
        delete ring0Slot_;
        delete ring1Slot_;
        delete trinket0Slot_;
        delete trinket1Slot_;
        delete weaponLeft_;
        delete weaponRight_;
    }

    int CharacterSheet::CalculateAttribute(Attribute attr)
    {
        if(attr == Attribute::Level)
            return level_;
        
        const auto found = cachedAttributes_.find(attr);
        if(found != cachedAttributes_.end())
            return found->second;
        // else gotta calculate it
        int baseNumber = 0;
        // core stats built into character
        switch(attr)
        {
        case Attribute::Strength: baseNumber += strength_; break;
        case Attribute::Agility: baseNumber += agility_; break;
        case Attribute::Intellect: baseNumber += intellect_; break;
        case Attribute::Dexterity: baseNumber += dexterity_; break;
        case Attribute::Wisdom: baseNumber += wisdom_; break;
        case Attribute::Knowledge: baseNumber += knowledge_; break;
        case Attribute::Vitality: baseNumber += vitality_; break;
        }
        // look in each piece of armor for the stat
        if(headSlot_ != nullptr)
            baseNumber += headSlot_->GetAttribute(attr);
        if(neckSlot_ != nullptr)
            baseNumber += neckSlot_->GetAttribute(attr);
        if(shoulderSlot_ != nullptr)
            baseNumber += shoulderSlot_->GetAttribute(attr);
        if(chestSlot_ != nullptr)
            baseNumber += chestSlot_->GetAttribute(attr);
        if(beltSlot_ != nullptr)
            baseNumber += beltSlot_->GetAttribute(attr);
        if(handsSlot_ != nullptr)
            baseNumber += handsSlot_->GetAttribute(attr);
        if(leggingsSlot_ != nullptr)
            baseNumber += leggingsSlot_->GetAttribute(attr);
        if(shoesSlot_ != nullptr)
            baseNumber += shoesSlot_->GetAttribute(attr);
        if(ring0Slot_ != nullptr)
            baseNumber += ring0Slot_->GetAttribute(attr);
        if(ring1Slot_ != nullptr)
            baseNumber += ring1Slot_->GetAttribute(attr);
        if(trinket0Slot_ != nullptr)
            baseNumber += trinket0Slot_->GetAttribute(attr);
        if(trinket1Slot_ != nullptr)
            baseNumber += trinket1Slot_->GetAttribute(attr);
        if(weaponLeft_ != nullptr)
            baseNumber += weaponLeft_->GetAttribute(attr);
        if(weaponRight_ != nullptr)
            baseNumber += weaponRight_->GetAttribute(attr);
        // apply buffs/debuffs to base number (character sheet and gear)
        int statAdd = 0;
        float multiplier = 1.f;
        for(const auto& effect : statusEffects_)
        {
            if(effect->IsActive())
            {
                for(const auto& add : effect->GetAdded())
                {
                    if(add.attribute == attr)
                    {
                        statAdd += add.amount;
                    }
                }
                for(const auto& mult : effect->GetMult())
                {
                    if(mult.attribute == attr)
                    {
                        multiplier *= mult.coefficient;
                    }
                }
            }
        }
        // finally determine the result by multiplying the baseStat and then adding adds
        baseNumber = (int)((float)baseNumber * multiplier);
        baseNumber += statAdd;
        // and cache it
        cachedAttributes_[attr] = baseNumber;
        return baseNumber;
    }

    
    int CharacterSheet::GetMaxHealth()
    {
        // max health is determined by 5 * calculated VIT + 1 * LVL + MH - 1
        int vit = CalculateAttribute(Attribute::Vitality);
        int mh = CalculateAttribute(Attribute::MaxHealth);
        return 5 * vit + mh + level_ - 1;
    }

    int CharacterSheet::GetMaxMana()
    {
        // max mana is determined by 5 * INT and Max mana 
        int i = CalculateAttribute(Attribute::Intellect);
        int mm = CalculateAttribute(Attribute::MaxMana);
        return 5 * i + mm;
    }

    float CharacterSheet::GetArmorPercent()
    {
        // armor percent is calculated ARM * (0.95 ^ (LVL-1))
        int arm = CalculateAttribute(Attribute::Armor);
        float m = pow(0.95f, (float)(level_ - 1));
        return ((float)arm * m);
    }

    float CharacterSheet::GetPhysicalCritChance()
    {
        // first turn crit rating from dexterity into a percent
        int dex = CalculateAttribute(Attribute::Dexterity);
        float m = pow(0.945f, (float)(level_ - 1));
        float baseChance = ((float)dex * m);
        // add any raw crit chance buffs
        baseChance += (float)CalculateAttribute(Attribute::CriticalChance);
        return baseChance;
    }

    float CharacterSheet::GetDodgeChance()
    {
        // first derive base dodge chance from dexterity
        int dex = CalculateAttribute(Attribute::Dexterity);
        float m = pow(0.94f, (float)(level_ -1));
        float baseChance = ((float)dex * m);
        // add any raw dodge chance buffs
        baseChance += (float)CalculateAttribute(Attribute::DodgeChance);
        return baseChance;
    }

    float CharacterSheet::GetLeechPercent()
    {
        // first derive base leech based on rating
        int leech = CalculateAttribute(Attribute::Leech);
        float m = pow(0.935f, (float)(level_ - 1));
        float basePercent = ((float)leech * m);
        // add any raw percent from buffs
        basePercent += (float)CalculateAttribute(Attribute::LeechPercent);
        return basePercent;
    }

    float CharacterSheet::GetSpeedPercent()
    {
        // first derive base speed rating
        int speed = CalculateAttribute(Attribute::Speed);
        float m = pow(0.93f, (float)(level_ -1));
        float basePercent = ((float)speed * m);
        return basePercent; // that's all for now maybe direct speed increases in the future
    }

    float CharacterSheet::GetSpellCritChance()
    {
        // first derive base chance from intellect
        int i = CalculateAttribute(Attribute::Intellect);
        float m = pow(0.925f, (float)(level_ - 1));
        float basePercent = ((float)i * m);
        basePercent += (float)CalculateAttribute(Attribute::CriticalChance);
        return basePercent;
    }

    int CharacterSheet::GetMeleeAttackPower()
    {
        int str = CalculateAttribute(Attribute::Strength);
        str += CalculateAttribute(Attribute::MeleeAttackPower);
        return str;
    }

    int CharacterSheet::GetRangedAttackPower()
    {
        int agi = CalculateAttribute(Attribute::Agility);
        agi += CalculateAttribute(Attribute::RangedAttackPower);
        return agi;
    }

    int CharacterSheet::GetSpellPower()
    {
        int sp = CalculateAttribute(Attribute::Intellect);
        sp += CalculateAttribute(Attribute::SpellPower);
        return sp;
    }

    int CharacterSheet::GetResistance(OutputType t)
    {
        switch(t)
        {
        case OutputType::Earth: return CalculateAttribute(Attribute::EarthResistance);
        case OutputType::Air: return CalculateAttribute(Attribute::AirResistance);
        case OutputType::Fire: return CalculateAttribute(Attribute::FireResistance);
        case OutputType::Frost: return CalculateAttribute(Attribute::FrostResistance);
        case OutputType::Shadow: return CalculateAttribute(Attribute::ShadowResistance);
        case OutputType::Holy: return CalculateAttribute(Attribute::HolyResistance);
        case OutputType::Nature: return CalculateAttribute(Attribute::NatureResistance);
        case OutputType::Life: return CalculateAttribute(Attribute::LifeResistance);
        default: return 0;
        }
    }

    bool CharacterSheet::EquipItem(const EquipmentEntry& equipment, int durability, game::Inventory& inv, 
            bool slot1)
    {
        // first dequip any possible item in the target slot
        UnequipItem(equipment.slot, inv, slot1);
        // find the matching equipment in inventory and remove it
        bool ok = inv.RemoveItem(equipment.name, 1, durability);
        if(!ok) // didn't locate item in inventory
            return false;
        // next determine which slot to place equipment in
        Equipment*& equipmentSlot = weaponRight_; 
        switch(equipment.slot)
        {
            case EquipmentEntry::Slot::Head: equipmentSlot = headSlot_; break;
            case EquipmentEntry::Slot::Neck: equipmentSlot = neckSlot_; break;
            case EquipmentEntry::Slot::Shoulders: equipmentSlot = shoulderSlot_; break;
            case EquipmentEntry::Slot::Chest: equipmentSlot = chestSlot_; break;
            case EquipmentEntry::Slot::Hands: equipmentSlot = handsSlot_; break;
            case EquipmentEntry::Slot::Belt: equipmentSlot = beltSlot_; break;
            case EquipmentEntry::Slot::Leggings: equipmentSlot = leggingsSlot_; break;
            case EquipmentEntry::Slot::Shoes: equipmentSlot = shoesSlot_; break;
            case EquipmentEntry::Slot::Ring:
                equipmentSlot = !slot1 ? ring0Slot_ : ring1Slot_;
                break;
            case EquipmentEntry::Slot::Trinket:
                equipmentSlot = !slot1 ? trinket0Slot_ : trinket1Slot_;
                break;
            case EquipmentEntry::Slot::Weapon:
                equipmentSlot = !slot1? weaponRight_ : weaponLeft_;
                break;
        }
        equipmentSlot = new Equipment(equipment, durability);
        cachedAttributes_.clear();
        return true;
    }

    bool CharacterSheet::UnequipItem(EquipmentEntry::Slot slot, game::Inventory& inv, bool slot1)
    {
        Equipment*& equipmentSlot = weaponRight_; 
        switch(slot)
        {
            case EquipmentEntry::Slot::Head: equipmentSlot = headSlot_; break;
            case EquipmentEntry::Slot::Neck: equipmentSlot = neckSlot_; break;
            case EquipmentEntry::Slot::Shoulders: equipmentSlot = shoulderSlot_; break;
            case EquipmentEntry::Slot::Chest: equipmentSlot = chestSlot_; break;
            case EquipmentEntry::Slot::Hands: equipmentSlot = handsSlot_; break;
            case EquipmentEntry::Slot::Belt: equipmentSlot = beltSlot_; break;
            case EquipmentEntry::Slot::Leggings: equipmentSlot = leggingsSlot_; break;
            case EquipmentEntry::Slot::Shoes: equipmentSlot = shoesSlot_; break;
            case EquipmentEntry::Slot::Ring:
                equipmentSlot = !slot1 ? ring0Slot_ : ring1Slot_;
                break;
            case EquipmentEntry::Slot::Trinket:
                equipmentSlot = !slot1 ? trinket0Slot_ : trinket1Slot_;
                break;
            case EquipmentEntry::Slot::Weapon:
                equipmentSlot = !slot1? weaponRight_ : weaponLeft_;
                break;
        }

        if(equipmentSlot == nullptr)
        {
            // nothing to dequip
            return false;
        }

        // store the item as inventory item with durability preserved
        inv.AddItem(equipmentSlot->GetEntry().name, 1, equipmentSlot->GetRemainingDurability());
        // delete and set to nullptr
        delete equipmentSlot;
        equipmentSlot = nullptr;
        cachedAttributes_.clear();
        return true;
    }

    void CharacterSheet::SetLevel(int level)
    {
        level_ = level;
        float baseStat = 5.0f;
        // first calculate strength
        float m = pow(combatClassEntry_.strength, (float)(level_ -1));
        strength_ = (int)(baseStat * m);
        // agility
        m = pow(combatClassEntry_.agility, (float)(level_ - 1));
        agility_ = (int)(baseStat * m);
        // intellect
        m = pow(combatClassEntry_.intellect, (float)(level_ - 1));
        intellect_ = (int)(baseStat * m);
        // dexterity
        m = pow(combatClassEntry_.dexterity, (float)(level_ - 1));
        dexterity_ = (int)(baseStat * m);
        // wisdom
        m = pow(combatClassEntry_.wisdom, (float)(level_ - 1));
        wisdom_ = (int)(baseStat * m);
        // knowledge
        m = pow(combatClassEntry_.knowledge, (float)(level_ - 1));
        knowledge_ = (int)(baseStat * m);
        // vitality
        m = pow(combatClassEntry_.vitality, (float)(level_ - 1));
        vitality_ = (int)(baseStat * m);
        // clear cache
        cachedAttributes_.clear();
    }

}
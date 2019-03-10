// AnimationSystem.cpp
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

#include "AnimationSystem.hpp"
#include "AnimationTable.hpp"
#include "engine/GameEngine.hpp"

namespace combat
{
    AnimationSystem::AnimationSystem()
    {

    }

    AnimationSystem::~AnimationSystem()
    {
        for(auto& eachAnim : animations_)
        {
            delete eachAnim;
        }
    }

    void AnimationSystem::AddAndStartNewAnimation(const std::string& animName, const CombatUnit& src, 
            const CombatUnit& tgt, int srcRadius)
    {
        const auto& animationTable = AnimationTable::Get().GetEntries();
        const auto& itEntry = animationTable.find(animName);
        if(itEntry == animationTable.end())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Unable to find animation `%s'", __FUNCTION__, animName.c_str());
            return;
        }
        animations_.push_back(new Animation(*(itEntry->second), src, tgt, srcRadius));
    }

    void AnimationSystem::InvalidateAnimationSource(const CombatUnit& src)
    {
        for(auto& eachAnim : animations_)
        {
            eachAnim->InvalidateSource(src);
        }
    }
    
    void AnimationSystem::InvalidateAnimationTarget(const CombatUnit& tgt)
    {
        for(auto& eachAnim : animations_)
        {
            eachAnim->InvalidateTarget(tgt);
        }
    }

    void AnimationSystem::Update(float dtime)
    {
        // remove one finished animation at a time
        std::vector<Animation*>::iterator toRemove = animations_.end();
        for(auto it=animations_.begin(); it != animations_.end(); ++it)
        {
            (*it)->Update(dtime);
            if((*it)->IsFinished())
                toRemove = it; // doesn't matter if it gets overwritten
        }
        if(toRemove != animations_.end())
        {
            delete *toRemove;
            animations_.erase(toRemove);
        }
    }

    void AnimationSystem::Render(int camX, int camY, ogl::Program& program)
    {
        // the render loops gives us another pass to remove one entry
        std::vector<Animation*>::iterator toRemove = animations_.end();
        for(auto it=animations_.begin(); it != animations_.end(); ++it)
        {
            // it won't get rendered if its time is up anyway
            (*it)->Render(camX, camY, program);
            if((*it)->IsFinished())
                toRemove = it;
        }
        if(toRemove != animations_.end())
        {
            delete *toRemove;
            animations_.erase(toRemove);
        }
    }
}
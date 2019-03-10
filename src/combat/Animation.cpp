// Animation.cpp
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

#include "Animation.hpp"
#include "engine/GameEngine.hpp"

namespace combat
{

    Animation::Animation(const AnimationEntry& entry, const CombatUnit& source, const CombatUnit& target,
                         int sourceRadius)
        : entry_(entry), source_(source), target_(target), sourceRadius_(sourceRadius)
    {
        // get location of a combat unit should return the center of the unit. If not that needs to be
        //  fixed.
        glm::vec2 origin = source_.GetLocation();
        projectileOriginX_ = (int)origin.x;
        projectileOriginY_ = (int)origin.y;
    }

    Animation::~Animation()
    {

    }
    
    void Animation::Update(float dtime)
    {
        timeElapsed_ += dtime;    
    }
    
    void Animation::Render(int cameraX, int cameraY, ogl::Program& program)
    {
        int x = 0, y = 0;

        // there's nothing to do if animation has run its course (saves a lot of reference validation checks)
        // unless it's a projectile still needing to reach its target destination.
        if(timeElapsed_ >= entry_.GetDuration() && entry_.GetType() != AnimationEntry::Type::Projectile)
            return;

        // calculate x and y based on the type of entry
        switch(entry_.GetType())
        {
        case AnimationEntry::Type::AtTarget: 
            // this mode requires both units to be valid
            if(sourceInvalidated_ || targetInvalidated_)
                return; {
            glm::vec2 center = source_.GetLocation();
            glm::vec2 direction = target_.GetLocation() - center;
            direction = glm::normalize(direction);
            direction = direction * (float)sourceRadius_ / 2.f;
            x = (int)direction.x + (int)center.x; 
            y = (int)direction.y + (int)center.y; }
            break;
        case AnimationEntry::Type::OnSelf:
            // this mode requires just the source to remain valid
            if(sourceInvalidated_)
                return; {
            glm::vec2 center = source_.GetLocation();
            x = (int)center.x;  y = (int)center.y; }
            break;
        case AnimationEntry::Type::OnTarget:
            // this mode requires just the target to remain valid
            if(targetInvalidated_)
                return; {
            glm::vec2 center = target_.GetLocation(); 
            x = (int)center.x;  y = (int)center.y; }
            break;
        case AnimationEntry::Type::Projectile:
            // this mode requires just the target to remain valid
            if(targetInvalidated_)
                return; {
            glm::vec2 center = target_.GetLocation();
            glm::vec2 dir(center.x - (float)projectileOriginX_, center.y - (float)projectileOriginY_);
            dir = glm::normalize(dir) * PROJECTILE_SPEED;
            dir *= timeElapsed_;
            x = (int)(dir.x);
            y = (int)(dir.y);
            // did the projectile reach within a 10 radius of target?
            if(glm::distance(center,dir) <= 10.0f)
                projectileReachedDestination_ = true; }
            break;
        }
        entry_.Render(cameraX, cameraY, program, timeElapsed_, x, y);
    }

    bool Animation::IsFinished() const
    {
        if(entry_.GetType() != AnimationEntry::Type::Projectile)
            return entry_.GetDuration() - timeElapsed_ <= 0.f;
        else
            return projectileReachedDestination_;
    }

    void Animation::PlaySound() const
    {
        if(entry_.GetSound() != "")
            engine::GameEngine::Get().GetSoundManager().PlaySound(entry_.GetSound());
    }

    void Animation::InvalidateSource(const CombatUnit& src)
    {
        if(&source_ == &src)
            sourceInvalidated_ = true;
    }

    void Animation::InvalidateTarget(const CombatUnit& tgt)
    {
        if(&target_ == &tgt)
            targetInvalidated_ = true;
    }

}
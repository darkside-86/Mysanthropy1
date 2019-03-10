// Animation.hpp
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

#include "AnimationEntry.hpp"
#include "CombatUnit.hpp"
#include "ogl/Program.hpp"

namespace combat
{
    // Represents an animation entry state, such as where to draw, what frame to use.
    class Animation
    {
    public:
        static constexpr float PROJECTILE_SPEED = 150.0f;

        // Constructor
        Animation(const AnimationEntry& entry, const CombatUnit& source, const CombatUnit& target,
                  int sourceRadius = 32);
        // Destructor
        virtual ~Animation();
        // Updates the elapsed time determining the animation frame as well as location for the entry
        void Update(float dtime);
        // Renders the animation entry
        void Render(int cameraX, int cameraY, ogl::Program& program);
        // check if animation is finished running
        bool IsFinished() const;
        // play sound associated with if any
        void PlaySound() const;
        // get source
        inline const CombatUnit& GetSource() const { return source_; }
        // get target 
        inline const CombatUnit& GetTarget() const { return target_; }
        // invalidate source IFF parameter src is the stored source
        void InvalidateSource(const CombatUnit& src);
        // invalidate target IFF parameter tgt is the stored target
        void InvalidateTarget(const CombatUnit& tgt);
        // check validation of source TODO: depend on type
        inline bool SourceIsInvalid() const { return sourceInvalidated_; }
        // check validation of target TODO: depend on type
        inline bool TargetIsInvalid() const { return targetInvalidated_; }
    private:
        // the amount of time elapsed since the animation started in seconds
        float timeElapsed_ = 0.f;
        // the animation entry to be rendered
        const AnimationEntry& entry_;
        // radius information of source
        int sourceRadius_;
        // origin of projectile
        int projectileOriginX_ = 0, projectileOriginY_ = 0;
        // the center of the animation. For projectiles, the origin.
        const CombatUnit& source_;
        // determines where AtTarget, OnTarget, or Projectile are located
        const CombatUnit& target_;
        // since  C++ doesn't have null references...
        bool sourceInvalidated_ = false;
        bool targetInvalidated_ = false;
        // projectile reached its target so we're done
        bool projectileReachedDestination_ = false;
    };
}

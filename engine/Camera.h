// Camera.h 
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

#include <glm/glm.hpp>

namespace engine
{

	class Camera
	{
	public:
		Camera();
		virtual ~Camera();
		glm::mat4 CalculateView();

		void MoveSide(float amount);
		void MoveFront(float amount);
		void MoveUp(float amount);

		inline glm::vec3 GetPosition() { return position_; }
		inline void SetPosition(const glm::vec3& pos) { position_ = pos; }
		inline glm::vec3 GetDirection() { return direction_; }
		inline void SetDirection(const glm::vec3& dir) { direction_ = dir; }
		inline glm::vec3 GetUp() { return up_; }
		inline void SetUp(const glm::vec3& up) { up_ = up; }
		inline float GetPitch() { return pitch_; }
		inline void SetPitch(float pitch) { pitch_ = pitch; }
		inline float GetYaw() { return yaw_; }
		inline void SetYaw(float yaw) { yaw_ = yaw; }
	private:
		glm::vec3 position_;
		glm::vec3 direction_;
		glm::vec3 up_;
		float pitch_;
		float yaw_;
	};
}
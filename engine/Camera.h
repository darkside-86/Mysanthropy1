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

		~Camera();

		glm::mat4 GetViewMatrix();
		void MoveForward(float n);
		void MoveRight(float n);
		void MoveUp(float n);

		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);

		inline float GetPitch() const { return pitch_; }
		inline void SetPitch(float n) { pitch_ = n; }
		inline float GetYaw() const { return yaw_; }
		inline void SetYaw(float n) { yaw_ = n; }

	private:

		float pitch_ = 0.0f; // rotation around x
		float yaw_ = 0.0f; // rotation around y
		glm::vec3 direction_ = glm::vec3(0.f, 0.f, 1.f);
		glm::vec3 up_ = glm::vec3(0.f, 1.f, 0.f);
	};
}
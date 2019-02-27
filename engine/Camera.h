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
	// Defines a powerful 3D camera in which the x,y,z axis can be rotated.
	class Camera
	{
	public:
		// constructor
		Camera();
		// destructor
		virtual ~Camera();
		// Returns the view matrix for use in the model-view-projection calculations
		glm::mat4 CalculateView();
		// Move along a possibly rotated X axis
		void MoveSide(float amount);
		// Move along a possibly rotated Z axis
		void MoveFront(float amount);
		// Move along a possibly rotated Y axis
		void MoveUp(float amount);
		// Gets the position of the camera in space
		inline glm::vec3 GetPosition() const { return position_; }
		// Sets the position of the camera in space
		inline void SetPosition(const glm::vec3& pos) { position_ = pos; }
		// Gets the direction the camera is pointed
		inline glm::vec3 GetDirection() const { return direction_; }
		// Sets the direction the camera should point to
		inline void SetDirection(const glm::vec3& dir) { direction_ = dir; }
		// Gets the possibly rotated defined Y axis
		inline glm::vec3 GetUp() const { return up_; }
		// Sets what is defined as "up" usually (0,1,0)
		inline void SetUp(const glm::vec3& up) { up_ = up; }
		// Rotate the "up" vector by specified degrees
		void RotateUp(float angle);
		// Rotate the direction vector by specified degrees
		void RotateDirection(float angle);
		// Rotate the axis orthogonal to the direction vector
		void RotateUpSide(float angle);
	private:
		// Rotates a vector v around k by angle degrees
		glm::vec3 RotateVector(const glm::vec3& v, const glm::vec3& k, float angle);
		// Position of the camera in space
		glm::vec3 position_;
		// Direction the camera is looking
		glm::vec3 direction_;
		// The movable "Y" axis defining the orientation of the camera
		glm::vec3 up_;
	};
}
// Camera.cpp 
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

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine
{

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		glm::mat4 view(1.0f);
		// calculate pitch and yaw
		direction_.x = glm::cos(glm::radians(pitch_)) * glm::cos(glm::radians(yaw_));
		direction_.y = glm::sin(glm::radians(pitch_));
		direction_.z = glm::cos(glm::radians(pitch_)) * glm::sin(glm::radians(yaw_));

		view = glm::lookAt(position, position + direction_, up_);

		return view;
	}

	void Camera::MoveForward(float n)
	{
		position += n * direction_;
	}

	void Camera::MoveRight(float n)
	{
		position += glm::normalize(glm::cross(direction_, up_)) * n;
	}

	void Camera::MoveUp(float n)
	{
		position += (n * up_);
	}

}
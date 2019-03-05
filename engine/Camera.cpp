// Camera.cpp 
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

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

namespace engine
{

	Camera::Camera() : position_({0.f,0.f,0.f}), direction_({0.f,0.f,-1.f}), up_({0.f,1.0f,0.f})
	{
	}

	Camera::~Camera()
	{
	}

	glm::mat4 Camera::CalculateView()
	{
		direction_ = glm::normalize(direction_);
		glm::mat4 view = glm::lookAt(position_, position_+direction_, up_);
		return view;
	}

	void Camera::MoveSide(float amount)
	{
		position_ += glm::normalize(glm::cross(direction_, up_)) * amount;
	}

	void Camera::MoveFront(float amount)
	{
		position_ += glm::normalize(direction_) * amount;
	}

	void Camera::MoveUp(float amount)
	{
		position_ += glm::normalize(up_) * amount;
	}

	void Camera::RotateUp(float angle)
	{
		up_ = RotateVector(up_, direction_, angle);
	}

	void Camera::RotateDirection(float angle)
	{
		direction_ = RotateVector(direction_, up_, angle);
	}

	void Camera::RotateUpSide(float angle)
	{
		glm::vec3 k = glm::cross(up_, direction_);
		up_ = RotateVector(up_, k, angle );
		direction_ = RotateVector(direction_, k, angle);

	}

	glm::vec3 Camera::RotateVector(const glm::vec3& v, const glm::vec3& k, float angle)
	{
		angle = glm::radians(angle);
		glm::vec3 rot = v * cos(angle) + (glm::cross(k,v) * sin(angle)) 
                        + k * glm::dot(k,v) * (1.0f - cos(angle));
		return rot;
	}
}
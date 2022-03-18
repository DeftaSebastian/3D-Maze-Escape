#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace bullet
{
	class Bullet
	{
	public:

		Bullet()
		{
			position = glm::vec3(0, 2, 5);
			forward = glm::vec3(0, 0, -1);
			up = glm::vec3(0, 1, 0);
			right = glm::vec3(1, 0, 0);
			distanceToTarget = 3.5;
		}

		Bullet(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
		{
			Set(position, center, up);
		}

		~Bullet()
		{ }

		void Set(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up)
		{
			this->position = position;
			this->forward = forward;
			this->right = glm::cross(forward, up);
			this->up = glm::cross(right, forward);
		}

		void TranslateForward(float distance)
		{
			// TODO(student): Translate the camera using the `forward` vector.
			// What's the difference between `TranslateForward()` and
			// `MoveForward()`?
			position += glm::normalize(forward) * distance;

		}

	public:
		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};
}
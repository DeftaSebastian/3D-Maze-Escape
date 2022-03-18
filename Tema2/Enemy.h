#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace enemy
{
	class Enemy
	{
	public:
		Enemy()
		{
			this->translateX = 0;
			this->translateY = 0;
			this->translateZ = 0;
			modelMatrix = glm::mat4(1);
			this->alive = true;

		}
		Enemy(const float spawnX, const float spawnY, const float spawnZ)
		{
			this->spawnX = spawnX;
			this->spawnY = spawnY;
			this->spawnZ = spawnZ;
			this->translateX = 0;
			this->translateY = 0;
			this->translateZ = 0;
			modelMatrix = glm::mat4(1);
			this->alive = true;
			this->move = true;
		}
		~Enemy()
		{ }
	public:
		float spawnX;
		float spawnY;
		float spawnZ;
		float translateX;
		float translateY;
		float translateZ;
		glm::mat4 modelMatrix;
		bool alive;
		bool move;
		bool doubleHp;
		int length;
		int heigth;
	};
}

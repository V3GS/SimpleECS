#pragma once
#include <set>
#include "../Entity/Entity.h"

class System
{
	public:
		std::set<Entity> m_Entities;
		virtual void Init() = 0;
		virtual void Update(float deltaTime) = 0;
};
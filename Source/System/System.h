#pragma once
#include <set>
#include "../Entity/Entity.h"

class World;

class System
{
	protected:
		World* m_World;
	public:
		std::set<Entity> m_Entities;
		void Init(World* world)
		{
			m_World = world;
		}

		virtual void Update(float deltaTime) = 0;
};
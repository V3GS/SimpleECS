#pragma once
#include <set>
#include "../Entity/Entity.h"

class System
{
	public:
		std::set<Entity> m_Entities;
};
#pragma once

class IComponentArray
{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
		virtual void InsertDefaultData(Entity entity) = 0;
};
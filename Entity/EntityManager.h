#pragma once
#include <list>
#include <queue>
#include <array>
#include <cassert>

#include "Entity.h"

class EntityManager
{
	private:
		std::list<Entity> m_Entities;
		Entity m_AvailableId;
		std::queue<Entity> m_AvailableEntities;

		// Array of signatures where the index corresponds to the entity ID
		std::array<ComponentMask, MAX_ENTITIES> m_ComponentMasks{};
	public:
		Entity Create()
		{
			if (m_Entities.size() < MAX_ENTITIES)
			{
				Entity entity;

				// If there are not a set of Entities to be reused
				if (m_AvailableEntities.size() == 0)
				{
					entity = m_AvailableId;
					m_AvailableId++;
				}
				// If it can be reused Entities, then retreive the from the queue
				else
				{
					entity = m_AvailableEntities.front();
					m_AvailableEntities.pop();
				}

				m_Entities.push_back(entity);

				return entity;
			}
		}

		void Destroy(Entity entity)
		{
			m_Entities.remove(entity);
			m_AvailableEntities.push(entity);
		}

		void PrintEntities()
		{
			std::cout << "Entities: ";
			for (Entity currentEntity: m_Entities) {
				std::cout << currentEntity << " ";
			}
			std::cout << std::endl;

			std::cout << "Available Entities: ";
			std::queue<Entity> tmpQueue = m_AvailableEntities;

			while (!tmpQueue.empty()) {
				Entity availableEntity = tmpQueue.front();
				tmpQueue.pop();

				std::cout << availableEntity << " ";
			}
			
			std::cout << std::endl << std::string(60, '-') << std::endl;
		}

		std::list<Entity> GetEntities()
		{
			return m_Entities;
		}

		void SetComponentMask(Entity entity, ComponentMask componentMask)
		{
			m_ComponentMasks[entity] = componentMask;
		}

		ComponentMask GetComponentMask(Entity entity)
		{
			// Get this entity's signature from the array
			return m_ComponentMasks[entity];
		}
};
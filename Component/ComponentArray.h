#pragma once

template<typename ComponentType>
class ComponentArray : public IComponentArray
{
	private:
		std::array<ComponentType, MAX_ENTITIES> m_ComponentArray{};
		std::unordered_map<Entity, size_t> m_EntityToIndexMap{};
		std::unordered_map<size_t, Entity> m_IndexToEntityMap{};
		size_t mSize{};

	public:
		void InsertData(Entity entity, ComponentType component)
		{
			// Put new entry at end
			size_t newIndex = mSize;
			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;
			m_ComponentArray[newIndex] = component;
			++mSize;
		}

		void RemoveData(Entity entity)
		{
			assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
			size_t indexOfLastElement = mSize - 1;
			m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
			m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(indexOfLastElement);

			--mSize;
		}

		ComponentType& GetData(Entity entity)
		{
			assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Retrieving non-existent component.");

			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity) override
		{
			if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
			{
				RemoveData(entity);
			}
		}
};
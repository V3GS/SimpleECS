#pragma once
#include <iostream>
#include <iomanip>
#include <memory>
#include "Entity/EntityManager.h"
#include "Component/ComponentManager.h"

class World
{
	private:
		std::unique_ptr<EntityManager> m_EntityManager;
		std::unique_ptr<ComponentManager> m_ComponentManager;

	public:
		void Init() {
			m_EntityManager = std::make_unique<EntityManager>();
			m_ComponentManager = std::make_unique<ComponentManager>();
		}

		// Entities
		Entity CreateEntity()
		{
			return m_EntityManager->Create();
		}

		void DestroyEntity(Entity entity)
		{
			m_EntityManager->Destroy(entity);
		}

		// Components
		template<typename ComponentType>
		void RegisterComponent()
		{
			m_ComponentManager->Register<ComponentType>();
		}

		template<typename ComponentType>
		void AddComponent(Entity entity, ComponentType component)
		{
			// TODO: If the component isn't registered yet, create a new ComponentArray<ComponentType>.
			// This prevents failures when a component is not explicitly registered.

			m_ComponentManager->AddComponent<ComponentType>(entity, component);

			// Each time a component is added, the 'ComponentMask' bitset must be updated.
			// The mask will help to obtain the Entities that belong to an specific 'System'
			ComponentMask componentMask = m_EntityManager->GetComponentMask(entity);
			ComponentInfo componentInfo = m_ComponentManager->GetComponentInfo<ComponentType>();
			// The biset is based on the component's id
			componentMask.set(componentInfo.id, true);
			// Register the component mask in the Entity manager
			m_EntityManager->SetComponentMask(entity, componentMask);

			// TODO: Register the mask when adding a component and have reference in the sytem manager
			//mSystemManager->EntitySignatureChanged(entity, signature);
		}

		void PrintEntities()
		{
			m_EntityManager->PrintEntities();
		}

		void PrintComponents()
		{
			m_ComponentManager->PrintComponents();
		}

		void PrintEntitiesComponentsRelationship()
		{
			std::list<Entity> entities = m_EntityManager->GetEntities();
			std::list<ComponentInfo> components = m_ComponentManager->GetRegisteredComponents();

			std::cout << std::left;
			std::cout << "================ Entities - Components table ===============" << std::endl;
			std::cout << std::setw(15) << "Entity";
			for (ComponentInfo currentComponentInfo : components)
			{
				std::cout << std::setw(15) << currentComponentInfo.name;
			}

			std::cout << std::endl << std::string( (components.size() + 1) * 15 , '-');

			for (Entity currentEntity : entities)
			{
				std::cout << std::endl << std::setw(15) << currentEntity << "\t";
				for (ComponentInfo currentComponentInfo : components)
				{
					ComponentMask mask = m_EntityManager->GetComponentMask(currentEntity);

					if (mask.test(currentComponentInfo.id))
					{
						std::cout << std::setw(15) << "X";
					}
					else
					{
						std::cout << std::setw(15) << " ";
					}
				}
			}

			std::cout << std::endl << std::string((components.size() + 1) * 15, '-') << std::endl;
		}
};
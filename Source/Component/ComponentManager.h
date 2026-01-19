#pragma once
#include <list>
#include <unordered_map>
#include "IComponentArray.h"
#include "ComponentArray.h"

class ComponentManager
{
	private:
		// List that contains a description of components available
		std::list<ComponentInfo> m_Components;
		std::unordered_map<std::string, std::shared_ptr<IComponentArray>> m_ComponentArrays{};

		template<typename ComponentType>
		std::shared_ptr<ComponentArray<ComponentType>> GetComponentArray()
		{
			ComponentInfo componentInfo = GetComponentInfo<ComponentType>();
			// Return the 'ComponentArray' by using the component's name
			return std::static_pointer_cast<ComponentArray<ComponentType>>(m_ComponentArrays[componentInfo.name]);
		}
	public:
		template<typename ComponentType>
		ComponentInfo GetComponentInfo()
		{
			ComponentInfo componentInfo
			{
				.id = GetComponentId<ComponentType>(),
				.name = GetComponentName<ComponentType>(),
			};

			return componentInfo;
		}

		template<typename ComponentType>
		void RegisterComponent()
		{
			// Create a new component info that will contain the id and name of the Component
			ComponentInfo componentInfo = GetComponentInfo<ComponentType>();
			// Add the component to a collection that maintains a set of available components
			m_Components.push_back(componentInfo);

			// When registering, a container for "ComponentType" is created and memory its respective memory is reserved
			m_ComponentArrays.insert({
				componentInfo.name,
				std::make_shared<ComponentArray<ComponentType>>()
			});
		}

		template<typename ComponentType>
		void AddComponent(Entity entity, ComponentType component)
		{
			// Oncre it's retrieved the 'ComponentArray', then insert the component into the map
			GetComponentArray<ComponentType>()->InsertData(entity, component);
		}

		template<typename ComponentType>
		ComponentType& GetComponent(Entity entity)
		{
			return GetComponentArray<ComponentType>()->GetData(entity);
		}

		std::list<ComponentInfo> GetRegisteredComponents()
		{
			return m_Components;
		}
    
        template<typename ComponentType>
        void RemoveComponent(Entity entity)
        {
            GetComponentArray<ComponentType>()->RemoveData(entity);
        }
    
        void EntityDestroyed(Entity entity)
        {
            for (auto const& pair : m_ComponentArrays)
            {
                auto const& component = pair.second;
                component->EntityDestroyed(entity);
            }
        }

		void PrintComponents()
		{
			std::cout << "Registered Components" << std::endl;
			for (ComponentInfo currentComponent : m_Components) {
				std::cout << "{" << currentComponent.id << "} = " << currentComponent.name << std::endl;
			}
			std::cout << std::string(60, '-') << std::endl;
		}
};

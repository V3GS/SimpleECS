#pragma once
#include <list>
#include <unordered_map>
#include "IComponentArray.h"
#include "ComponentArray.h"
#include <functional>
#include <any>
#include "Component.h"

class ComponentManager
{
	private:
		// List that contains a description of components available
		std::list<ComponentInfo> m_Components;
		std::unordered_map<std::string, std::shared_ptr<IComponentArray>> m_ComponentArrays{};
		// Container that helps to instantiate a component in Runtime
		std::unordered_map<std::string, std::function<std::any()>> m_ComponentsRegistry;

		template<typename ComponentType>
		std::shared_ptr<ComponentArray<ComponentType>> GetComponentArray()
		{
			ComponentInfo componentInfo = GetComponentInfo<ComponentType>();
			// Return the 'ComponentArray' by using the component's name
			return std::static_pointer_cast<ComponentArray<ComponentType>>(m_ComponentArrays[componentInfo.name]);
		}

		std::shared_ptr<IComponentArray> GetComponentArray(const std::string& componentName)
		{
			ComponentInfo componentInfo = GetComponentInfo(componentName);
			// Return the 'ComponentArray' by using the component's name
			return m_ComponentArrays[componentName];
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

		ComponentInfo GetComponentInfo(const std::string componentName)
		{
			// Check if the componentName is registered in the m_Components list.
			// Since this is retrieve in runtime, it should match a coincidence
			for (ComponentInfo componentInfo : m_Components)
			{
				if (componentInfo.name == componentName)
				{
					return componentInfo;
				}
			}
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

			m_ComponentsRegistry[componentInfo.name] = [] { return std::any{ ComponentType{} }; };
		}

		template<typename ComponentType>
		void AddComponent(Entity entity, ComponentType component)
		{
			// Once it's retrieved the 'ComponentArray', then insert the component into the map
			GetComponentArray<ComponentType>()->InsertData(entity, component);
		}

		void AddComponentByName(Entity entity, const std::string& componentName)
		{
			auto it = m_ComponentsRegistry.find(componentName);

			if (it != m_ComponentsRegistry.end())
			{
				// Get the component array based on the component name
				GetComponentArray(componentName)->InsertDefaultData(entity);
			}
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

#pragma once
#include <unordered_map>
#include <memory>
#include "System.h"

class SystemManager
{
	private:
		std::unordered_map<std::string, ComponentMask> m_ComponentMasks{};
		std::unordered_map<std::string, std::shared_ptr<System>> m_Systems{};

		template<typename SystemType>
		std::string GetSystemName()
		{
			// Strides by 6 to remove the "class " string
			return std::string(typeid(SystemType).name()).substr(6);
		}

	public:
		template<typename SystemType>
		std::shared_ptr<SystemType> RegisterSystem()
		{
			std::string typeName = GetSystemName<SystemType>();

			std::shared_ptr<SystemType> system = std::make_shared<SystemType>();
			
			m_Systems.insert(std::pair{ typeName, system });
			return system;
		}

		template<typename SystemType>
		void SetMask(ComponentMask mask)
		{
			std::string typeName = GetSystemName<SystemType>();

			m_ComponentMasks.insert({ typeName, mask });
		}

		void EntityMaskChanged(Entity entity, ComponentMask entityMask)
		{
			for (auto const& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_ComponentMasks[type];

				if ((entityMask & systemSignature) == systemSignature)
				{
					system->m_Entities.insert(entity);
				}
				else
				{
					system->m_Entities.erase(entity);
				}
			}
		}
};

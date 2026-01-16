#pragma once
#include <cstdint>
#include <bitset>

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
extern int s_ComponentCount = 0;

using ComponentMask = std::bitset<MAX_COMPONENTS>;

template <class ComponentType>
uint16_t GetComponentId()
{
	static uint16_t componentId = s_ComponentCount++;
	return componentId;
}

template <class ComponentType>
std::string GetComponentName()
{
	// Strides by 7 to remove the "struct " string
	return std::string(typeid(ComponentType).name()).substr(7);
}

struct ComponentInfo
{
	uint16_t id;
	std::string name;
};
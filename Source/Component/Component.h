#pragma once
#include <cstdint>
#include <bitset>

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
inline int s_ComponentCount = 0;

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
	#if defined(_WIN32)
		// Strides by 7 to remove the "struct " string
		return std::string(typeid(ComponentType).name()).substr(7);
	#elif defined(__APPLE__) && defined(__MACH__)
		// Strides by 1 to remove the an integer id value in the string
		return std::string(typeid(ComponentType).name()).substr(1);
	#endif
}

struct ComponentInfo
{
	uint16_t id;
	std::string name;
};
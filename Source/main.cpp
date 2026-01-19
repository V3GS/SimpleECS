#include <iostream>
#include <fstream>
#include <string>
#include "World.h"

// This class is in charge of performance operations over the Entity, Component and System managers
World world;

// Test components
struct Position
{
	int x;
	int y;
	int z;

	void Print()
	{
		std::cout << "Position (" << x << ", " << y << ", " << z << ")" << std::endl;
	}
};

struct Color
{
	int r;
	int g;
	int b;
	int a;
};

struct Rotation
{
	float value;
};

/// <summary>
/// This system was made with testing purposes. Works over all the Entities that have a Position component
/// </summary>
class TranslateSystem : public System
{
	public:
		void Init() {}
		void Update(float dt)
		{
			for (auto const& entity : m_Entities)
			{
				auto& position = world.GetComponent<Position>(entity);

				position.x += 1;
				position.y += 2;
				position.z += 3;

				std::cout << "Entity {" << entity << "} - ";
				position.Print();
			}
		}
};

class RotateSystem : public System
{
	public:
		void Init() {}
		void Update(float dt)
		{
			for (auto const& entity : m_Entities)
			{
				auto& rotation = world.GetComponent<Rotation>(entity);
				
				rotation.value += 1.0f;

				std::cout << "Entity {" << entity << "} - Current rotation value = " << rotation.value << std::endl;
			}
		}
};

int main()
{
	// WORLD
	// Initialize the world
	world.Init();

	// ENTITIES OPERATIONS
	// Create 3 entities
	Entity e0 = world.CreateEntity(); // Entity: 0
	Entity e1 = world.CreateEntity(); // Entity: 1
	Entity e2 = world.CreateEntity(); // Entity: 2
	world.PrintEntities();

	// Destroy the entity 1 and 0. Thefore, those entities will be available for posterior use.
	// When creating a new entity, it will grab from a list in the order they were destroyed
	// In this case, the available entities are {1, 0}
	world.DestroyEntity(e1);
	world.DestroyEntity(e0);
	world.PrintEntities();

	// Reuses the entities deleted in order. Thus, it will grab the Entity 1
	// the available entities are {0}
	Entity e3 = world.CreateEntity();
	// Print the use and available entities IDs
	world.PrintEntities();

	// COMPONENT OPERATIONS
	// Register the components. This will also reserve memory of ComponentArray<ComponentType> for posterior use
	world.RegisterComponent<Position>();
	world.RegisterComponent<Color>();
	world.RegisterComponent<Rotation>();
	// Print the IDs and names of the components
	world.PrintComponents();

	// SYSTEM OPERATIONS
	// Register a system
	auto translateSystem = world.RegisterSystem<TranslateSystem>();
	{
		// Obtain the component information (id, name)
		ComponentInfo componentInfo = world.GetComponentInfo<Position>();
		
		// Create a mask indicating a set of components
		ComponentMask mask;
		mask.set( componentInfo.id );

		// Define the components the system will operate on
		world.SetSystemMask<TranslateSystem>(mask);
	}

	auto rotationSystem = world.RegisterSystem<RotateSystem>();
	{
		// Obtain the component information (id, name)
		ComponentInfo componentInfo = world.GetComponentInfo<Rotation>();
		
		// Create a mask indicating a set of components
		ComponentMask mask;
		mask.set( componentInfo.id );

		// Define the components the system will operate on
		world.SetSystemMask<RotateSystem>(mask);
	}

	translateSystem->Init();
	rotationSystem->Init();
	
	// COMPONENT DATA
	// Add component data to the Entity
	world.AddComponent<Position>(e3, Position{
			.x = 0,
			.y = 0,
			.z = 0
		});
	world.AddComponent<Color>(e3, Color{});
	world.AddComponent<Rotation>(e3, Rotation{
		.value = -45
	});

	world.AddComponent<Position>(e2, Position{
			.x = 1,
			.y = 2,
			.z = 3
		});
	world.AddComponent<Rotation>(e2, Rotation{
		.value = 90
	});

	// Print in console a table of the relations between the Entities and the Components
	world.PrintEntitiesComponentsRelationship();

	// ENTITIES OPERATIONS
	// Destroy an Entity
	world.DestroyEntity(e2);
	world.PrintEntitiesComponentsRelationship();
	world.PrintEntities();

	// COMPONENTS OPERATIONS
	// Remove component from Entity
	world.RemoveComponent<Color>(e1);
	world.PrintEntitiesComponentsRelationship();

	// Update the test system until 'q' is pressed
	std::string line;

	do
	{
		if (!line.empty() && line[0] == 'q')
			break;

		std::cout << "Updating systems..." << std::endl;

		// The World updates all the Systems and send them the corresponding deltaTime
		world.UpdateSystems(0.0f);

	} while (std::getline(std::cin, line));
}


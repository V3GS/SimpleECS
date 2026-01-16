#include <iostream>
#include "World.h"

struct Position
{
	int x;
	int y;
	int z;

	Position() {}
	Position(int newX, int newY, int newZ) : x(newX), y(newY), z(newZ) {}
};

struct Color
{
	int r;
	int g;
	int b;
	int a;

	Color() {}
	Color(int newR, int newG, int newB, int newA) : r(newR), g(newG), b(newB), a(newA) {}
};

struct Rotate
{
	float value;

	Rotate() {}
	Rotate(float newValue) : value(newValue) {}
};

int main()
{
	// WORLD
	// This class is in charge of performance operations over the Entity, Component and System managers
	World world;
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
	world.RegisterComponent<Rotate>();
	// Print the IDs and names of the components
	world.PrintComponents();

	//
	world.AddComponent<Position>(e3, Position{});
	world.AddComponent<Color>(e3, Color{});
	world.AddComponent<Rotate>(e3, Rotate{});

	world.AddComponent<Position>(e2, Position{});
	world.AddComponent<Rotate>(e2, Rotate{});

	world.PrintEntitiesComponentsRelationship();
}


#include <gtest/gtest.h>
#include <array>
#include <unordered_map>
#include "Entity/Entity.h"
#include "Component/ComponentManager.h"

struct TestComponent
{
    int value = 5;
};

namespace SimpleECS::Tests
{
    class ComponentManagerTest : public ::testing::Test
    {
        protected:
            ComponentManager componentManager;

            void SetUp() override
            {
                // Setup code that runs before each test. It runs before each test
            }

            void TearDown() override
            {
                // Cleanup code that runs after each test. It runs after each test
            }
    };

    // Test: Create a single entity
    TEST_F(ComponentManagerTest, RegisterComponent)
    {
        componentManager.RegisterComponent<TestComponent>();
        std::list<ComponentInfo> registeredComponents = componentManager.GetRegisteredComponents();

        EXPECT_EQ(registeredComponents.size(), 1);
    }

    TEST_F(ComponentManagerTest, AddComponent)
    {
        Entity e0 = 0;

        componentManager.RegisterComponent<TestComponent>();
        componentManager.AddComponent<TestComponent>(e0, TestComponent{ .value = 55 });

        TestComponent retrievedComponent = componentManager.GetComponent<TestComponent>(e0);

        EXPECT_EQ(retrievedComponent.value, 55);
    }

    TEST_F(ComponentManagerTest, AddComponentByName)
    {
        Entity e0 = 0;

        componentManager.RegisterComponent<TestComponent>();
        componentManager.AddComponentByName(e0, "TestComponent");

        TestComponent retrievedComponent = componentManager.GetComponent<TestComponent>(e0);

        EXPECT_EQ(retrievedComponent.value, 5);
    }
};
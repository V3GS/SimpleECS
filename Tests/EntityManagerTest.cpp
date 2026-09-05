#include <gtest/gtest.h>
#include <Component/Component.h>
#include "Entity/EntityManager.h"

namespace SimpleECS::Tests
{
    class EntityManagerTest : public ::testing::Test
    {
        protected:
            EntityManager entityManager;

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
    TEST_F(EntityManagerTest, CreateEntity)
    {
        Entity entity = entityManager.Create();
        EXPECT_EQ(entity, 0);
    }

    // Test: Create multiple entities
    TEST_F(EntityManagerTest, CreateMultipleEntities)
    {
        Entity e0 = entityManager.Create();
        Entity e1 = entityManager.Create();
        Entity e2 = entityManager.Create();

        EXPECT_EQ(e0, 0);
        EXPECT_EQ(e1, 1);
        EXPECT_EQ(e2, 2);
    }

    // Test: Entity list contains created entities
    TEST_F(EntityManagerTest, GetEntitiesReturnsCreatedEntities)
    {
        Entity e0 = entityManager.Create();
        Entity e1 = entityManager.Create();

        std::list<Entity> entities = entityManager.GetEntities();

        EXPECT_EQ(entities.size(), 2);
        // As the Entity 0, 1 should be in the entities list
        EXPECT_TRUE(std::find(entities.begin(), entities.end(), e0) != entities.end());
        EXPECT_TRUE(std::find(entities.begin(), entities.end(), e1) != entities.end());
    }

    // Test: Destroy entity
    TEST_F(EntityManagerTest, DestroyEntity)
    {
        Entity e0 = entityManager.Create();
        Entity e1 = entityManager.Create();

        entityManager.Destroy(e0);

        std::list<Entity> entities = entityManager.GetEntities();
        // The entities list should only contain one element, and must only find the remain entity (1)
        EXPECT_EQ(entities.size(), 1);
        EXPECT_TRUE(std::find(entities.begin(), entities.end(), e1) != entities.end());
        EXPECT_FALSE(std::find(entities.begin(), entities.end(), e0) != entities.end());
    }

    // Test: Reuse destroyed entity
    TEST_F(EntityManagerTest, ReuseDestroyedEntity)
    {
        Entity e0 = entityManager.Create();
        Entity e1 = entityManager.Create();

        entityManager.Destroy(e0);

        Entity e2 = entityManager.Create();
        // When creating a new Entity, it should reuse the destroyed entity ID (0)
        EXPECT_EQ(e2, e0);
    }

    // Test: Component mask operations
    TEST_F(EntityManagerTest, SetAndGetComponentMask)
    {
        Entity e0 = entityManager.Create();

        ComponentMask mask;
        mask.set(0, true);
        mask.set(2, true);

        entityManager.SetComponentMask(e0, mask);

        // Check that the bitmask coincides with the set for the Entity
        ComponentMask retrievedMask = entityManager.GetComponentMask(e0);
        EXPECT_TRUE(retrievedMask.test(0));
        EXPECT_FALSE(retrievedMask.test(1));
        EXPECT_TRUE(retrievedMask.test(2));
    }
};
#pragma once
#include <vector>

#include "ecs.hpp"
#include "components.hpp"

class ECSRegistry
{
    // Callbacks to remove a particular or all entities in the system
    std::vector<ContainerInterface*> registry_list;

public:
    // Manually created list of all components this game has
    ComponentContainer<Health> healths;
    ComponentContainer<HealthFlask> healthFlasks;
    ComponentContainer<Damage> damages;
    ComponentContainer<InvincibilityTimer> invinciblityTimers;
    ComponentContainer<Motion> motions;
    ComponentContainer<Collision> collisions;
    ComponentContainer<DeathTimer> deathTimers;
    ComponentContainer<Player> players;
    ComponentContainer<TransformComponent> transforms;
    ComponentContainer<Sprite> sprites;
    ComponentContainer<std::vector<Sprite>> heartSprites;
    ComponentContainer<RenderRequest> renderRequests; 
    ComponentContainer<ScreenState> screenStates;
    ComponentContainer<Animation<PlayerState>> playerAnimations;
    ComponentContainer<Gravity> gravity;

    // constructor that adds all containers for looping over them
    // IMPORTANT: Don't forget to add any newly added containers!
    ECSRegistry()
    {
        registry_list.push_back(&healths);
        registry_list.push_back(&healthFlasks);
        registry_list.push_back(&damages);
        registry_list.push_back(&invinciblityTimers);
        registry_list.push_back(&deathTimers);
        registry_list.push_back(&motions);
        registry_list.push_back(&collisions);
        registry_list.push_back(&players);
        registry_list.push_back(&transforms);
        registry_list.push_back(&sprites);
        registry_list.push_back(&heartSprites);
        registry_list.push_back(&renderRequests);
        registry_list.push_back(&screenStates);
        registry_list.push_back(&playerAnimations);
        registry_list.push_back(&gravity);
    }

    void clear_all_components() {
        for (ContainerInterface* reg : registry_list)
            reg->clear();
    }

    void list_all_components() {
        printf("Debug info on all registry entries:\n");
        for (ContainerInterface* reg : registry_list)
            if (reg->size() > 0)
                printf("%4d components of type %s\n", (int)reg->size(), typeid(*reg).name());
    }

    void list_all_components_of(Entity e) {
        printf("Debug info on components of entity %u:\n", (unsigned int)e);
        for (ContainerInterface* reg : registry_list)
            if (reg->has(e))
                printf("type %s\n", typeid(*reg).name());
    }

    void remove_all_components_of(Entity e) {
        for (ContainerInterface* reg : registry_list)
            reg->remove(e);
    }
};

extern ECSRegistry registry; // The global ECS registry
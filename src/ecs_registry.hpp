#pragma once
#include <vector>

#include "ecs.hpp"
#include "components.hpp"

class ECSRegistry
{
    // Callbacks to remove a particular or all entities in the system
    std::vector<ContainerInterface*> registry_list;

public:
    template<typename Component>
    ComponentContainer<Component>& component() {
        static ComponentContainer<Component> container;
        static bool registered = false;
        if (!registered) {
            registered = true;
            registry_list.push_back(&container);
        }
        return container;
    }

    // constructor that adds all containers for looping over them
    // IMPORTANT: Don't forget to add any newly added containers!
    ECSRegistry() = default;

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

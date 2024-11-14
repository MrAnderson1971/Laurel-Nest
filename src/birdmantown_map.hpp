#pragma once

#include "ecs.hpp"
#include "region.hpp"

class Birdmantown : public Region {
public:
    Birdmantown();
    ~Birdmantown();

    void init() override;

    Connection SetDoor(float width, float height, float xPos, float yPos, Entity connectNextRoom, vec2 connectNextSpawn, bool isLimited);

private:

};
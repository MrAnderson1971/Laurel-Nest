#include "collision_system.h"
#include "ecs.hpp"


bool static checkForCollision(Entity e1, Entity e2){
    unsigned long size = registry.bounding_box.size();
    BoundingBox b1 = registry.bounding_box.get(e1);
    BoundingBox b2 = registry.bounding_box.get(e2);

    //top left
    if(b1.p1.x <= b2.p1.x && b1.p4.x >= b2.p1.x ){
        if(b1.p1.y >= b2.p1.y && b1.p2.y <= b2.p1.y){
            return true;
        }
    }else if(b1.p1.x <= b2.p4.x && b1.p4.x >= b2.p4.x){ //Top right
        if(b1.p1.y >= b2.p4.y && b1.p2.y <= b2.p4.y){
            return true;
        }
    } else if(b1.p1.x <= b2.p2.x && b1.p4.x >= b2.p2.x) { //Bottom Left
        if(b1.p1.y >= b2.p2.y && b1.p2.y <= b2.p2.y){
            return true;
        }
    }else if(b1.p1.x < b2.p3.x && b1.p4.x > b2.p3.x){ // Bottom Right
        if(b1.p1.y >= b2.p3.y && b1.p2.y <= b2.p3.y){
            return true;
        }
    } else{
        return false;
    }
}


void Collision_System::updateBoundingBox(Entity e1){
    Motion& player_motion = registry.motions.get(e1);
    float box_height = 12 * player_motion.scale.y; // 12 is a placeholder, that value should be the height of the player entity when scale = 1
    float y_value_min = player_motion.position.y - box_height/2;
    float y_value_max = player_motion.position.y + box_height/2;
    float box_weight = 12 * player_motion.scale.x; // 12 is a placeholder, that value should be the width of the player entity when scale = 1
    float x_value_min = player_motion.position.x - box_weight/2;
    float x_value_max = player_motion.position.x + box_weight/2;
    BoundingBox bounding_box = registry.bounding_box.get(e1);

    //Top Left
    bounding_box.p1.x = x_value_min;
    bounding_box.p1.y = y_value_max;

    //Bottom Left
    bounding_box.p2.x = x_value_min;
    bounding_box.p2.y = y_value_min;

    //Bottom Right
    bounding_box.p3.x = x_value_max;
    bounding_box.p3.y = y_value_min;

    //Top Right
    bounding_box.p4.x = x_value_max;
    bounding_box.p4.y = y_value_max;
}




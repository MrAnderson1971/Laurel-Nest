// internal
#include "ai_system.h"

//void AISystem::step(Entity player_entity)
//{
//    unsigned long size = registry.patrol_ais.size();
//
//    for (int i = 0; i < size; i++) {
//        Patrol_AI& patrol_component = registry.patrol_ais.components[i];
//        Entity entity = registry.patrol_ais.entities[i];
//        Motion& motion = registry.motions.get(entity);
//        Motion& motion_player = registry.motions.get(player_entity);
//
//        //motion.position.x += motion.velocity.x;
//
////        if (motion.position.x >= patrol_component.patrolMaxX || motion.position.x < patrol_component.patrolMinX) {
////            patrol_component.direction *= -1;
////            motion.velocity.x = motion.velocity.x * (patrol_component.direction);
////        }
//        if((abs(motion_player.position.x - motion.position.x)) < 400 && (abs(motion_player.position.y - motion.position.y) <= 100)){
//            if(motion_player.position.x < motion.position.x){
//                patrol_component.patrolMinX = 0;
//                motion.velocity.x = -3;
//                patrol_component.chasing = true;
//            }else{
//                patrol_component.patrolMinX = 0;
//                motion.velocity.x = 3;
//                patrol_component.chasing = true;
//            }
//        }else{
//            patrol_component.chasing = false;
//        }
//
////        if (patrol_component.chasing ){
////            if(motion_player.position.x < motion.position.x && patrol_component.movingRight && (abs(motion_player.position.x - motion.position.x) < 500)){
////                patrol_component.movingRight = false;
////                motion.velocity.x = -5;
////            }
////        }
//
//
//    }
//    //(void)elapsed_ms; // placeholder to silence unused warning until implemented
//
//}

void AISystem::step(Entity player_entity)
{
    size_t size = registry.patrol_ais.size();
    const float chaseRange = 400.0f;  // The distance within which Goomba starts chasing
    const float dashDistance = 1000.0f;  // The distance Goomba dashes beyond the chase range

    for (int i = 0; i < size; i++) {
        Patrol_AI& patrol_component = registry.patrol_ais.components[i];
        Entity entity = registry.patrol_ais.entities[i];
        Motion& motion = registry.motions.get(entity);
        Motion& motion_player = registry.motions.get(player_entity);

        // Calculate the distance between the Goomba and the player
        float player_distance_x = abs(motion_player.position.x - motion.position.x);
        float player_distance_y = abs(motion_player.position.y - motion.position.y);

        // Start chasing if the player is within the chase range and Goomba is not already chasing
        if (!patrol_component.chasing && player_distance_x < chaseRange && player_distance_y <= 100) {
            patrol_component.chasing = true;

            // Dash towards the player by setting the velocity
            if (motion_player.position.x < motion.position.x) {
                // Player is to the left, Goomba dashes left
                motion.velocity.x = -5.0f;
            } else {
                // Player is to the right, Goomba dashes right
                motion.velocity.x = 5.0f;
            }

            // Store the initial position where the Goomba started dashing
            patrol_component.dashStartX = motion.position.x;
        }

        // Continue dashing until Goomba has traveled the dash distance
        if (patrol_component.chasing) {
            // Calculate the total distance Goomba has moved from its starting point
            float dash_traveled_distance = abs(motion.position.x - patrol_component.dashStartX);

            // Stop dashing when Goomba has traveled the full dash distance
            if (dash_traveled_distance >= dashDistance || player_distance_x >= chaseRange) {
                patrol_component.chasing = false;

                // Resume patrolling if not chasing
                if (motion.position.x >= patrol_component.patrolMaxX || motion.position.x <= patrol_component.patrolMinX) {
                    patrol_component.direction *= -1;
                }

                // Move according to patrol direction
                motion.velocity.x = patrol_component.direction * 2.0f;  // Patrol speed
            }
        } else {
            // Continue patrolling when not chasing
            if (motion.position.x >= patrol_component.patrolMaxX || motion.position.x <= patrol_component.patrolMinX) {
                patrol_component.direction *= -1;
            }

            // Move according to patrol direction
            motion.velocity.x = patrol_component.direction * 2.0f;  // Patrol speed
        }
    }
}


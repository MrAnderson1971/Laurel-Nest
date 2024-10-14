// internal
#include "ai_system.h"

void AISystem::step(Entity player_entity)
{
    unsigned long size = registry.patrol_ais.size();

    for (int i = 0; i < size; i++) {
        Patrol_AI& patrol_component = registry.patrol_ais.components[i];
        Entity entity = registry.patrol_ais.entities[i];
        Motion& motion = registry.motions.get(entity);
        Motion& motion_player = registry.motions.get(player_entity);

        //motion.position.x += motion.velocity.x;

//        if (motion.position.x >= patrol_component.patrolMaxX || motion.position.x < patrol_component.patrolMinX) {
//            patrol_component.direction *= -1;
//            motion.velocity.x = motion.velocity.x * (patrol_component.direction);
//        }
        if((abs(motion_player.position.x - motion.position.x)) < 300 && (abs(motion_player.position.y - motion.position.y) <= 100) && !patrol_component.chasing){
            if(motion_player.position.x < motion.position.x){
                patrol_component.patrolMinX = 0;
//                if(motion.velocity.x > -3){
//                    motion.velocity.x--;
//                }else{
//                    motion.velocity.x = -3;
//                }
                motion.velocity.x = -3;
                patrol_component.chasing = true;
            }else{
                patrol_component.patrolMinX = 0;
//                if(motion.velocity.x < -3){
//                    motion.velocity.x++;
//                }else{
//                    motion.velocity.x = 3;
//                }
                motion.velocity.x = 3;
                patrol_component.chasing = true;
            }
        }else{
            patrol_component.chasing = false;
        }

//        if (patrol_component.chasing ){
//            if(motion_player.position.x < motion.position.x && patrol_component.movingRight && (abs(motion_player.position.x - motion.position.x) < 500)){
//                patrol_component.movingRight = false;
//                motion.velocity.x = -5;
//            }
//        }


    }
    //(void)elapsed_ms; // placeholder to silence unused warning until implemented

}
using System.Runtime.CompilerServices;
namespace IS
{
    class BossBattle
    {

        static SimpleVector2D Boss_spawn_pos;


        // for smash follow
        static private float smash_timer = 2f;
        static private float smash_timer_set = 2f;
        static private bool smash=false;
        static private float miss_smash_y_pos = -600f;


        // for idle
        static private float idle_timer = 2f;
        static private float idle_timer_set = 2f;

        private enum BossStates : int
        {
            Idle=0,
            Clap=1,
            Smash=2
        }

        static private BossStates current_state = BossStates.Smash;

        static public void Init(){
            CameraScript.CameraTargetZoom(0.5f, 1f);
            Boss_spawn_pos = InternalCalls.GetTransformPosition();
        }

        static public void Update(){
            InternalCalls.SetGravityScale(0f);
            InternalCalls.TransformSetRotation(0, 0);

            // restart level if player lose
            if (PlayerScript.isDead)
            {
                PlayerScript.isDead = false;
                InternalCalls.LoadScene("Assets/Scenes/BossLevel.insight");
            }



            BossFSM();
        }
        
        static public void CleanUp(){

        }




        static private void BossFSM()
        {
            switch (current_state)
            {
                case BossStates.Idle:
                    Rest();
                    break;
                case BossStates.Clap:
                    break;
                case BossStates.Smash:
                    FollowBeforeSmash();
                    break;
            }
        }



        static private void StateChanger()
        {
            float random=InternalCalls.GetRandomFloat();
            if (random <=0.5f)
            {
                ResetPosition();
                current_state = BossStates.Smash;
                return;
            }
            if(random <= 1f)
            {
                ResetPosition();
                current_state = BossStates.Idle;
                return;
            }

        }


        static private void Rest()
        {
            ResetPosition();
            idle_timer-=InternalCalls.GetDeltaTime();
            if(idle_timer<=0)
            {
                idle_timer = idle_timer_set;
                StateChanger();
            }

        }


        static private void Clap()
        {


        }

        static private void ResetPosition()
        {
            InternalCalls.TransformSetPosition(Boss_spawn_pos.x, Boss_spawn_pos.y);
            InternalCalls.RigidBodySetForce(0, 0);
            CameraScript.CameraTargetZoom(0.5f, 1f);
        }

        
        // functions for the battle

        static private void FollowBeforeSmash()
        {

            if (!smash)
            {
                CameraScript.CameraTargetZoom(0.3f, 1f);
                SimpleVector2D pos = InternalCalls.GetTransformPosition();
                if(pos.x != PlayerScript.player_pos.x)
                {
                    pos.x = CameraScript.camera_pos.x;
                    InternalCalls.TransformSetPosition(CameraScript.camera_pos.x, pos.y);
                }

                smash_timer-=InternalCalls.GetDeltaTime();
                if(smash_timer <= 0)
                {
                    smash = true;
                    smash_timer = smash_timer_set;
                }

            }
            else
            {
                Smash();
            }



        }



        static private void Smash()
        {
            // going down
            InternalCalls.RigidBodyAddForce(0, -200);



            // check for collide
            int boss = InternalCalls.GetCurrentEntityID();
            SimpleArray collided_objects = InternalCalls.GetCollidingEntityArray(boss);
            for (int i = 0; i < collided_objects.length; i++)
            {
                int entity = collided_objects.GetValue(i);
                if(entity==PlayerScript.PLAYER_ID)
                {
                    PlayerScript.Health -= 1;
                    smash_timer = smash_timer_set;
                    StateChanger();
                    return;
                }


                if (InternalCalls.RigidBodyGetBodyTypeEntity(entity) == 0)
                {
                    DoFloorBreak(entity);
                    smash = false;
                    smash_timer = smash_timer_set;
                    StateChanger();

                }
            }

            // if the boss miss
            if(InternalCalls.GetTransformPosition().y < miss_smash_y_pos)
            {
                smash = false;
                smash_timer = smash_timer_set;
                StateChanger();
            }


        }


        static private void DoFloorBreak(int entity)
        {
            CameraScript.CameraShake(2f);
            CameraScript.camera_shake_intensity = 3f;
            CameraScript.camera_shake_duration = 0.2f;
            SimpleVector2D pos=InternalCalls.GetTransformPosition();
            SimpleVector2D entity_pos = InternalCalls.GetTransformPositionEntity(entity);
            SimpleVector2D size= InternalCalls.GetTransformScaling();
            SimpleVector2D entity_size= InternalCalls.GetTransformScalingEntity(entity);

            //if the boss is bigger
            if (size.x>entity_size.x)
            {
                InternalCalls.RigidBodySetBodyTypeEntity(1, entity);

                return;
            }

            // local var
            float entity_rightmost = entity_pos.x + entity_size.x / 2f;
            float entity_leftmost = entity_pos.x - entity_size.x / 2f;
            float boss_right = pos.x + size.x / 2f;
            float boss_left = pos.x - size.x / 2f;

            if (entity_rightmost > boss_right && entity_leftmost < boss_left)
            {
                // following code assumes there are left and right sides of the smashed block

                int new_entity1 = InternalCalls.CloneEntity(entity);
                int new_entity2 = InternalCalls.CloneEntity(entity);

                //determine start of entity1
                float start_point1 = entity_pos.x - entity_size.x / 2f;
                //determine end point of entity1
                float end_point1 = pos.x - size.x / 2f;
                //determine lenght of entity1
                float length1 = end_point1 - start_point1;
                // entity 1 spawn point
                SimpleVector2D entity1_pos = new SimpleVector2D(start_point1 + length1 / 2f, entity_pos.y);
                // set entity 1 pos and scale
                InternalCalls.TransformSetPositionEntity(entity1_pos.x, entity1_pos.y, new_entity1);
                InternalCalls.TransformSetScaleEntity(length1, entity_size.y, new_entity1);


                //determine start point of entity2
                float start_point2 = pos.x + size.x / 2f;
                //determine end point of entity2
                float end_point2 = entity_pos.x + entity_size.x / 2f;
                //determine length of entity2
                float length = end_point2 - start_point2;
                //new entity 2 spawn point
                SimpleVector2D entity2_pos = new SimpleVector2D(start_point2 + length / 2, entity_pos.y);
                // set entity position and scale
                InternalCalls.TransformSetPositionEntity(entity2_pos.x, entity2_pos.y, new_entity2);
                InternalCalls.TransformSetScaleEntity(length, entity_size.y, new_entity2);


                // make the first entity fall
                InternalCalls.RigidBodySetBodyTypeEntity(1, entity);
                InternalCalls.TransformSetScaleEntity(size.x, entity_size.y, entity);
                InternalCalls.TransformSetPositionEntity(pos.x, entity_pos.y, entity);
                InternalCalls.RigidBodySetMassEntity(size.x, entity);

                return;
            }



            // assume that there are no right blocks
            if(entity_rightmost <boss_right)
            {

                int new_entityR= InternalCalls.CloneEntity(entity);
                
                //length of new block 
                float lengthR= entity_rightmost - boss_left;

                //position of new block
                float x_pos = entity_rightmost - lengthR / 2f;

                //set new block pos
                InternalCalls.TransformSetScaleEntity(lengthR, entity_size.y, new_entityR);
                InternalCalls.TransformSetPositionEntity(x_pos, entity_pos.y,new_entityR);
                InternalCalls.RigidBodySetBodyTypeEntity(1, new_entityR);
                InternalCalls.RigidBodySetMassEntity(lengthR, new_entityR);

                // change old block pos
                InternalCalls.TransformSetScaleEntity(entity_size.x - lengthR, entity_size.y, entity);
                InternalCalls.TransformSetPositionEntity(entity_pos.x - lengthR / 2f, entity_pos.y, entity);



                return;
            }

            // assume that there are no left blocks (boss hits only left edge
            if (entity_leftmost > boss_left)
            {

                int new_entityL = InternalCalls.CloneEntity(entity);

                //length of new block 
                float lengthL = boss_right - entity_leftmost;

                //position of new block
                float x_pos = entity_leftmost + lengthL / 2f;

                //set new block pos
                InternalCalls.TransformSetScaleEntity(lengthL, entity_size.y, new_entityL);
                InternalCalls.TransformSetPositionEntity(x_pos, entity_pos.y, new_entityL);
                InternalCalls.RigidBodySetBodyTypeEntity(1, new_entityL);
                InternalCalls.RigidBodySetMassEntity(lengthL, new_entityL);

                // change old block pos
                InternalCalls.TransformSetScaleEntity(entity_size.x - lengthL, entity_size.y, entity);
                InternalCalls.TransformSetPositionEntity(entity_pos.x + lengthL / 2f, entity_pos.y, entity);


                return;
            }





        }


    }
}

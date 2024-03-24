using System;
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

        // for summon enemy
        static private float singing_spell_timer_set = 2f;
        static private float singing_spell_timer = 2f;
        static private bool isSummoning = false;


        // for idle
        static private float idle_timer = 2f;
        static private float idle_timer_set = 2f;

        private enum BossStates : int
        {
            Idle=0,
            Clap=1,
            Smash=2,
            SummonEnemy=3
        }

        static private BossStates current_state = BossStates.Smash;

        static public void Init(){
            CameraScript.CameraTargetZoom(0.5f, 1f);
            Boss_spawn_pos = InternalCalls.GetTransformPosition();
            InternalCalls.AudioStopAllSounds();
            InternalCalls.AudioPlayMusicSFX("SCI-FI-AMBIENCE_GEN-HDF-20503.wav", 0.1f);

            // clear all enemy dic
            HandEnemy.enemies.Clear();
            Enemy.enemies.Clear();
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
                case BossStates.SummonEnemy:
                    SummonEnemy();
                    break;
            }
        }



        static private void StateChanger()
        {
            //float random=InternalCalls.GetRandomFloat();
            MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
            uint random = rnd.Next(0,3); // random from 0 to 2
            //random = 2;
            if (random == 0)
            {
                ResetPosition();
                current_state = BossStates.Smash;
                return;
            }
            if(random == 1)
            {
                ResetPosition();
                current_state = BossStates.Idle;
                return;
            }
            if (random == 2)
            {
                ResetPosition();
                current_state = BossStates.SummonEnemy;
                Console.WriteLine("summoning");
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
                    smash = false;
                    StateChanger();
                    return;
                }

                if(InternalCalls.RigidBodyGetBodyTypeEntity(entity) == 1)
                {
                    float x_dir = CustomMath.Abs(InternalCalls.GetTransformPositionEntity(entity).x - InternalCalls.GetTransformPosition().x);
                    InternalCalls.RigidBodyAddForceEntity(x_dir * 200, 0, entity);
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
            InternalCalls.AudioPlaySoundSFX("EXPLOSION-LARGE_GEN-HDF-10849.wav",false, 0.6f);
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

        static private void SummonEnemy()
        {
            CameraScript.CameraTargetZoom(0.3f, 2f);
            SingingSpell();
            
        }

        static private void SingingSpell()
        {
            if (singing_spell_timer > 0f)
            {
                singing_spell_timer -= InternalCalls.GetDeltaTime();

                // spawn particles
                SpawnSummoningParticles();

                // shake camera
                SummoningCameraShake();

                // summoning
                Summoning();


            }
            else
            {
                // summon ends

                // reset
                singing_spell_timer = singing_spell_timer_set;
                isSummoning = false;
                StateChanger();
            }
        }

        static private void Summoning()
        {
            if (!isSummoning) // summon enemy once only
            {
                // spawn random number of enemies in random location on the ground
                MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
                int random = (int)rnd.Next(1, 4); // random from 1 to 3
                for (int i = 0; i < 1; i++)
                {
                    int enemy_id = InternalCalls.CreateEntityPrefab("HandEnemy");
                    HandEnemy.enemies.Add(enemy_id, new EachHandEnemy());
                    Console.WriteLine("spawn enemy");
                    Vector2D rnd_pos = PlayerScript.player_pos;
                    // random left/right pos around player
                    rnd_pos.x = rnd_pos.x + 1000f * (rnd.NextFloat() - 0.5f);
                    InternalCalls.TransformSetPositionEntity(rnd_pos.x, rnd_pos.y, enemy_id);
                }
                isSummoning = true;
            }
        }

        static private void SpawnSummoningParticles()
        {
            MyRandom rnd = new MyRandom((uint)(12924 * InternalCalls.GetRandomFloat()));

            SimpleVector2D pos = InternalCalls.GetTransformPosition();
            SimpleVector2D scaling = InternalCalls.GetTransformScaling();

            for (int i = 0; i < 30; i++)
            {

                float rand = rnd.NextFloat();
                float dir = rnd.NextFloat() * 360;
                //float dir = MathF.Sign(scaling.x) > 0 ? 330 + 30 * dir_rand /* 330 to 360 */: 180 + 30 * dir_rand/* 180 to 210 */;
                
                rand = rnd.NextFloat();
                float size = 30f * rand;

                rand = rnd.NextFloat();
                float size_scale = 30 * rand;

                rand = rnd.NextFloat();
                float alpha = 0.8f * rand;

                rand = rnd.NextFloat();
                float lifetime = 0.5f + 0.5f * rand;

                rand = rnd.NextFloat();
                float speed = 500f + 500f * rand;

                rand = rnd.NextFloat();
                float x = pos.x + scaling.x / 2f * (rand - 0.5f);

                rand = rnd.NextFloat();
                float y = pos.y + scaling.y / 2f * (rand - 0.5f);

                InternalCalls.GameSpawnParticleExtra(
                    x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bleeding.txt"
                 );
            }
        }

        static private void SummoningCameraShake()
        {
            CameraScript.CameraShake(2f);
            CameraScript.camera_shake_intensity = 2f;
            CameraScript.camera_shake_duration = 0.2f;
        }


    } // class end here
}

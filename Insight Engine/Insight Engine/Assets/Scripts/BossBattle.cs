using System;
using System.Runtime.CompilerServices;

namespace IS
{
    class BossBattle
    {

        static SimpleVector2D Boss_spawn_pos;

        //boss stats
        static public float boss_hp=150;
        static private float boss_max_hp=500;
        static private int boss_phase = 0; // when health drops below 0 go to next phase

        //boss projectiles
        static private float boss_projectile_timer;
        static private float boss_projectile_timer_set;



        // for smash follow
        static private float smash_timer = 2f;
        static private float smash_timer_set = 2f;
        static private bool smash=false;
        static private float miss_smash_y_pos = -600f;

        // for summon enemy
        static private float singing_spell_timer_set = 2f;
        static private float singing_spell_timer = 2f;
        static private bool isSingingSpell = false;
        static private bool isSummoning = false;
        static private Vector2D enemy_spawn_pos = new Vector2D(0f, 0f);


        // for idle
        static private float idle_timer = 2f;
        static private float idle_timer_set = 2f;

        // health bar
        static SimpleImage health_bar;
        static private Vector2D health_bar_scaling = new Vector2D(1000f, 250f);

        // boss invul after hit duration
        static private float invul_timer = 0.1f;
        static private float invul_timer_set = 0.1f;

        // projectile image
        static SimpleImage boss_bullet;

        // sweep timer for projectiles
        static private float sweep_timer = 3f;
        static private float sweep_timer_set = 3f;
        static private bool sweeped=false;
        // bullet direction
        static int number_of_bullets = 40;
        static int[] bullet_array = new int[number_of_bullets];

        // hand clap
        static private bool is_clapping = false;
        static private float boss_move_away_timer_set = 2f;
        static private float boss_move_away_timer = 2f;

        // for scene transition to restore sweep
        static private bool restore_sweep = false;  

        public enum BossStates : int
        {
            Idle=0,
            Clap=1,
            Smash=2,
            SummonEnemy=3,
            LeftSweep=4,
            RightSweep=5,
            SpikesSpawn=6,
            Boss360=7,
            ReturnSpikesSpawn=8,
            ReturnBoss360=9

        }

        static public BossStates current_state = BossStates.Boss360;




        static public void Init(){
            CameraScript.StopCameraPan();
            ClearBullets();
            index360 = 0; //this is for spawning one by one
            restore_sweep=false; //scene transistion

            boss_phase = 0;
            // reseting boss health
            boss_hp = boss_max_hp;
            health_bar = InternalCalls.GetSpriteImage("enemy_healthbar.png");
            boss_bullet = InternalCalls.GetSpriteImage("Boss Projectile.png");
            current_state = BossStates.Smash;

            CameraScript.CameraTargetZoom(0.5f, 1f);
            Boss_spawn_pos = InternalCalls.GetTransformPosition();
            InternalCalls.AudioStopAllSounds();
            InternalCalls.AudioPlayMusicSFX("SCI-FI-AMBIENCE_GEN-HDF-20503.wav", 0.1f);

            // clear all enemy dic
            HandEnemy.enemies.Clear();
            Enemy.enemies.Clear();

            // init 
            is_clapping = false;
        }

        static public void Update(){

            if (InternalCalls.KeyPressed((int)KeyCodes.Slash))
            {
                boss_hp = 0;
            }


                InternalCalls.SetGravityScale(0f);
            InternalCalls.TransformSetRotation(0, 0);

            // restart level if player lose
            if (PlayerScript.isDead)
            {
                PlayerScript.isDead = false;
                InternalCalls.LoadScene("Assets/Scenes/BossLevel.insight");
            }


            // stopping the scene and playing transition to next phase
            if (boss_hp <= 0)
            {
                if (boss_phase==0)
                {
                    if (!restore_sweep)
                    {
                        sweeped = false;
                        sweep_timer = sweep_timer_set;
                        restore_sweep = true;
                    }
                    GoNextPhase(); // this will increase the boss_phase itself
                    return;
                }

                if(boss_phase==1)
                {
                    InternalCalls.LoadScene("Assets/Scenes/Credits.insight");
                }

            }


            BossFSM();

            DrawHealthBar();
            //TakeDamage();
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
                case BossStates.Clap: //clap function to add here
                    BossHandClap();
                    break;
                case BossStates.Smash:
                    FollowBeforeSmash();
                    break;
                case BossStates.SummonEnemy:
                    SummonEnemy();
                    break;
                case BossStates.LeftSweep:
                    LeftSweep();
                    break;
                case BossStates.RightSweep:
                    RightSweep();
                    break;
                case BossStates.SpikesSpawn:
                    SpikesSpawn(-1f);
                    break;
                case BossStates.ReturnSpikesSpawn:
                    SpikesSpawn(1f);
                    break;
                case BossStates.Boss360:
                    BossAOE();
                    break;
                case BossStates.ReturnBoss360:
                    ReverseAOE();
                    break;

            }
        }




        static private void DrawHealthBar() // draw health bar
        {
            
            Vector3 color = new Vector3(0f, 0f, 0f);
            if (boss_hp > boss_max_hp/2)
            {
                color = new Vector3(0f, 1f, 0f);
            }
            else if (boss_hp > boss_max_hp/5)
            {
                color = new Vector3(1f, 1f, 0f);
            }
            else
            {
                color = new Vector3(1f, 0f, 0f);
            }
            int layer = InternalCalls.GetTopLayer();
            float health_wdith = (((boss_hp > 0) ? boss_hp : 0f) / boss_max_hp) * health_bar_scaling.x*0.8f /CameraScript.camera_zoom; // width lenght of the health bar
            float health_pos_x = CameraScript.camera_pos.x;
            float health_pos_y = CameraScript.camera_pos.y+400f/CameraScript.camera_zoom;
            Vector2D health_bar_length = new Vector2D(health_wdith, 84f / CameraScript.camera_zoom);

            // draw health bar only when the health is lesser
            if (boss_hp <= boss_max_hp && boss_hp > 0f)
            {
                // draw health blood
                InternalCalls.DrawSquare(health_pos_x, health_pos_y, health_bar_length.x, health_bar_length.y, color.x, color.y, color.z, 0.7f, layer);

                // draw health bar UI
                SimpleVector2D pos = new SimpleVector2D(health_pos_x- 8f / CameraScript.camera_zoom, health_pos_y);
                InternalCalls.DrawImageAt
                    (
                        pos, 0, new SimpleVector2D(health_bar_scaling.x / CameraScript.camera_zoom, health_bar_scaling.y / CameraScript.camera_zoom), health_bar, 1f, layer
                    );
            }
        }


        static private void TakeDamage()
        {
            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            if (invul_timer > 0)
            {
                invul_timer -= InternalCalls.GetDeltaTime();
            }

            if (array.FindIndex(PlayerScript.entity_attack) != -1 && invul_timer <=0)
            {
                boss_hp -= PlayerScript.attack_damage;
                invul_timer = invul_timer_set;
            }
        }





        static public void StateChanger()
        {
            //float random=InternalCalls.GetRandomFloat();
            MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
            uint random = rnd.Next(0,4); // random from 0 to 3
            //uint random = 3; // random from 0 to 3
            //random = 2;
            if (random == 0)
            {
                if (boss_phase == 0)
                {
                    ResetPosition();
                    current_state = BossStates.Smash;
                    return;
                }
                if (boss_phase == 1)
                {
                    ResetPosition();
                    current_state = BossStates.SummonEnemy;
                    //Console.WriteLine("summoning");
                    return;
                }

            }
            if(random == 1)
            {
                ResetPosition();
                current_state = BossStates.Idle;
                return;
            }
            if (random == 2)
            {
                if (boss_phase == 0)
                {
                    ResetPosition();
                    ClearBullets();
                    current_state = BossStates.Boss360;
                    return;
                }

                if (boss_phase == 1)
                {
                    MyRandom rand = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
                    uint random2 = rnd.Next(0, 2); // random from 0 to 1
                    if (random2 == 0)
                    {
                        ResetPosition();
                        ClearBullets();
                        current_state = BossStates.LeftSweep;
                        return;
                    }
                    if (random2 == 1)
                    {
                        ResetPosition();
                        ClearBullets();
                        current_state = BossStates.RightSweep;
                        return;
                    }

                }


            }
            // if random 3 
            if (random == 3)
            {
                if (boss_phase == 0) //based on boss phase
                {
                    ResetPosition();
                    ClearBullets();
                    current_state = BossStates.SpikesSpawn;
                    return;
                }

                if (boss_phase == 1) // switching to clap here
                {
                    current_state = BossStates.Clap;
                    // reset before clapping
                    boss_move_away_timer = boss_move_away_timer_set;
                    is_clapping = false;
                    return;
                }

            }


        }


        static private void Rest()
        {
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
            SimpleVector2D pos = InternalCalls.GetTransformPosition();
            InternalCalls.RigidBodySetBodyTypeEntity(1, InternalCalls.GetCurrentEntityID());
            InternalCalls.TransformSetPosition(pos.x, pos.y - 2000f * InternalCalls.GetDeltaTime());



            // check for collide
            int boss = InternalCalls.GetCurrentEntityID();
            SimpleArray collided_objects = InternalCalls.GetCollidingEntityArray(boss);
            for (int i = 0; i < collided_objects.length; i++)
            {
                int entity = collided_objects.GetValue(i);
                if(entity==PlayerScript.PLAYER_ID)
                {
                    if (PlayerScript.invulnerable == false)
                    {
                        PlayerScript.Health -= 1;
                    }

                    smash_timer = smash_timer_set;
                    smash = false;
                    InternalCalls.RigidBodySetBodyTypeEntity(6, InternalCalls.GetCurrentEntityID());
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
                    InternalCalls.RigidBodySetBodyTypeEntity(6, InternalCalls.GetCurrentEntityID());
                    StateChanger();

                }

                if (InternalCalls.RigidBodyGetBodyTypeEntity(entity) == 5)
                {
                    boss_hp -= boss_max_hp/12;
                    smash = false;
                    smash_timer = smash_timer_set;
                    InternalCalls.RigidBodySetBodyTypeEntity(6, InternalCalls.GetCurrentEntityID());
                    StateChanger();

                }

            }

            // if the boss miss
/*            if(InternalCalls.GetTransformPosition().y < miss_smash_y_pos)
            {
                smash = false;
                smash_timer = smash_timer_set;
                StateChanger();
            }*/


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
           // CameraScript.CameraTargetZoom(0.3f, 2f); //zoom is hard to see so i remove for now
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

                // draw spawn position
                RenderSpawnPosition();

            }
            else
            {
                // summoning
                Summoning();

                // summon ends

                // reset
                singing_spell_timer = singing_spell_timer_set;
                isSingingSpell = false;
                isSummoning = false;

                StateChanger();
            }
        }

        static private void Summoning()
        {
            if (!isSummoning) // summon enemy once only
            {
                // spawn random number of enemies in random location on the ground
                //MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
                //int random = (int)rnd.Next(1, 4); // random from 1 to 3
                for (int i = 0; i < 1; i++) // spawn one enemy each time
                {
                    int enemy_id = InternalCalls.CreateEntityPrefab("HandEnemy");

                    HandEnemy.enemies.TryAdd(enemy_id, new EachHandEnemy());
                   // Console.WriteLine("spawn enemy");
                    //Vector2D rnd_pos = PlayerScript.player_pos;
                    // random left/right pos around player
                    //rnd_pos.x = rnd_pos.x + 1000f * (rnd.NextFloat() - 0.5f);
                    InternalCalls.TransformSetPositionEntity(enemy_spawn_pos.x, enemy_spawn_pos.y, enemy_id); // enemy spawn position was defined by RenderSpawnPosition()
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
                float dir = 80 + 20 * rand;
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
            CameraScript.camera_shake_intensity = 0.2f;
            CameraScript.camera_shake_duration = 0.2f;
        }

        static private void RenderSpawnPosition()
        {
            // spawn random number of enemies in random location on the ground
            MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
            if (!isSingingSpell) // render spawn position once only
            {
                enemy_spawn_pos = PlayerScript.player_pos;
                // random left/right pos around player
                enemy_spawn_pos.x = enemy_spawn_pos.x + 1000f * (rnd.NextFloat() - 0.5f);
                isSingingSpell = true;
            }

            // render spawning particles
            for (int i = 0; i < 3; i++)
            {

                float rand = rnd.NextFloat();
                float dir = rnd.NextFloat() * 360;
                //float dir = MathF.Sign(scaling.x) > 0 ? 330 + 30 * dir_rand /* 330 to 360 */: 180 + 30 * dir_rand/* 180 to 210 */;

                rand = rnd.NextFloat();
                float size = 20f * rand;

                rand = rnd.NextFloat();
                float size_scale = 20 * rand;

                rand = rnd.NextFloat();
                float alpha = 0.8f * rand;

                rand = rnd.NextFloat();
                float lifetime = 0.3f + 0.3f * rand;

                rand = rnd.NextFloat();
                float speed = 200f + 200f * rand;

                InternalCalls.GameSpawnParticleExtra(
                    enemy_spawn_pos.x, enemy_spawn_pos.y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Enemy Bleeding.txt"
                 );
            }
        }

        static private void ClearBullets()
        {
            BossProjectile.destroy_self = true;
            
        }
        static private void LeftSweep()
        {
            // ensure the boss is at the defaulted position
            if (MoveToLocation(Boss_spawn_pos.x, Boss_spawn_pos.y)) { return; }

            //create all entities

            if (!sweeped)
            {
                //clear all existing bullet directions
                BossProjectile.bullet_direction.Clear();

                // allow bullets to exist
                BossProjectile.destroy_self = false;

                int flipper = 1;
                for (int i = 0; i < number_of_bullets; i++)
                {
                    //flip flipper
                    flipper *= -1;

                    int entity = InternalCalls.CreateEntityPrefab("Boss Projectile");
                    SimpleVector2D scale = InternalCalls.GetTransformScalingEntity(entity);
                    InternalCalls.TransformSetScaleEntity(-scale.x, scale.y, entity);
                    InternalCalls.TransformSetPositionEntity(PlayerScript.player_pos.x+500f/CameraScript.camera_zoom, PlayerScript.player_pos.y+ i*flipper*scale.y*1.5f,entity);
                    bullet_array[i] = entity;
                }

/*
                MyRandom rnd = new MyRandom((uint)(221 * InternalCalls.GetRandomFloat()));
                // sweep chooser
                int rand = (int)rnd.Next(0, (uint)number_of_bullets);

                InternalCalls.DestroyEntity(bullet_array[rand]);*/

                sweeped = true;
            }

            sweep_timer-=InternalCalls.GetDeltaTime();

            if (sweep_timer > 1.5 && sweep_timer < 2)
            {
                for (int i = 0; i < number_of_bullets; i++)
                {
                    BossProjectile.bullet_direction.TryAdd(bullet_array[i], (-40, 0));
                }
            }

            if (sweep_timer < 0)
            {


                sweep_timer = sweep_timer_set;
                sweeped = false;
                StateChanger();
            }


        }



        static private void RightSweep()
        {
            // ensure the boss is at the defaulted position
            if (MoveToLocation(Boss_spawn_pos.x, Boss_spawn_pos.y)) { return; }

            //create all entities

            if (!sweeped)
            {
                //clear all existing bullet directions
                BossProjectile.bullet_direction.Clear();

                // allow bullets to exist
                BossProjectile.destroy_self = false;

                int flipper = 1;
                for (int i = 0; i < number_of_bullets; i++)
                {
                    //flip flipper
                    flipper *= -1;

                    int entity = InternalCalls.CreateEntityPrefab("Boss Projectile");
                    SimpleVector2D scale = InternalCalls.GetTransformScalingEntity(entity);
                    InternalCalls.TransformSetScaleEntity(scale.x, scale.y, entity);
                    InternalCalls.TransformSetPositionEntity(PlayerScript.player_pos.x - 500f / CameraScript.camera_zoom, PlayerScript.player_pos.y + i * flipper * scale.y * 1.5f, entity);
                    bullet_array[i] = entity;
                }

                /*
                                MyRandom rnd = new MyRandom((uint)(221 * InternalCalls.GetRandomFloat()));
                                // sweep chooser
                                int rand = (int)rnd.Next(0, (uint)number_of_bullets);

                                InternalCalls.DestroyEntity(bullet_array[rand]);*/

                sweeped = true;
            }

            sweep_timer -= InternalCalls.GetDeltaTime();

            if (sweep_timer > 1.5 && sweep_timer < 2)
            {
                for (int i = 0; i < number_of_bullets; i++)
                {
                    BossProjectile.bullet_direction.TryAdd(bullet_array[i], (40, 0));
                }
            }

            if (sweep_timer < 0)
            {


                sweep_timer = sweep_timer_set;
                sweeped = false;
                current_state = BossStates.LeftSweep;
                ClearBullets();
            }


        }




        static private void SpikesSpawn(float left_or_right)
        {
            // ensure the boss is at the defaulted position
            if (MoveToLocation(Boss_spawn_pos.x, Boss_spawn_pos.y)) { return; } 

            //create all entities

            if (!sweeped)
            {
                //clear all existing bullet directions
                BossProjectile.bullet_direction.Clear();

                // allow bullets to exist
                BossProjectile.destroy_self = false;

                float flipper = 1;
                for (int i = 0; i < number_of_bullets; i++)
                {
                    //flip flipper
                    flipper *= -1;

                    int entity = InternalCalls.CreateEntityPrefab("Boss Projectile Spikes");
                    SimpleVector2D scale = InternalCalls.GetTransformScalingEntity(entity);
                    InternalCalls.TransformSetScaleEntity(-scale.x *left_or_right, scale.y, entity);
                    InternalCalls.TransformSetPositionEntity(CameraScript.camera_pos.x - left_or_right*800f / CameraScript.camera_zoom, CameraScript.camera_pos.y + i * scale.y/2f *flipper , entity);
                    bullet_array[i] = entity;
                }


                MyRandom rnd = new MyRandom((uint)(221 * InternalCalls.GetRandomFloat()));
                // sweep chooser
                int rand = (int)rnd.Next(0, 3);

                InternalCalls.DestroyEntity(bullet_array[rand]);
                bullet_array[rand] = 0;

                sweeped = true;
            }

            sweep_timer -= InternalCalls.GetDeltaTime();


            if (sweep_timer > 1 && sweep_timer < 1.5)
            {
                for (int i = 0; i < number_of_bullets; i++)
                {
                    if (bullet_array[i]!=0)
                    BossProjectile.bullet_direction.TryAdd(bullet_array[i], (40 *left_or_right, 0));
                }
            }

            if (sweep_timer < 0)
            {

                sweep_timer = sweep_timer_set;
                sweeped = false;
                if (left_or_right == -1)
                {
                    ClearBullets();
                    current_state = BossStates.ReturnSpikesSpawn;
                 
                }
                else
                {
                    ClearBullets();
                    current_state = BossStates.Smash;
                }
                
            }


        }

        static int index360 = 0;
        static private void BossAOE()
        {
            // ensure the boss is at the center position
            if (MoveToLocation(0, 0)) { return; };

            //create all entities

            if (!sweeped)
            {
                //clear all existing bullet directions
                BossProjectile.bullet_direction.Clear();

                // allow bullets to exist
                BossProjectile.destroy_self = false;

                int flipper = 1;
                int i = index360;
                //flip flipper
                flipper *= -1;

                int entity = InternalCalls.CreateEntityPrefab("Boss Projectile");
                SimpleVector2D pos = InternalCalls.GetTransformPosition();
                SimpleVector2D scale = InternalCalls.GetTransformScalingEntity(entity);
                InternalCalls.TransformSetScaleEntity(-scale.x, scale.y, entity);

                float degree = i * 360 / number_of_bullets;
                Vector2D from_dir = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(degree));
                from_dir = from_dir.Multiply(800);
                InternalCalls.TransformSetPositionEntity(pos.x+from_dir.x,pos.y+from_dir.y,entity);
                InternalCalls.TransformSetRotationEntity(degree+180, 0, entity);
                
                   
                bullet_array[i] = entity;

                if (index360 < number_of_bullets-1)
                {
                    index360++;
                    return;
                }

                index360 = 0;
                sweeped = true;
            }

            sweep_timer -= InternalCalls.GetDeltaTime();

            if (sweep_timer > 1.5 && sweep_timer < 2)
            {
                for (int i = 0; i < number_of_bullets; i++)
                {
                    if (bullet_array[i] != 0)
                    {
                        Vector2D from_dir = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(i * 360 / number_of_bullets));
                        from_dir = from_dir.Multiply(40);
                        BossProjectile.bullet_direction.TryAdd(bullet_array[i], (from_dir.x, from_dir.y));
                    }

                }
            }

            if (sweep_timer < 0)
            {
                sweep_timer = sweep_timer_set/1.4f; //this is for the return to be shorter
                sweeped = false;
                current_state = BossStates.ReturnBoss360;
            }


        }

        static private void ReverseAOE()
        {
            if (!sweeped)
            {
                for (int i = 0; i < number_of_bullets; i++)
                {
                    if (bullet_array[i] != 0 && BossProjectile.bullet_direction.ContainsKey(bullet_array[i]))
                    {
                        // Correctly declare 'tuple' with its explicit type.
                        float i1= BossProjectile.bullet_direction[bullet_array[i]].Item1;
                        float i2= BossProjectile.bullet_direction[bullet_array[i]].Item2;


                        Vector2D from_dir = new Vector2D(-i1, -i2);
                        BossProjectile.bullet_direction[bullet_array[i]] = (from_dir.x, from_dir.y);
                        
                    }
                }
                sweeped = true;
            }

            sweep_timer-=InternalCalls.GetDeltaTime();
            if (sweep_timer < 0)
            {
                sweep_timer = sweep_timer_set;
                sweeped = false;
                current_state = BossStates.Idle;
            }
        }



        // if movement is completed return false
        static private bool MoveToLocation( float directed_x, float directed_y)
        {
            InternalCalls.RigidBodySetBodyTypeEntity(6, InternalCalls.GetCurrentEntityID());
            float move_speed = 15f;
            SimpleVector2D current_pos = InternalCalls.GetTransformPosition();
            SimpleVector2D directed = new SimpleVector2D(directed_x, directed_y);
            Vector2D direction = new Vector2D(directed.x - current_pos.x,
                                              directed.y - current_pos.y);
            if (Vector2D.Distance(Vector2D.FromSimpleVector2D(current_pos), Vector2D.FromSimpleVector2D(directed)) > 15f)
            {


            direction = direction.Normalize();
            InternalCalls.TransformSetPosition(current_pos.x + direction.x * move_speed, current_pos.y + direction.y * move_speed);


                return true;
            }

            return false;
        }

        static private void BossHandClap()
        {
            // move current boss away
            SimpleVector2D pos = InternalCalls.GetTransformPosition();
            pos.y += 20f;
            InternalCalls.TransformSetPosition(pos.x, pos.y);

            if (boss_move_away_timer > 0f)
            {
                boss_move_away_timer -= InternalCalls.GetDeltaTime();
                return;
            }

            if (!is_clapping)
            {
                // create left hand
                int left_hand = InternalCalls.CreateEntityPrefab("BossLeftHand");
                is_clapping = true;
            }
        }

        static private void GoNextPhase()
        {
            //clear bullets
            ClearBullets();
            InternalCalls.TransformSetPositionEntity(0, 500, PlayerScript.PLAYER_ID);
            BossProjectile.bullet_direction.Clear();
            CameraScript.CameraShake(2f);
            CameraScript.camera_shake_duration = 0.1f;
            CameraScript.camera_shake_intensity = 1f;
           // CameraScript.CameraPanTo(new Vector2D(0,500),0.5f);
            CameraScript.CameraTargetZoom(0.5f, 0.5f);

            if (MoveToLocation(0f, -2000f)) { return; } //prevents anything from happening when not done

            if (!sweeped)
            {
                for (int i = 0; i < InternalCalls.GetTotalEntities(); i++)
                {
                    if (InternalCalls.CheckEntityCategory(i, "Ground"))
                    {
                        InternalCalls.RigidBodySetBodyTypeEntity(1, i);
                    }
                }

                sweeped = true; //cleanup all the floors
                sweep_timer = 3f; //1s to destroy all entities
            }
            sweep_timer -= InternalCalls.GetDeltaTime();
            if (sweep_timer <= 0f)
            {
                for (int i = 0; i < InternalCalls.GetTotalEntities(); i++)
                {
                    if (InternalCalls.CheckEntityCategory(i, "Ground"))
                    {
                        InternalCalls.DestroyEntity(i);
                    }
                }

                for (int i = 0; i< 5; i++)
                {
                    int entity = InternalCalls.CreateEntityPrefab("Floor");
                    InternalCalls.TransformSetPositionEntity(-2000 + i * 900, -1000,entity);
                }
                


                sweep_timer = sweep_timer_set;
                sweeped=false;
                boss_phase = 1;
                boss_hp = boss_max_hp;
                Boss_spawn_pos.x = 0;
                Boss_spawn_pos.y = -300;
                current_state = BossStates.Idle;
                PlayerScript.player_cam_y_offset = 300f;
                CameraScript.StopCameraPan();
            }

        }



    } // class end here
}

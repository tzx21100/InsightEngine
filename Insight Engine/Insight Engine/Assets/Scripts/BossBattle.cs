using System;
using System.Runtime.CompilerServices;

namespace IS
{
    class BossBattle
    {

        static SimpleVector2D Boss_spawn_pos;
        static public int BOSS_ID=9999;//random number first

        //boss stats
        static public float boss_hp=150;
        static private float boss_max_hp=250;
        static public int boss_phase = 0; // when health drops below 0 go to next phase

        static private float temp_boss_hp=150; // for decrease hp animation
        static private float minus_hp_timer_set = 0.5f;
        static private float minus_hp_timer = 0.5f;

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

        static public bool wasHit=false;
        static private float invul_timer = 0.1f;
        static private float invul_timer_set = 0.1f;

        // projectile image
        static SimpleImage boss_bullet;

        // sweep timer for projectiles
        static private float sweep_timer = 3f;
        static private float sweep_timer_set = 3f;
        static private bool sweeped=false;

        // bullet 
        static int number_of_bullets = 40;


        // hand clap
        static private bool is_clapping = false;
        static private float boss_move_away_timer_set = 2f;
        static private float boss_move_away_timer = 2f;

        // for scene transition to restore sweep
        static private bool restore_sweep = false;

        //images
        static SimpleImage boss_smash_image;
        static SimpleImage boss_clap_image;
        static SimpleImage boss_idle_image;
        static SimpleImage boss_grab_image;
        static SimpleImage boss_death_image;

        // for 360 random locations
        static float random_x_location;
        static float random_y_location;

        //spawning one by one
        static int index360 = 0;
        static bool play360audio = false;
        static bool hitsound = false;

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

        //static public bool is_dead = false;


        static public void Init(){
            BOSS_ID = InternalCalls.GetCurrentEntityID();

            BossProjectile.CLEAN();


            InternalCalls.SetColliderOffset(0, 0);

            boss_idle_image = InternalCalls.GetSpriteImage("BossIdle 4R4C.png");
            boss_smash_image = InternalCalls.GetSpriteImage("BossSmash 5R4C.png");
            boss_clap_image = InternalCalls.GetSpriteImage("BossClap 6R4C.png");
            boss_grab_image = InternalCalls.GetSpriteImage("BossGrab 6R4C.png");
            boss_death_image = InternalCalls.GetSpriteImage("BossDeath 6R4C.png");

            InternalCalls.SetAnimationEntityPlaying(InternalCalls.GetCurrentEntityID(), true);
            //Change size of the collider

            InternalCalls.SetSpriteImage(boss_idle_image);
            InternalCalls.SetSpriteAnimationIndex(0);

            InternalCalls.ResetAnimations();
            InternalCalls.CreateAnimationFromSprite(4, 4, 0.7f);  // idle 0
            InternalCalls.CreateAnimationFromSprite(5, 4, 0.7f);  //smash 1
            InternalCalls.CreateAnimationFromSprite(6, 4, 0.7f); // clap 2
            InternalCalls.CreateAnimationFromSprite(6, 4, 0.7f);  //grab 3
            InternalCalls.CreateAnimationFromSprite(6, 4, 0.7f); // death 4
            



            CameraScript.StopCameraPan();

            index360 = 0; //this is for spawning one by one
            restore_sweep=false; //scene transistion

            boss_phase = 0;
            // reseting boss health
            boss_hp = boss_max_hp;
            temp_boss_hp = boss_hp;
            minus_hp_timer = minus_hp_timer_set;
            health_bar = InternalCalls.GetSpriteImage("UI_Healthbar_Boss.png");
            boss_bullet = InternalCalls.GetSpriteImage("Boss Projectile.png");
            current_state = BossStates.Smash;

            CameraScript.CameraTargetZoom(0.5f, 1f);
            Boss_spawn_pos = InternalCalls.GetTransformPosition();
            //InternalCalls.AudioPlayMusicSFX("SCI-FI-AMBIENCE_GEN-HDF-20503.wav", 0.1f);

            // clear all enemy dic
            HandEnemy.enemies.Clear();
            Enemy.enemies.Clear();

            // init 
            is_clapping = false;

            //update audio
            play360audio = false;

        }

        static public void Update(){
            BOSS_ID = InternalCalls.GetCurrentEntityID();

            if(PauseButtonScript.paused == true)
            {
                return; //do nothing when pause
            }

            if (InternalCalls.KeyPressed((int)KeyCodes.J))
            {
                boss_hp = 0;
            }


                InternalCalls.SetGravityScale(0f);
            InternalCalls.TransformSetRotation(0, 0);


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
                    BossProjectile.DestroyAllProjectiles();
                    BossProjectile.CLEAN();
                    int dead = InternalCalls.CreateEntityPrefab("BossDeath");
                    SimpleVector2D pos = InternalCalls.GetTransformPositionEntity(InternalCalls.GetCurrentEntityID());
                    InternalCalls.TransformSetPositionEntity(pos.x, pos.y, dead);
                    InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());
                }

            }


            BossFSM();

            DrawHealthBar();
            TakeDamage();
            //update projectile
            BossProjectile.Update();

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
            if (temp_boss_hp >= boss_hp)
            {
                // render the white bar for the minus health effect
                RenderMinusHealthBarEffect();
            }
            else
            {
                minus_hp_timer = minus_hp_timer_set; // reset the timer when the white bar is same as the health bar
            }

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
            float health_wdith = (((boss_hp > 0) ? boss_hp : 0f) / boss_max_hp) * health_bar_scaling.x* 0.87f / CameraScript.camera_zoom; // width lenght of the health bar
            float health_pos_x = CameraScript.camera_pos.x;
            float health_blood_x = CameraScript.camera_pos.x - ((boss_max_hp - boss_hp) / boss_max_hp) * (health_bar_scaling.x * 0.87f / CameraScript.camera_zoom) / 2f - 8f / CameraScript.camera_zoom;
            float health_pos_y = CameraScript.camera_pos.y+400f/CameraScript.camera_zoom;
            Vector2D health_bar_length = new Vector2D(health_wdith, 84f / CameraScript.camera_zoom);

            // draw health bar only when the health is lesser
            if (boss_hp <= boss_max_hp && boss_hp > 0f)
            {
                // draw health blood
                InternalCalls.DrawSquare(health_blood_x, health_pos_y, health_bar_length.x, health_bar_length.y, color.x, color.y, color.z, 0.7f, layer);

                // draw health bar UI
                SimpleVector2D pos = new SimpleVector2D(health_pos_x- 8f / CameraScript.camera_zoom, health_pos_y);
                InternalCalls.DrawImageAt
                    (
                        pos, 0, new SimpleVector2D(health_bar_scaling.x / CameraScript.camera_zoom, health_bar_scaling.y / CameraScript.camera_zoom), health_bar, 1f, layer
                    );
            }
        }

        static private void RenderMinusHealthBarEffect()
        {
            // it will pause the white bar for a while
            if (minus_hp_timer > 0f)
            {
                minus_hp_timer -= InternalCalls.GetDeltaTime();
            }
            else
            {
                // decrease the hp only when the timer is less than 0
                temp_boss_hp -= 0.5f;
            }

            int layer = InternalCalls.GetTopLayer() - 1;
            float health_wdith = (((temp_boss_hp > 0) ? temp_boss_hp : 0f) / boss_max_hp) * health_bar_scaling.x * 0.87f / CameraScript.camera_zoom; // width lenght of the health bar
            //float health_pos_x = CameraScript.camera_pos.x;
            float health_blood_x = CameraScript.camera_pos.x - ((boss_max_hp - temp_boss_hp) / boss_max_hp) * (health_bar_scaling.x * 0.87f / CameraScript.camera_zoom) / 2f - 8f / CameraScript.camera_zoom;
            float health_pos_y = CameraScript.camera_pos.y + 400f / CameraScript.camera_zoom;
            Vector2D health_bar_length = new Vector2D(health_wdith, 84f / CameraScript.camera_zoom);

            // draw health bar only when the health is lesser
            if (temp_boss_hp <= boss_max_hp && temp_boss_hp > 0f)
            {
                // draw white bar
                InternalCalls.DrawSquare(health_blood_x, health_pos_y, health_bar_length.x, health_bar_length.y, 1f, 1f, 1f, 0.7f, layer);

            }
        }


        static private void TakeDamage()
        {
            if(wasHit)
            {
                if (!hitsound)
                {
                    hitsound = true;
                    BossHurtSound();

                    RenderTakeDamageParticles();

                }
                InternalCalls.SetSpriteAlpha(InternalCalls.GetRandomFloat() * 2f);

                invul_timer-=InternalCalls.GetDeltaTime();
                if(invul_timer < 0 )
                {
                    invul_timer = invul_timer_set;
                    wasHit = false;
                    hitsound = false;
                    InternalCalls.SetSpriteAlpha(1f);
                }
            }
        }

        static private void RenderTakeDamageParticles()
        {
            // load particles
            int boss_id = InternalCalls.GetCurrentEntityID();
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            int particle_count = (int)(my_rand.Next(20, 31)); // random from 80 to 100 particles
            SimpleVector2D pos = InternalCalls.GetTransformPositionEntity(boss_id);
            SimpleVector2D scaling = InternalCalls.GetTransformScalingEntity(boss_id);
            for (int i = 0; i < particle_count; i++)
            {
                float rand = my_rand.NextFloat();
                float dir = 90 + 90 * (rand - 0.5f);

                rand = my_rand.NextFloat();
                float size = 10 + 10f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = 10f + 10 * rand; // pariticles going nigger

                rand = my_rand.NextFloat();
                float alpha = 0.5f + 0.5f * rand; // 0 to 1

                rand = my_rand.NextFloat();
                float lifetime = 0.5f + 0.5f * rand; // 0.5s to 0.8s

                rand = my_rand.NextFloat();
                float speed = 500f + 500f * rand;

                rand = my_rand.NextFloat();
                float x = pos.x + scaling.x / 2f * (rand - 0.5f);
                //float x = enemy_pos.x;

                rand = my_rand.NextFloat();
                float y = pos.y + scaling.y / 2f * (rand - 0.5f);

                InternalCalls.GameSpawnParticleExtraFramesFull(
                    x, y, dir, size, size_scale, alpha, 0f, lifetime, speed,
                    "Particle Boss Die.txt", 1, 1, 5f, 2, lifetime, 10f,
                    InternalCalls.GetTopLayer(), (1f, 1f, 1f)
                 );
            }
        }



        static public void StateChanger()
        {
            InternalCalls.SetColliderOffset(0, 0); //reset collider
            InternalCalls.SetSpriteAnimationIndex(0); //reset animation
            InternalCalls.SetSpriteImage(boss_idle_image);

            //clear all projectiles on switch
            BossProjectile.DestroyAllProjectiles();

            //float random=InternalCalls.GetRandomFloat();
            MyRandom rnd = new MyRandom((uint)(129243 * InternalCalls.GetRandomFloat()));
            uint random = rnd.Next(0,4); // random from 0 to 3
            //uint random = 3; // random from 0 to 3
            //random = 3;
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
                    InternalCalls.AudioPlaySoundSFX("BossSummon.wav",false,0.3f);
                    current_state = BossStates.SummonEnemy;
                    //Console.WriteLine("summoning");
                    return;
                }

            }
            if(random == 1)
            {
                ResetPosition();
                if(current_state != BossStates.Idle)
                {
                    current_state = BossStates.Idle;
                }
                else
                {
                    StateChanger();
                }
                
                return;
            }
            if (random == 2)
            {
                if (boss_phase == 0)
                {
                    ResetPosition();
                    random_x_location =-1938+ InternalCalls.GetRandomFloat()*4000;
                    random_y_location = -700+ InternalCalls.GetRandomFloat()* 1200;
                    index360 = 0;
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

                        current_state = BossStates.LeftSweep;
                        return;
                    }
                    if (random2 == 1)
                    {
                        ResetPosition();

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
                    InternalCalls.AudioPlaySoundSFX("ADDING-MACHINE-ELECTRONIC_GEN-HDF-00040.wav", false, 0.3f);
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
            InternalCalls.SetSpriteImage(boss_idle_image);
            InternalCalls.SetSpriteAnimationIndex(0);
            idle_timer -=InternalCalls.GetDeltaTime();
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
            MoveToLocation(Boss_spawn_pos.x, Boss_spawn_pos.y);
            //InternalCalls.TransformSetPosition(Boss_spawn_pos.x, Boss_spawn_pos.y);
            InternalCalls.RigidBodySetForce(0, 0);
            CameraScript.CameraTargetZoom(0.5f, 1f);
        }

        
        // functions for the battle

        static private void FollowBeforeSmash()
        {

            if (!smash)
            {

                
                InternalCalls.SetSpriteImage(boss_smash_image);
                InternalCalls.SetSpriteAnimationIndex(1);
                int index = InternalCalls.GetCurrentAnimationEntity(InternalCalls.GetCurrentEntityID());
                InternalCalls.SetAnimationFrameIndex(7);
                InternalCalls.SetAnimationEntityPlaying(InternalCalls.GetCurrentEntityID(), false);
                /*                if (index >= 7)
                                {
                                    InternalCalls.SetAnimationEntityPlaying(InternalCalls.GetCurrentEntityID(), false);
                                }*/


                CameraScript.CameraTargetZoom(0.3f, 1f);
                SimpleVector2D pos = InternalCalls.GetTransformPosition();
                if(pos.x != PlayerScript.player_pos.x)
                {
                    pos.x = CameraScript.camera_pos.x;
                    //InternalCalls.TransformSetPosition(CameraScript.camera_pos.x, pos.y);
                }
                MoveToLocation(pos.x, Boss_spawn_pos.y);

                smash_timer -=InternalCalls.GetDeltaTime();
                if(smash_timer <= 0)
                {

                    smash = true;
                    smash_timer = smash_timer_set;
                }

            }
            else
            {
                InternalCalls.SetColliderOffset(0, -300);
                InternalCalls.SetAnimationFrameIndex(10);
                InternalCalls.SetAnimationEntityPlaying(InternalCalls.GetCurrentEntityID(), true);
                Smash();
                //SimpleVector2D pos = InternalCalls.GetTransformPosition();
                

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
                        PlayerScript.screen_flash_timer += 1.5f;
                        PlayerScript.is_colliding_enemy = true;
                    }

                    smash_timer = smash_timer_set;
                    smash = false;
                    InternalCalls.RigidBodySetBodyTypeEntity(6, InternalCalls.GetCurrentEntityID());
                    StateChanger();
                    InternalCalls.AudioPlaySoundSFX("DieSound.wav", false, 0.2f);
                    CameraScript.camera_shake_intensity = 3f;
                    CameraScript.camera_shake_duration = 0.2f;
                    InternalCalls.AudioPlaySoundSFX("EXPLOSION-LARGE_GEN-HDF-10849.wav", false, 0.54f);
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
                    return;

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

            SimpleVector2D pos=InternalCalls.GetTransformPosition();
            SimpleVector2D entity_pos = InternalCalls.GetTransformPositionEntity(entity);
            SimpleVector2D size= new SimpleVector2D(InternalCalls.GetTransformScaling().x*0.4f, InternalCalls.GetTransformScaling().y * 0.4f);
            SimpleVector2D entity_size= InternalCalls.GetTransformScalingEntity(entity);
            float rotation = InternalCalls.GetTransformRotationEntity(entity);

            // if the floor can be broken
            if (InternalCalls.CheckEntityCategory(entity, "BreakingPoint"))
            {
                //spawn particles

                //destroy
                InternalCalls.DestroyEntity(entity);
                for (int i = 0; i < 52; i++)
                {
                    DrawParticles(entity);
                }
                InternalCalls.AudioPlaySoundSFX("EXPLOSION-LARGE_GEN-HDF-10849.wav", false, 0.54f);
                return;
            }
            else
            {
                int new_entity = InternalCalls.CreateEntityPrefab("PlatformBossLevel2");
                InternalCalls.TransformSetPositionEntity(entity_pos.x, entity_pos.y, new_entity);
                InternalCalls.TransformSetScaleEntity(entity_size.x, entity_size.y, new_entity);
                InternalCalls.TransformSetRotationEntity(rotation, 0, new_entity);
                //destroy the old entity
                InternalCalls.DestroyEntity(entity);
                for(int i=0; i < 32; i++)
                {
                    DrawParticles(entity);
                }

                InternalCalls.AudioPlaySoundSFX("EXPLOSION-LARGE_GEN-HDF-10849.wav", false, 0.54f);

            }







            //old code to break accurately
            //if the boss is bigger
            /* if (size.x>entity_size.x)
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
            */
        }

        static private void SummonEnemy()
        {
            // CameraScript.CameraTargetZoom(0.3f, 2f); //zoom is hard to see so i remove for now
            SimpleVector2D pos = InternalCalls.GetTransformPositionEntity(InternalCalls.GetCurrentEntityID());
            Vector2D dir = new Vector2D(Boss_spawn_pos.x - pos.x, Boss_spawn_pos.y - pos.y);

            if (MathF.Abs(dir.x) > 50f || MathF.Abs(dir.y) > 50f)
            {
                MoveToLocation(Boss_spawn_pos.x, Boss_spawn_pos.y);
                return; // wait for boss to reach target position
            }
            
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
                    x, y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Boss Hand.txt"
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
            int side = (int)rnd.Next(0, 2); // random from 0 to 1
            if (!isSingingSpell) // render spawn position once only
            {
                float minx = (side == 0) ? -1000f : 1000f ; // 0 for left side, 1 for right side
                enemy_spawn_pos = PlayerScript.player_pos;
                // random left/right pos around player
                enemy_spawn_pos.x = enemy_spawn_pos.x + minx + 500f * (rnd.NextFloat() - 0.5f);
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
                    enemy_spawn_pos.x, enemy_spawn_pos.y, dir, size, size_scale, alpha, 0f, lifetime, speed, "Particle Boss Hand.txt"
                 );
            }
        }


        static private void LeftSweep()
        {
            InternalCalls.SetSpriteImage(boss_idle_image);
            // ensure the boss is at the defaulted position
            if (MoveToLocation(Boss_spawn_pos.x, Boss_spawn_pos.y)) { return; }

            //create all entities

            if (!sweeped)
            {

                //spawn line of projectiles
                int flipper = 1;
                for (int i = 0; i < number_of_bullets; i++)
                {
                    //flip flipper
                    flipper *= -1;
                    Vector2D bullet_pos=new Vector2D(PlayerScript.player_pos.x+500f/CameraScript.camera_zoom, PlayerScript.player_pos.y+ i*flipper*50*1.5f);
                    BossProjectile.CreateProjectile("Boss Projectile",180,180,0,bullet_pos.x,bullet_pos.y);
                }

                sweeped = true;
            }

            sweep_timer-=InternalCalls.GetDeltaTime();

            if (sweep_timer > 1.5 && sweep_timer < 2)
            {
                if (!play360audio)
                {
                    InternalCalls.AudioPlaySoundSFX("SCI-FI-WHOOSH_GEN-HDF-20863.wav", false, 0.2f);
                    play360audio = true;
                }

                BossProjectile.SetAllProjectileSpeed(40);
            }

            if (sweep_timer < 0)
            {
                play360audio = false;
                sweep_timer = sweep_timer_set;
                sweeped = false;
                current_state = BossStates.RightSweep;
            }


        }


        static private void RightSweep()
        {
            InternalCalls.SetSpriteImage(boss_idle_image);
            // ensure the boss is at the defaulted position
            if (MoveToLocation(Boss_spawn_pos.x, Boss_spawn_pos.y)) { return; }

            //create all entities

            if (!sweeped)
            {

                //spawn line of projectiles
                int flipper = 1;
                for (int i = 0; i < number_of_bullets; i++)
                {
                    //flip flipper
                    flipper *= -1;
                    Vector2D bullet_pos = new Vector2D(PlayerScript.player_pos.x - 500f / CameraScript.camera_zoom, PlayerScript.player_pos.y + i * flipper * 50 * 1.5f);
                    BossProjectile.CreateProjectile("Boss Projectile", 0, 0, 0, bullet_pos.x, bullet_pos.y);
                }

                sweeped = true;
            }

            sweep_timer -= InternalCalls.GetDeltaTime();

            if (sweep_timer > 1.5 && sweep_timer < 2)
            {
                if (!play360audio)
                {
                    InternalCalls.AudioPlaySoundSFX("SCI-FI-WHOOSH_GEN-HDF-20863.wav", false, 0.2f);
                    play360audio = true;
                }
                BossProjectile.SetAllProjectileSpeed(40);
            }

            if (sweep_timer < 0)
            {
                play360audio = false;
                sweep_timer = sweep_timer_set;
                sweeped = false;
                StateChanger();
            }


        }



        static private void SpikesSpawn(float left_or_right)
        {
            InternalCalls.SetSpriteImage(boss_idle_image);
            InternalCalls.SetSpriteAnimationIndex(0);
            // ensure the boss is at the defaulted position
            if (MoveToLocation(Boss_spawn_pos.x, Boss_spawn_pos.y)) { return; } 

            //create all entities

            if (!sweeped)
            {

                float flipper = 1;
                for (int i = 0; i < number_of_bullets; i++)
                {
                    //flip flipper
                    flipper *= -1;

                   Vector2D pos= new Vector2D(CameraScript.camera_pos.x - left_or_right*800f / CameraScript.camera_zoom, CameraScript.camera_pos.y+ 250 + i * 250 *flipper);

                    float dir = 90;
                    if (left_or_right == 1) { dir = 270; }
                    BossProjectile.CreateProjectile("Boss Projectile Spikes",dir+90, dir, 0f, pos.x, pos.y);
                }



                sweeped = true;
            }

            sweep_timer -= InternalCalls.GetDeltaTime();


            if (sweep_timer > 1 && sweep_timer < 1.5)
            {
                BossProjectile.SetAllProjectileSpeed(40);
                if (!play360audio)
                {
                    InternalCalls.AudioPlaySoundSFX("SCI-FI-WHOOSH_GEN-HDF-20863.wav", false, 0.2f);
                    play360audio = true;
                }

            }

            if (sweep_timer < 0)
            {

                sweep_timer = sweep_timer_set;
                sweeped = false;
                if (left_or_right == -1)
                {
                    play360audio = false;
                    InternalCalls.AudioPlaySoundSFX("ADDING-MACHINE-ELECTRONIC_GEN-HDF-00040.wav", false, 0.3f);
                    current_state = BossStates.ReturnSpikesSpawn;
                 
                }
                else
                {
                    play360audio = false;
                    current_state = BossStates.Smash;
                }
                
            }


        }


        static private void BossAOE()
        {
            // ensure the boss is at the center position
            if (MoveToLocation(random_x_location, random_y_location)) { return; };
            InternalCalls.SetSpriteImage(boss_idle_image);
            InternalCalls.SetSpriteAnimationIndex(0);

            //create all entities

            if (!sweeped)
            {
                if (index360 == 1)
                {
                    InternalCalls.AudioPlaySoundSFX("SCI-FI-LIGHT-SABRE_GEN-HDF-20726.wav", false, 0.2f); 
                }

                int flipper = 1;
                int i = index360;
                //flip flipper
                flipper *= -1;

                //create projectiles in a ring
                SimpleVector2D pos = InternalCalls.GetTransformPosition();
                float degree = i * 360 / number_of_bullets;
                Vector2D from_dir = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(degree));
                from_dir = from_dir.Multiply(800);
                BossProjectile.CreateProjectile("Boss Projectile", degree,degree,0f, pos.x + from_dir.x, pos.y + from_dir.y);


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
                if (!play360audio)
                {
                    play360audio=true;
                    InternalCalls.AudioPlaySoundSFX("SCI-FI-WHOOSH_GEN-HDF-20863.wav", false, 0.2f);

                }
                BossProjectile.SetAllProjectileSpeed(40f);
 
            }

            if (sweep_timer < 0)
            {
                play360audio = false;
                sweep_timer = sweep_timer_set/1.4f; //this is for the return to be shorter
                sweeped = false;
                InternalCalls.AudioPlaySoundSFX("SCI-FI-WHOOSH_GEN-HDF-20863.wav", false, 0.2f); // play again for return
                current_state = BossStates.ReturnBoss360;
            }


        }

        static private void ReverseAOE()
        {

            InternalCalls.SetSpriteImage(boss_idle_image);
            InternalCalls.SetSpriteAnimationIndex(0);

            if (!sweeped)
            {
                BossProjectile.ReverseAllProjectile();
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
            float move_speed = 20f;
            SimpleVector2D current_pos = InternalCalls.GetTransformPosition();
            SimpleVector2D directed = new SimpleVector2D(directed_x, directed_y);
            Vector2D direction = new Vector2D(directed.x - current_pos.x,
                                              directed.y - current_pos.y);
            if (Vector2D.Distance(Vector2D.FromSimpleVector2D(current_pos), Vector2D.FromSimpleVector2D(directed)) > 20f)
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
            /*pos.y += 20f;
            InternalCalls.TransformSetPosition(pos.x, pos.y);*/
            MoveToLocation(pos.x, 2000f);

            if (boss_move_away_timer > 0f)
            {
                boss_move_away_timer -= InternalCalls.GetDeltaTime();
                return;
            }

            if (InternalCalls.GetTransformPosition().y < 1400f)
            {
                return; // boss havent reach top position, do not create clap hand
            }

            if (!is_clapping)
            {
                // create left hand
                int left_hand = InternalCalls.CreateEntityPrefab("BossLeftHand");
                is_clapping = true;
            }

            /*if (BossClap.is_destoryed)
            {
                StateChanger();
            }*/
        }

        static private void GoNextPhase()
        {
            BossProjectile.DestroyAllProjectiles();
            InternalCalls.SetSpriteImage(boss_idle_image);
            InternalCalls.SetSpriteAnimationIndex(0);
            InternalCalls.TransformSetPositionEntity(0, 500, PlayerScript.PLAYER_ID);
            //InternalCalls.SetEntityGravityScale(0, PlayerScript.PLAYER_ID);
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
                    if (InternalCalls.RigidBodyGetBodyTypeEntity(i)==0)//if static
                    {
                        InternalCalls.RigidBodySetBodyTypeEntity(1, i);
                        //BossBackground.phase_move -= 20f;
                    }
                }

                sweeped = true; //cleanup all the floors
                sweep_timer = 3f; //1s to destroy all entities
            }
            sweep_timer -= InternalCalls.GetDeltaTime();
            if (sweep_timer <= 0f)
            {

                for (int i = -2; i< 3; i++)
                {
                    int entity = InternalCalls.CreateEntityPrefab("Floor");
                    InternalCalls.TransformSetPositionEntity(0 + i * 900, -1000,entity);
                }
                


                sweep_timer = sweep_timer_set;
                sweeped=false;
                boss_phase = 1;
                boss_hp = boss_max_hp;
                temp_boss_hp = boss_max_hp; // reset the white bg bar
                Boss_spawn_pos.x = 0;
                Boss_spawn_pos.y = -500;
                current_state = BossStates.Clap;
                PlayerScript.Health = PlayerScript.Max_Health;
                PlayerScript.player_cam_y_offset = 300f;
                PlayerScript.invulnerable = false;
                CameraScript.StopCameraPan();
            }

        }

        static private void DrawParticles(int entity_id)
        {
            Vector2D pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(entity_id));
            uint r = (uint)(12314 * InternalCalls.GetRandomFloat());
            MyRandom my_rand = new MyRandom(r);
            int num = (int)(3f * my_rand.NextFloat());
            int top_layer = InternalCalls.GetTopLayer();
            Vector2D scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScalingEntity(entity_id));
            for (int i = 0; i < num; i++)
            {

                float rand = InternalCalls.GetRandomFloat();
                float dir = 270 + 30 * (rand - 0.5f);

                rand = InternalCalls.GetRandomFloat();
                float size = 20f + 10f * rand;

                rand = InternalCalls.GetRandomFloat();
                float size_scale = -10 * rand;

                rand = InternalCalls.GetRandomFloat();
                float alpha = 0.5f + 0.5f * rand;

                rand = InternalCalls.GetRandomFloat();
                float lifetime = 0.3f + 1f * rand;

                rand = InternalCalls.GetRandomFloat();
                float speed = 1000f + 500f * rand;

                rand = InternalCalls.GetRandomFloat();
                float x = pos.x + scaling.x / 2f * (rand - 0.5f);

                rand = InternalCalls.GetRandomFloat();
                float y = pos.y + scaling.y / 2f * (rand - 0.2f);

                rand = InternalCalls.GetRandomFloat();
                float rotation = rand * 360;

/*                InternalCalls.GameSpawnParticleExtraLayer(
                    x, y, dir, size, size_scale, alpha, 0.5f, lifetime, speed, "ParticleSpike.txt",
                (1f, 1f, 1f), top_layer);*/
                InternalCalls.GameSpawnParticleExtraFramesRotate(x, y, dir, size, size_scale, alpha, 0.5f, lifetime, speed, "ParticleSpike.txt", 0, 0, 20f);

            }
        }


        static void BossHurtSound()
        {
            float num = InternalCalls.GetRandomFloat();
            float volume = 0.4f;
            if (num <= 0.2f)
            {
                InternalCalls.AudioPlaySoundSFX("Boss_Hurt_1.wav", false, volume);
                return;
            }
            if (num <= 0.4f)
            {
                InternalCalls.AudioPlaySoundSFX("Boss_Hurt_2.wav", false, volume);
                return;
            }
            if (num <= 0.6f)
            {
                InternalCalls.AudioPlaySoundSFX("Boss_Hurt_3.wav", false, volume);
                InternalCalls.AudioPlaySoundSFX("BossGrunt.wav", false, volume);
                return;
            }
            if (num <= 0.8f)
            {
                InternalCalls.AudioPlaySoundSFX("Boss_Hurt_4.wav", false, volume);
                return;
            }            
            if (num <= 1f)
            {
                InternalCalls.AudioPlaySoundSFX("Boss_Hurt_5.wav", false, volume);
                return;
            }
        }



    } // class end here
}

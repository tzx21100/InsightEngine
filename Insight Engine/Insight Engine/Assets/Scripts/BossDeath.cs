using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System;
namespace IS
{
    class BossDeath
    {
        static private float pause_shake_timer = 2f;

        static private float death_animation_timer_set = 2f;
        static private float death_animation_timer = 2f;
        static SimpleImage boss_death_image;

        static private bool is_starting = true;
        static private bool is_pausing = false;
        static private bool is_shining = false;
        static private bool is_breaking = false;
        static private bool is_destoryed = false;
        static private bool randomize_breaking_time = false;

        static private int Boss_ID;

        // target pos
        static private Vector2D target_pos = new Vector2D(0f, -100f);
        static private Vector2D current_pos = new Vector2D(0f, 0f);

        static private float speed = 15f;

        // Invis Wall
        static public List<int> block_list = new List<int>();
        static public List<int> rot_list = new List<int>();
        static private int max_block_size = 16;
        static private int block_size = 8;

        // invis Wall disappear timer
        static private float break_block_timer_set = 0f;
        static private float break_block_timer = 0f;

        static private float rot = 0f;

        // camera shake info
        static private float shake_intensity = 0f;

        static private float burst_timer = 0f;
        static private float burst_interval = 0.8f;

        // fade white bg
        static private float fade_out_timer = 0f;
        static private float white_bg_timer = 2f;

        static public void Init()
        {
            Boss_ID = InternalCalls.GetCurrentEntityID();
            pause_shake_timer = 2f;
            death_animation_timer = death_animation_timer_set;
            boss_death_image = InternalCalls.GetSpriteImage("BossDeath 6R4C.png");

            is_starting = true;
            is_pausing = false;
            is_shining = false;
            is_breaking = false;
            is_destoryed = false;
            randomize_breaking_time = false;
            break_block_timer = 0f;

            burst_timer = 0f;
            burst_interval = 0.8f;
            shake_intensity = 0f;
            fade_out_timer = 0f;
            white_bg_timer = 2f;
            InternalCalls.ResetAnimations();
            InternalCalls.CreateAnimationFromSprite(6, 4, death_animation_timer_set); // dead
            InternalCalls.SetAnimationEntityPlaying(Boss_ID, false);

            target_pos = new Vector2D(0f, -100f);
            current_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(Boss_ID));

            // init light block walls
            block_size = 8;
            InitInvisibleWalls();

            // turn off the light initially
            InternalCalls.SetLightIntensityEntity(Boss_ID, 0f);
        }

        static public void Update()
        {
            Boss_ID = InternalCalls.GetCurrentEntityID();
            current_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(Boss_ID));

            if (is_starting) {
                Vector2D dir = new Vector2D(target_pos.x - current_pos.x, target_pos.y - current_pos.y);
                if (MathF.Abs(dir.x) < 50f && MathF.Abs(dir.y) < 50f)
                {
                    is_starting = false;
                    is_pausing = true;
                }

                dir = dir.Normalize();

                current_pos.x += dir.x * speed;
                current_pos.y += dir.y * speed;
                InternalCalls.TransformSetPositionEntity(current_pos.x, current_pos.y, Boss_ID);
                CameraScript.CameraTargetZoom(0.3f, 1f);
                return;
            }
            if (is_pausing)
            {
                // turn on the lights and move camera
                InternalCalls.SetLightIntensityEntity(Boss_ID, 0.4f);
                CameraScript.CameraTargetZoom(0.4f, 1f);
                CameraScript.CameraPanToTime(target_pos, 15f, 10f);
                if (pause_shake_timer > 0f)
                {
                    pause_shake_timer -= InternalCalls.GetDeltaTime();

                    // shake camera
                    PausingCameraShake();

                    return;
                }
                else
                {
                    is_pausing = false; // proceed to next stage
                    is_shining = true;
                }
            }

            /*if (!is_breaking)
            {
                InternalCalls.SetAnimationEntityPlaying(Boss_ID, false);
                return;
            }*/
            StartShining();
            StartBreaking();
            FadeToWhite();

            if (is_shining || is_breaking)
            {
                burst_timer += InternalCalls.GetDeltaTime();
                if (burst_timer >= burst_interval)
                {
                    burst_timer = 0f;
                    burst_interval -= 0.1f;
                    if (burst_interval <= 0.2f)
                        burst_interval = 0.2f;
                    BossShakingParticles();
                }
            }    
        }
        
        static public void CleanUp(){

        }

        static private void BossPause()
        {
            
        }

        static private void StartShining()
        {
            if (is_shining)
            {

                
                if (!randomize_breaking_time)
                {
                    // render shaking particles

                        
                    MyRandom rand = new MyRandom((uint)(12924818 * InternalCalls.GetRandomFloat()));
                    // determine remove random block
                    int index = -1;
                    if (block_size > 1)
                    {
                        index = (int)rand.Next(0, (uint)block_size - 1);
                    }
                    else
                    {
                        index = 0;
                        // destory all blocks
                        foreach (var key in rot_list)
                            InternalCalls.DestroyEntity(key);

                        rot_list.Clear();
                        is_shining = false;
                    }

                    if (block_size < 4)
                    {
                        is_breaking = true;
                    }
                    // random timer
                    if (block_size == 8)
                    {
                        break_block_timer = 0f;
                    }
                    else
                    {
                        break_block_timer = rand.NextFloat();
                    }

                    InternalCalls.DestroyEntity(block_list[index]);
                    block_list.Remove(block_list[index]);
                    block_size--;
                    block_list.Sort();
                    /*if (block_list.Contains(index))
                    {
                        
                    }
                    else
                    {
                        // error
                        Console.WriteLine("block not found in list");
                    }*/
                    
                    randomize_breaking_time = true;

                }
                else
                {
                    break_block_timer -= InternalCalls.GetDeltaTime();
                    if (break_block_timer < 0f)
                    {
                        randomize_breaking_time = false;
                    }
                }
                // rotate the light wall
                RotateInvisibleWalls();

                // camera shake
                ShiningCameraShake();
            }
        }

        static private void StartBreaking()
        {
            // start dead animation
            if (is_breaking) {
                InternalCalls.SetAnimationEntityPlaying(Boss_ID, true);
                CameraShake();
                death_animation_timer -= InternalCalls.GetDeltaTime();

                //shake camera
                CameraScript.CameraShake(2f);
                CameraScript.camera_shake_duration = 0.2f;
                CameraScript.camera_shake_intensity = 1f;

                if (death_animation_timer < 1f && death_animation_timer > 0f)
                {
                    fade_out_timer += InternalCalls.GetDeltaTime();
                }

                if (death_animation_timer < 0f)
                {
                    if (InternalCalls.GetCurrentAnimationEntity(Boss_ID) == 23) // last animiation index is 23
                    {
                        InternalCalls.SetAnimationEntityPlaying(Boss_ID, false);
                    }

                    InternalCalls.SetSpriteAlpha(0f);
                    //InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());

                    // render destory particle

                    // create the red n blue fragments
                    if (!is_destoryed) {
                        int blue = InternalCalls.CreateEntityPrefab("FinalBlueFragment");
                        int red = InternalCalls.CreateEntityPrefab("FinalRedFragment");
                        //-1800 1800 y -700

                        //setting positions
                        InternalCalls.TransformSetPositionEntity(-1800, -700, red);
                        InternalCalls.TransformSetPositionEntity(1800, -700, blue);
                        InternalCalls.TransformSetPositionEntity(0, -700, PlayerScript.PLAYER_ID);

                        is_breaking = false;
                        is_destoryed = true;

                    }
                    
                }
                
            }
            else
            {
                InternalCalls.SetAnimationEntityPlaying(Boss_ID, false);
            }

            
        }

        static private void InitInvisibleWalls()
        {
            Vector2D center = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(Boss_ID));
            for (int i = 0; i < max_block_size; i++)
            {
                int each_angle = 360 / max_block_size * i;
                Vector2D dir = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(each_angle + 90));
                dir = dir.Multiply(10f);

                int block = InternalCalls.CreateEntityPrefab("Light Wall");
                InternalCalls.TransformSetPositionEntity(center.x + dir.x, center.y + dir.y, block);
                InternalCalls.TransformSetRotationEntity(each_angle, 0f, block);
                if (i % 2 == 0)
                    block_list.Add(block);
                rot_list.Add(block);
            }
        }

        static private void RotateInvisibleWalls()
        {
            rot += InternalCalls.GetDeltaTime() * 20f;
            if (rot_list.Count == 0)
                return;
            Vector2D center = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(Boss_ID));
            for (int i = 0; i < max_block_size; i++)
            {
                float each_angle = (360 / max_block_size * i) + rot;
                Vector2D dir = Vector2D.DirectionFromAngle(CustomMath.DegreesToRadians(each_angle + 90f));
                dir = dir.Multiply(10f);

                int block = rot_list[i];
                InternalCalls.TransformSetPositionEntity(center.x + dir.x, center.y + dir.y, block);
                InternalCalls.TransformSetRotationEntity(each_angle, 0f, block);
                //block_list.Add(block);
            }
        }

        static private void PausingCameraShake()
        {
            CameraScript.CameraShake(2f);
            CameraScript.camera_shake_intensity = 0.5f;
            CameraScript.camera_shake_duration = 2f;
        }

        static private void ShiningCameraShake()
        {
            CameraScript.CameraShake(2f);
            CameraScript.camera_shake_intensity = 1f;
            CameraScript.camera_shake_duration = 2f;
        }

        static private void CameraShake()
        {
            shake_intensity += InternalCalls.GetDeltaTime();
            CameraScript.CameraShake(2f);
            CameraScript.camera_shake_intensity = (0.5f + shake_intensity) / 2f;
            CameraScript.camera_shake_duration = 5f;
        }

        static private void BossShakingParticles()
        {
            // load bleeding particles
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            int particle_count = (int)(my_rand.Next(80, 121)); // random from 80 to 100 particles
            SimpleVector2D pos = InternalCalls.GetTransformPositionEntity(Boss_ID);
            SimpleVector2D scaling = InternalCalls.GetTransformScalingEntity(Boss_ID);
            for (int i = 0; i < particle_count; i++)
            {
                float rand = my_rand.NextFloat();
                float dir = 360 * rand;

                rand = my_rand.NextFloat();
                float size = 20 + 20f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = 150f + 50 * rand; // pariticles going nigger

                rand = my_rand.NextFloat();
                float alpha = rand; // 0 to 1

                rand = my_rand.NextFloat();
                float lifetime = 1.5f + 0.5f * rand; // 0.5s to 0.8s

                rand = my_rand.NextFloat();
                float speed = 800f + 500f * rand;

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

        static private void FadeToWhite()
        {
            //Console.WriteLine(fade_out_timer);
            if (is_destoryed)
            {
                if (white_bg_timer > 0f)
                {
                    white_bg_timer -= InternalCalls.GetDeltaTime();
                }
                else
                {
                    fade_out_timer -= InternalCalls.GetDeltaTime();

                    if (fade_out_timer < -2f)
                    {
                        CameraScript.StopCameraPan();
                    }

                    if (fade_out_timer < 0f)
                    {
                        InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());
                        CameraScript.CameraPanToTime(PlayerScript.player_pos, 15f, 2f);
                    }
                }
                
            }
            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 7000, 7000, 1, 1, 1, fade_out_timer, InternalCalls.GetTopLayer());
        }
    }
}

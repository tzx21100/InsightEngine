using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace IS
{



    class BossIntro
    {

        static private float grow_timer = 2f;
        static private float shouting_timer = 2f;
        static private float speed = 35f;

        static private int current_phase = 0;

        static private float fade_out_timer = 1f;


        static int other_hand;


        //images
        static SimpleImage boss_smash_image;
        static SimpleImage boss_clap_image;
        static SimpleImage boss_idle_image;
        static SimpleImage boss_grab_image;
        static SimpleImage boss_death_image;


        static private List<float> x_list = new List<float>();
        static private List<float> y_list = new List<float>();

        // particles
        static private float shouting_particles_timer_set = 0.5f;
        static private float shouting_particles_timer = 0.5f;


        static public void Init(){

            //stop sounds
            InternalCalls.AudioStopAllSounds();

            //create the other hand
            other_hand = InternalCalls.CreateEntityPrefab("BossIntroOther");

            fade_out_timer = 2f;

            current_phase = 0;
            grow_timer = 2f;
            shouting_timer = 2f;

            boss_idle_image = InternalCalls.GetSpriteImage("BossIdle 4R4C.png");
            boss_smash_image = InternalCalls.GetSpriteImage("BossSmash 5R4C.png");
            boss_clap_image = InternalCalls.GetSpriteImage("BossClap 6R4C.png");
            boss_grab_image = InternalCalls.GetSpriteImage("BossGrab 6R4C.png");
            boss_death_image = InternalCalls.GetSpriteImage("BossDeath 6R4C.png");


            x_list.Clear();
            x_list.Add(-4000);
            x_list.Add(4000);
            x_list.Add(-4000);
            x_list.Add(4000);

            y_list.Clear();
            y_list.Add(-2000);
            y_list.Add(2000);
            y_list.Add(2000);
            y_list.Add(-2000);

            // paritcles
            shouting_particles_timer = shouting_particles_timer_set;

        }

        static public void Update(){

            SimpleVector2D scale = InternalCalls.GetTransformScaling();
            SimpleVector2D pos = InternalCalls.GetTransformPosition();
            InternalCalls.TransformSetScaleEntity(-scale.x, scale.y, other_hand);
            InternalCalls.TransformSetPositionEntity(-pos.x, pos.y, other_hand);
            InternalCalls.SetSpriteAlphaEntity(InternalCalls.GetSpriteAlphaEntity(InternalCalls.GetCurrentEntityID()), other_hand);

            //phase 0 coming out 
            switch (current_phase)
            {
                case 0:
                    FadeOutFromWhite();
                    break;

                case 1:
                    GrowAndAppear();

                    break;
                case 2:
                    FlyAround();
                    break;
                case 3:
                    LastGrowAndAppear();
                    break;
                case 4:
                    if (!MoveToLocation(0,1304.95f))
                    {
                        int boss=InternalCalls.CreateEntityPrefab("Boss");
                        InternalCalls.DestroyEntity(other_hand);
                        InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());
                        //InternalCalls.LoadScene("Assets/Scenes/BossLevel.insight");
                    }
                    break;


            }


        }
        
        static public void CleanUp(){

        }

        static private void FadeOutFromWhite()
        {
            fade_out_timer -= InternalCalls.GetDeltaTime();
            InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 7000, 7000, 1, 1, 1, fade_out_timer, InternalCalls.GetTopLayer());
            if (fade_out_timer <= 0)
            {
                current_phase++;
            }
        }


        static private void GrowAndAppear()
        {

            CameraScript.CameraTargetZoom(0.30f, 0.6f);

            if (grow_timer >0f)
            {
                grow_timer -= InternalCalls.GetDeltaTime();
                SimpleVector2D scale = InternalCalls.GetTransformScaling();
                InternalCalls.SetSpriteAlpha((2 - grow_timer) * 0.5f);
                InternalCalls.TransformSetScale(scale.x * 1.01f, scale.y * 1.01f);

                CameraScript.CameraShake(2f);
                CameraScript.camera_shake_intensity = 1f;
                CameraScript.camera_shake_duration = 5f;
                return;
            }

            
            current_phase++;


        }   
        
        static private void LastGrowAndAppear()
        {

            CameraScript.CameraTargetZoom(0.30f, 0.6f);

            if (grow_timer > 0f)
            {
                grow_timer -= InternalCalls.GetDeltaTime();
                SimpleVector2D scale = InternalCalls.GetTransformScaling();
                InternalCalls.SetSpriteAlpha((2 - grow_timer) * 0.5f);
                InternalCalls.TransformSetScale(scale.x * 1.01f, scale.y * 1.01f);
                return;
            }

            if (shouting_timer > 0f)
            {
                shouting_timer -= InternalCalls.GetDeltaTime();

                CameraScript.CameraShake(2f);
                CameraScript.camera_shake_intensity = (2f - shouting_timer) * 3f;
                CameraScript.camera_shake_duration = 2f;

                if (shouting_particles_timer > 0f)
                {
                    shouting_particles_timer -= InternalCalls.GetDeltaTime();
                }
                else { 
                    shouting_particles_timer = shouting_particles_timer_set;
                }

                if (shouting_particles_timer > 0.45f)
                {
                    RenderShoutingParticles();
                }

                return;
            }


            current_phase++;


        }

        static private void RenderShoutingParticles()
        {
            int id = InternalCalls.GetCurrentEntityID();
            // load particles
            MyRandom my_rand = new MyRandom((uint)(129248189 * InternalCalls.GetRandomFloat()));
            //int particle_count = (int)(my_rand.Next(50, 101)); // random from 80 to 100 particles

            SimpleVector2D pos = InternalCalls.GetTransformPositionEntity(id);
            SimpleVector2D scaling = InternalCalls.GetTransformScalingEntity(id);
            for (int i = 0; i < 1; i++)
            {
                float rand = my_rand.NextFloat();
                float dir = 360 * rand;

                rand = my_rand.NextFloat();
                float size = 2500 + 30f * rand; // initial size

                rand = my_rand.NextFloat();
                float size_scale = 5000f + 30 * rand; // pariticles going nigger

                rand = my_rand.NextFloat();
                float alpha = 0.5f + 0.3f * rand; // 0 to 1

                rand = my_rand.NextFloat();
                float lifetime = 1.5f + 0.5f * rand; // 0.5s to 0.8s

                rand = my_rand.NextFloat();
                //float speed = 1000f + 500f * rand;
                float speed = 0;

                //rand = my_rand.NextFloat();
                float x = pos.x - 350f;
                //float x = enemy_pos.x;

                //rand = my_rand.NextFloat();
                float y = pos.y;

                InternalCalls.GameSpawnParticleExtraFramesFull(
                    x, y, dir, size, size_scale, alpha, 0f, lifetime, speed,
                    "Particle Boss Die.txt", 1, 1, 5f, 2, lifetime, 10f,
                    InternalCalls.GetTopLayer(), (1f, 1f, 1f)
                 );
            }
        }

        static private void FlyAround()
        {
            CameraScript.CameraTargetZoom(0.50f, 0.6f);
            bool ok = MoveToLocation(x_list[0], y_list[0]);
            if (!ok)
            {
                x_list.Remove(x_list[0]);
                y_list.Remove(y_list[0]);
                if (x_list.Count <= 0 || y_list.Count <= 0)
                {
                    InternalCalls.TransformSetPosition(247, 0);
                    InternalCalls.TransformSetScale(504.84f, 294.82f);
                    grow_timer = 2f;
                    current_phase++;
                    return;
                }
            }




        }
        static private bool MoveToLocation(float directed_x, float directed_y)
        {
            InternalCalls.RigidBodySetBodyTypeEntity(6, InternalCalls.GetCurrentEntityID());
            float move_speed = 150f;
            SimpleVector2D current_pos = InternalCalls.GetTransformPosition();
            SimpleVector2D directed = new SimpleVector2D(directed_x, directed_y);
            Vector2D direction = new Vector2D(directed.x - current_pos.x,
                                              directed.y - current_pos.y);
            if (Vector2D.Distance(Vector2D.FromSimpleVector2D(current_pos), Vector2D.FromSimpleVector2D(directed)) > move_speed)
            {


                direction = direction.Normalize();
                InternalCalls.TransformSetPosition(current_pos.x + direction.x * move_speed, current_pos.y + direction.y * move_speed);


                return true;
            }

            return false;
        }

    }
}

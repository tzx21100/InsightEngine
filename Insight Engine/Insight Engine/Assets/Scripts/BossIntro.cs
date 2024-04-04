using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
namespace IS
{



    class BossIntro
    {

        static private float grow_timer = 2f;
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


        static public void Init(){

            //create the other hand
            other_hand = InternalCalls.CreateEntityPrefab("BossIntroOther");

            fade_out_timer = 2f;

            current_phase = 0;
            grow_timer = 2f;

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
                    GrowAndAppear();
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
                return;
            }
            current_phase++;


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

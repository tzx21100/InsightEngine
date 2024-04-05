/*!
 * \file FlightAffector.cs
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This source file is meant for the player movement and camera.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace IS
{

    class FlightAffector
    {

        static SimpleImage wind_vfx = InternalCalls.GetSpriteImage("wind vfx.png");
        static float timer = 0.2f;
        static float timer_set = 0.2f;
        static int index = 0;
        static int[,] index_array= new int[64,64];
        static float[,] index_random= new float[64,64];
        static Dictionary<(int, int), (float,float)> indexToFloatMap = new Dictionary<(int, int), (float,float)>();

        static private Vector2D fog_pos = new Vector2D(0, 0);
        static private Vector2D fog_scaling = new Vector2D(0, 0);
        static private float flying_fog_render_timer_set = 0.2f;
        static private float flying_fog_render_timer = 0.2f;

        static public void Init(){

            FastScene.Init();

            CaveBackGroundRaw.Init();
            PlayerScript.isDead = false;

            CameraScript.CameraTargetZoom(0.4f, 0.1f);
            for (int i = -32; i < 32; i++)
            {
                for (int j = -32; j < 32; j++)
                {
                    index_array[i + 32, j + 32] = (int)(InternalCalls.GetRandomFloat() * 6);
                    index_random[i + 32, j + 32] = (InternalCalls.GetRandomFloat() * 400);
                    indexToFloatMap[(i + 32, j + 32)] = (InternalCalls.GetRandomFloat(), InternalCalls.GetRandomFloat()*0.1f);
                }
            }
            flying_fog_render_timer = flying_fog_render_timer_set;

            PlayerScript.collection_count = 0; // reset in flight level
        }

        static public void Update()
        {
            FastScene.vidUpdate();

            if (PlayerScript.isDead)
            {
                NarratorVoices.NarrateDeath();
                InternalCalls.LoadScene("Assets/Scenes/FlightLevel.insight");
            }

            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);
            bool colliding = false;

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                colliding = true;
            }

            SimpleVector2D pos= InternalCalls.GetTransformPositionEntity(PlayerScript.PLAYER_ID);
            InternalCalls.TransformSetPosition(pos.x, pos.y);

            if (PlayerScript.flight_held && colliding)
            {
                if (PlayerScript.isGrounded)
                {
                    InternalCalls.TransformSetPositionEntity(pos.x + MathF.Sign(-PlayerScript.trans_scaling.x) * 0f, pos.y + 50f, PlayerScript.PLAYER_ID);
                }
                PlayerScript.AddForcesToPlayer(0, 150, 0.1f);
            }

            // update fog scaling base on player
            fog_scaling = new Vector2D(MathF.Abs(PlayerScript.trans_scaling.x / 1.5f), PlayerScript.trans_scaling.y / 10f);

            // render flying gas
            DrawFlyingFogs(colliding);

            float size = 1200f;
            float ratio_inbetween = 1f; //more less gap inbetwen
            float speed = 0.1f;
            float squash_y = 400f;
            float squash_x = 0f;


            timer_set = speed;
            timer -= InternalCalls.GetDeltaTime();
            if (timer < 0f)
            {
                index++;
                if (index > 6) { index = 0; }

                // to randomize every single index
/*                for (int i = -32; i < 32; i++)
                {
                    for (int j = -32; j < 32; j++)
                    {
                        var key = (i + 32, j + 32); // Create the key

                        float t1 = indexToFloatMap[key].Item1;
                        float t2 = indexToFloatMap[key].Item2;
                        // Subtracting InternalCalls.GetDeltaTime() from the first item of the tuple
                        float newItem1 = t1 - InternalCalls.GetDeltaTime();
                        // Keeping the second item unchanged
                        float newItem2 = t2;

                        // Creating a new tuple with the updated value and assigning it back to the dictionary
                        indexToFloatMap[key] = (newItem1, newItem2);

                        if (newItem1 <= 0)
                        {
                            index_array[i + 32, j + 32]++;
                            if (index_array[i + 32, j + 32] > 6) { index_array[i + 32, j + 32] = 0; }
                            newItem1 = newItem2;
                            indexToFloatMap[key] = (newItem1, newItem2);
                        }

                    }
                }
*/

                timer = timer_set;
            }



            for (int i=-32; i< 32;i++) {
                for (int j = -32; j < 32; j++)
                {
                    
                    InternalCalls.DrawImageExtraAt(0, index, 1, 7, new SimpleVector2D(i * (size - squash_x) / ratio_inbetween, j * (size - squash_y)),
            0f, new SimpleVector2D(size - squash_x, size - squash_y), wind_vfx, 0.9f, InternalCalls.GetTopLayer() - 4);

                    InternalCalls.DrawImageExtraAt(0, index, 1, 7, new SimpleVector2D(i * (size - squash_x + index_random[i + 32, j + 32]) / ratio_inbetween, j * (size - squash_y)),
0f, new SimpleVector2D(size - squash_x+ index_random[i + 32, j + 32], size - squash_y), wind_vfx, 0.7f, InternalCalls.GetTopLayer() - 4);
                    InternalCalls.DrawImageExtraAt(0, index, 1, 7, new SimpleVector2D(i * (size - squash_x - index_random[i + 32, j + 32]) / ratio_inbetween, j * (size - squash_y)),
0f, new SimpleVector2D(size - squash_x + index_random[i + 32, j + 32], size - squash_y), wind_vfx, 0.5f, InternalCalls.GetTopLayer() - 4);

                    // for random insertions inbetween

                    /*                        InternalCalls.DrawImageExtraAt(0, index, 1, 7, new SimpleVector2D(i * (size - squash_x) / 2 / ratio_inbetween, j * (size - squash_y)),
                    0f, new SimpleVector2D(size - squash_x, size - squash_y), wind_vfx, 1f, InternalCalls.GetTopLayer() - 4);*/






                    /*                InternalCalls.DrawNonEnityAnimationSet(InternalCalls.GetDeltaTime(), new SimpleVector2D(i * size / ratio_inbetween, j * size),
                            0f, new SimpleVector2D(size, size), wind_vfx, 1f, InternalCalls.GetTopLayer() - 4, 1, 7, speed);*/


                    // InternalCalls.GameSpawnParticleExtraFramesFull(i * size / ratio_inbetween, j * size, 0f, size, 0f, 1f, 1f, 1f, 0f, "ParticleWind.txt", (1f, 1f, 1f), 3, 7, 1);



                }

            }



            //cave backgrond
/*            CaveBackGroundRaw.Update();*/

            
        }







        static public void CleanUp(){

        }

        static private void DrawFlyingFogs(bool check_colliding)
        {
            if (TransitionToBossLevel.is_transiting)
            {
                return; // if it is transiting, do not render the fogs
            }

            if (PlayerScript.flight_held && check_colliding)
            {
                if (flying_fog_render_timer > 0f)
                {
                    flying_fog_render_timer -= InternalCalls.GetDeltaTime();

                    if (flying_fog_render_timer > 0.13f)
                    {
                        // fog position follows player position
                        fog_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPositionEntity(PlayerScript.entity_feet));

                        // draw flying fogs under player
                        FogCover.fog_position = fog_pos;
                        FogCover.fog_width = fog_scaling.x;
                        FogCover.fog_height = fog_scaling.y;
                        FogCover.fog_density = 20f;
                        FogCover.fog_size = 50f;
                        FogCover.fog_timer_set = 0.001f;
                        FogCover.random_fog_life_timer_set = 0f;
                        FogCover.SpawnFog();
                    }

                }
                else
                {
                    flying_fog_render_timer = flying_fog_render_timer_set;
                }

                
            }
            else
            {
                // move the fogs away
            }
        }

    }




}

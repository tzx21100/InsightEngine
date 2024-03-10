/*!
 * \file SpikeScript.cs
 * \author  Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 12-03-2024
 * \brief
 * This source file is meant for the player movement and camera.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

using System.Runtime.CompilerServices;
using System;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.ComponentModel;
namespace IS
{
    class Trampoline
    {
        //static private float 
        static private float total_shining_duration = 0.5f;
        static private float highlight_timer_duration = 0.2f;
        static private float highlight_timer = 0.2f;

        static private bool IsTriggered = false;
        static public void Init()
        {

        }

        static public void Update()
        {

            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if(array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                //Vector2D vel = PlayerScript.player_vel;
                Vector2D pos = PlayerScript.player_pos;
                //Vector2D impluse = new Vector2D(vel.x, -vel.y*5f);
                //PlayerScript.isGrounded = false;
                InternalCalls.TransformSetPositionEntity(pos.x + MathF.Sign(-PlayerScript.trans_scaling.x) * 0f, pos.y + 50f, PlayerScript.PLAYER_ID); ;
                /*InternalCalls.RigidBodySetVelocityEntity(impluse.x, 2000f, PlayerScript.PLAYER_ID);*/
                PlayerScript.AddForcesToPlayer(0f, 1000f, 0.5f);

                //IsTriggered = true;
                
                
            }
            if (IsTriggered)
            {
                // play effect
                total_shining_duration -= InternalCalls.GetDeltaTime();
                if (total_shining_duration > 0f)
                {
                    HighLightPlatform();
                }
                else
                {
                    total_shining_duration = 0.5f;
                    InternalCalls.SetAnimationAlpha(1f);
                    IsTriggered = false;
                }
            }

            /*foreach (var key in Enemy.enemies.Keys)
            {
                if (array.FindIndex(key) != -1)
                {
                    Enemy.enemies[key].health= 0;
                }
            }*/


        }

        static public void CleanUp()
        {

        }

        static private void HighLightPlatform()
        {
            highlight_timer -= InternalCalls.GetDeltaTime();
            if (highlight_timer > 0.1f)
            {
                InternalCalls.SetAnimationAlpha(0.5f);
            }
            else if (highlight_timer > 0f && highlight_timer < 0.1f)
            {
                InternalCalls.SetAnimationAlpha(1f);
            }
            else
            {
                highlight_timer = highlight_timer_duration;
            }
        }

    }
}

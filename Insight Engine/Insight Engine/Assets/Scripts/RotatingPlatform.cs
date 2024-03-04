/*!
 * \file SpikeScript.cs
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

using System.Runtime.CompilerServices;
using System;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.ComponentModel;
namespace IS
{
    class RotatingPlatform
    {
        static Vector2D pos = new Vector2D(0f, 0f);
        static Vector2D scaling = new Vector2D(0f, 0f);
        static Vector2D offset = new Vector2D(0f, 0f);
        static Vector2D pivot = new Vector2D(0f, 0f);
        static float angle = 0f;
        static float rotating_speed = 25f; 

        static bool IsTriggered = false;

        static public void Init()
        {
            pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
            scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
            offset = new Vector2D(0f, (scaling.y / 2f));
            pivot = new Vector2D(pos.x + offset.x, pos.y - offset.y); // the bottom of the object
            angle = 0f;

            IsTriggered = false;
        }

        static public void Update()
        {

            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if(array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                IsTriggered = true;
            }

            if (InternalCalls.EntityCheckCollide(entity_id))
            {
                if (InternalCalls.GetCollidingEntityCheck(entity_id, PlayerScript.PLAYER_ID) || InternalCalls.GetCollidingEntityCheck(entity_id, PlayerScript.entity_feet))
                {
                    PlayerScript.isOnGrass = false;
                }
            }

            CheckRotatePlatform();
            
        }

        static public void CleanUp()
        {

        }

        static private void CheckRotatePlatform()
        {
            if (IsTriggered) {
                if (InternalCalls.GetTransformRotation() <= -90f)
                {
                    return;
                }
                /*float angle = (float)(MathF.PI / 180f) * InternalCalls.GetTransformRotation();
                angle -= 10f * InternalCalls.GetDeltaTime(); // rotating the angle

                Console.WriteLine(InternalCalls.GetTransformRotation());*/
                angle -= InternalCalls.GetDeltaTime() * rotating_speed;
                InternalCalls.TransformSetRotation(angle, 0f);

                float current_angle = angle * (MathF.PI / 180f);
                pos.x = MathF.Cos(current_angle) * offset.x - MathF.Sin(current_angle) * offset.y + pivot.x;
                pos.y = MathF.Sin(current_angle) * offset.x + MathF.Cos(current_angle) * offset.y + pivot.y;
                InternalCalls.TransformSetPosition(pos.x, pos.y);
            }
        }

    }
}

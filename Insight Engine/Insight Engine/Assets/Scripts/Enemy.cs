/*!
 * \file YouWinScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the YouWinScript class, used for the win scene
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System.Runtime.CompilerServices;
namespace IS
{
    class Enemy
    {

        static public float speed = 10f;
        static private Vector2D direction = new Vector2D(1f, 0f);
        static private bool isHit;
        static private float being_hit_timer = 0f;
        static private Vector2D scaling = new Vector2D(257f, 183f);
        static public int ENEMY_ID;
        static public void Init()
        {
            ENEMY_ID = InternalCalls.GetCurrentEntityID();
            InternalCalls.TransformSetScaleEntity(scaling.x, scaling.y, ENEMY_ID);
        }

        static public void Update()
        {
            if (isHit)
            {
                float vel_x = direction.x * speed;
                //float vel_y = direction.y * speed;
                InternalCalls.RigidBodySetVelocityEntity(vel_x, 0f, ENEMY_ID);
                being_hit_timer += InternalCalls.GetDeltaTime();
                if (being_hit_timer > 0.5f)
                {
                    isHit = false;
                    being_hit_timer = 0f;
                }
            }
            InternalCalls.TransformSetScaleEntity(InternalCalls.GetTransformScalingEntity(ENEMY_ID).x * direction.x, InternalCalls.GetTransformScalingEntity(ENEMY_ID).y, ENEMY_ID);
        }


        static public void CleanUp()
        {

        }

        static public void GetHit(Vector2D dir)
        {
            //InternalCalls.TransformSetScaleEntity(InternalCalls.GetTransformScalingEntity(ENEMY_ID).x * dir.x, InternalCalls.GetTransformScalingEntity(ENEMY_ID).y, ENEMY_ID);
            isHit = true;
            direction = dir;
        }
    }
}

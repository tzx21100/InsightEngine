/*!
 * \file BreakableWalls.cs
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

    class BreakableWalls
    {
        static public Dictionary<int, ActualBreakAbleWalls> created_entities = new Dictionary< int, ActualBreakAbleWalls>();


        static public void Init(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            created_entities[entity_id] = new ActualBreakAbleWalls();
            created_entities[entity_id].timer_to_allow_break = 1.5f;

        }

        static public void Update(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            created_entities[entity_id].Update();
        }

 

        }

    // the real class called by the wrapper above
    class ActualBreakAbleWalls
    {

        public float timer_to_allow_break = 1.5f;
        public void Update()
        {
            if (timer_to_allow_break > 0f) { timer_to_allow_break -= InternalCalls.GetDeltaTime(); }


            // dont allow if too small
            if (InternalCalls.GetTransformScaling().x < 30 || timer_to_allow_break>0f)
            {
                return;
            }

            int entity_id = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity_id))
            {
                if (InternalCalls.GetCollidingEntityCheck(entity_id, PlayerScript.PLAYER_ID))
                {
                    PlayerScript.isOnGrass = false;
                }
            }

            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                if (PlayerScript.isDashing && PlayerScript.bullet_time_timer <= 0)
                {

                    InternalCalls.RigidBodySetBodyTypeEntity(1, InternalCalls.GetCurrentEntityID());

                    InternalCalls.AudioPlaySound("AXE_GEN-HDF-02675.wav", false, 0.6f);

                    Vector2D trans = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
                    InternalCalls.TransformSetScale(trans.x / 2f, trans.y);
                    InternalCalls.RigidBodySetMassEntity(5, entity_id);

                    int entity = InternalCalls.CloneEntity(InternalCalls.GetCurrentEntityID());

                    InternalCalls.RigidBodySetBodyTypeEntity(1, entity);
                    InternalCalls.RigidBodySetMassEntity(5, entity);
                    timer_to_allow_break = 1.5f;

                }
            }
        }






        static public void CleanUp(){

        }

    }




}

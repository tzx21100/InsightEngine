/*!
 * \file BreakableWalls.cs
 * \author  Tan Zheng Xun, t.zhengxun@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This source file is meant for the player movement and camera.
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

    class BreakableWalls
    {
        static Dictionary<int, int> created_entities = new Dictionary<int, int>();


        static public void Init(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            created_entities[entity_id] = new int();
        }

        static public void Update(){

            int entity_id = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity_id))
            {
                if (InternalCalls.GetCollidingEntityCheck(entity_id, PlayerScript.PLAYER_ID))
                {
                    PlayerScript.isOnGrass = false;
                }
            }

            
            if (InternalCalls.GetCollidingEntityCheck(InternalCalls.GetCurrentEntityID(), PlayerScript.PLAYER_ID) && InternalCalls.RigidBodyGetBodyTypeEntity(InternalCalls.GetCurrentEntityID())==0)
            {
                if (PlayerScript.isDashing && PlayerScript.bullet_time_timer<=0) {


                    InternalCalls.RigidBodySetBodyTypeEntity(1, InternalCalls.GetCurrentEntityID());

                    InternalCalls.AudioPlaySound("AXE_GEN-HDF-02675.wav", false, 0.6f);
                    
                    Vector2D trans =  Vector2D.FromSimpleVector2D( InternalCalls.GetTransformScaling());
                    InternalCalls.TransformSetScale(trans.x / 2f, trans.y );
                    int entity = InternalCalls.CloneEntity(InternalCalls.GetCurrentEntityID());



                }
            }
        }



        static public void CleanUp(){

        }

    }




}

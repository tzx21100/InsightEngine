/*!
 * \file StoneCollider.cs
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
namespace IS
{
    class StoneCollider
    {
        static public void Init(){
            
        }

        static public void Update(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity_id)) {
                if (InternalCalls.GetCollidingEntityCheck(entity_id, PlayerScript.PLAYER_ID) || InternalCalls.GetCollidingEntityCheck(entity_id, PlayerScript.entity_feet))
                {
                    PlayerScript.isOnGrass = false;
                }
            }
        }
        
        static public void CleanUp(){

        }

    }
}

/*!
 * \file FragmentScript.cs
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

using System.ComponentModel;
using System.Runtime.CompilerServices;
namespace IS
{
    class CollectableScript
    {
        static public void Init() {

        }

        static public void Update()
        {
            int entity = InternalCalls.GetCurrentEntityID();
            Vector2D my_position = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity, PlayerScript.PLAYER_ID))
            {

                InternalCalls.AudioPlaySound("Collect.wav", false, 0.15f);
                PlayerScript.initialPowerUp = true;
                PlayerScript.collection_count += 1;
                InternalCalls.DestroyEntity(entity);



            }
        }

        static public void CleanUp() {

        }
       
    }
}

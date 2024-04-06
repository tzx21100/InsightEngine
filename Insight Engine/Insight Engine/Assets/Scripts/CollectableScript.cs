/*!
 * \file FragmentScript.cs
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

            int entity_id =InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {

                if(PlayerScript.collection_count == 0)
                {
                    CameraScript.CameraPanToTime(FogBlockade1.current_pos, 1.6f, 3f);
                }
                if (PlayerScript.collection_count == 1)
                {
                    CameraScript.CameraPanToTime(FogBlockade2.current_pos, 2.6f, 3f);
                }
                else
                {
                    CameraScript.CameraPanToTime(FogBlockade1.current_pos, 1.6f, 5f);
                }


                InternalCalls.AudioPlaySound("Collect.wav", false, 0.07f);
                PlayerScript.initialPowerUp = true;
                PlayerScript.collection_count += 1;
                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());


            }
        }

        static public void CleanUp() {

        }
       
    }
}

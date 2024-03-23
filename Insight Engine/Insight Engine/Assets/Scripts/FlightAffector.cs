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

    class FlightAffector
    {


        static public void Init(){

            CameraScript.CameraTargetZoom(0.7f, 0.1f);
        }

        static public void Update(){

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
                PlayerScript.AddForcesToPlayer(0, 400, 0.1f);
            }
        }







        static public void CleanUp(){

        }

    }




}

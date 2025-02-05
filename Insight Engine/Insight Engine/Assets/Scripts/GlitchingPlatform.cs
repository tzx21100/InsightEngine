/*!
 * \file GlitchingPlatform.cs
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
using System.Collections.Generic;
namespace IS
{

    class GlitchingPlatform
    {
        static Dictionary<int, GlitchedPlatforms> platforms = new Dictionary<int, GlitchedPlatforms>();

        static public void Init(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            platforms[entity_id] = new GlitchedPlatforms();
        }

        static public void Update(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            platforms[entity_id].update(); // Call Update on each GlitchingPlatform instance
           

        }



        static public void CleanUp(){
            InternalCalls.SetSpriteImageEntity(InternalCalls.GetSpriteImage("glitched_overworld_platform 5R2C.png"), InternalCalls.GetCurrentEntityID());
        }

    }


    class GlitchedPlatforms {

        private float TimeOutTimer = 4f;
        private float TimeOutTimerSet = 4f;
        private float RespawnTimerSet = 2f;
        private float RespawnTimer = 2f;
        private float shakeTimer = 0.2f;
        private float shakeTimerSet = 0.2f;
        private int shake_dir = 1;

        public void update()
        {
            
            int entity_id = InternalCalls.GetCurrentEntityID();
            TimeOutTimer -= InternalCalls.GetDeltaTime();

            if (TimeOutTimer <= 0)
            {
                //InternalCalls.SetSpriteAlphaEntity(0f, entity_id);
                InternalCalls.ColliderComponentRemove(entity_id);
                RespawnTimer -= InternalCalls.GetDeltaTime();
            }
            else {

                InternalCalls.SetSpriteAlphaEntity((TimeOutTimer*20 / TimeOutTimerSet), entity_id);

                if (shakeTimer > 0)
                {
                    shakeTimer -= InternalCalls.GetDeltaTime();
                }
                else
                {
                    InternalCalls.TransformSetRotationEntity(InternalCalls.GetTransformRotationEntity(entity_id) + 10f * shake_dir, 0, entity_id);
                    shakeTimer = shakeTimerSet;
                    shake_dir *= -1;
                }

            }




            if (RespawnTimer < 0)
            {
                InternalCalls.ColliderComponentAdd(entity_id, 0.80f, 0.85f);
                InternalCalls.SetSpriteAlphaEntity(1, entity_id); 
                TimeOutTimer = TimeOutTimerSet;
                RespawnTimer = RespawnTimerSet;
            }

        }

    }


}

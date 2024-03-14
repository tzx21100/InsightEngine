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
    class FragmentScript
    {
        static float glitch_timer = 0f;
        static public void Init(){

        }

        static public void Update(){





            int entity = InternalCalls.GetCurrentEntityID();
            Vector2D my_position = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());



            if (Vector2D.Distance(my_position, PlayerScript.player_pos) < 900f && !PlayerScript.Reward_Dash)
            {
                glitch_timer = 1f;
            }

            if (glitch_timer > 0)
            {
                InternalCalls.ChangeLightType(3);
                InternalCalls.GlitchEnable(true);
                glitch_timer -= InternalCalls.GetDeltaTime();
            }
            else
            {
                InternalCalls.ChangeLightType(PlayerScript.PLAYER_LIGHT);
                InternalCalls.GlitchEnable(false);
            }


            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity,PlayerScript.PLAYER_ID)) {
                if (!PlayerScript.Reward_DoubleJump) { PlayerScript.Reward_DoubleJump = true; PlayerScript.hasDoubleJump = true; } else
                {
                    if (!PlayerScript.Reward_WallClimb) {
                        PlayerScript.Reward_WallClimb = true;
                        PlayerScript.hasWallClimb = true;
                    }
                    else
                    {
                        if (!PlayerScript.Reward_Dash) {
                            PlayerScript.Reward_Dash = true;
                            PlayerScript.hasDash = true;    
                        }
                        else
                        {
                            InternalCalls.CameraSetZoom(1f);
                            InternalCalls.AttachCamera(0, 0);
                            InternalCalls.AudioStopAllSounds();
                            PlayerScript.PLAYER_LIGHT = 0;
                            InternalCalls.ChangeShaderIntensity(0f);
                            InternalCalls.LoadScene("Assets/Scenes/YouWin.insight");
                        }
                    }

                }

                InternalCalls.AudioPlaySound("StartClick.wav",false,0.15f);
                PlayerScript.initialPowerUp = true;
                InternalCalls.GlitchEnable(false);
                InternalCalls.ChangeLightType(PlayerScript.PLAYER_LIGHT);
                InternalCalls.DestroyEntity(entity);
                
            }







        }
        
        static public void CleanUp(){

        }

    }
}

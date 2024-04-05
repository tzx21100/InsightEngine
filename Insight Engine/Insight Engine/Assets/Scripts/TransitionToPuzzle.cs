/*!
 * \file TransitionToPuzzle.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2451
 * \date 15-03-2024
 * \brief
 * This header file contains the implementation for the transition to puzzle level.
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
    class TransitionToPuzzle
    {
        static private float fade_out_timer = 0f;
        static private float fade_out_duration =0.1f;
        static private bool is_transiting = false;
        static public void Init(){
            fade_out_timer = 0f;
            fade_out_duration = 0.1f;
            is_transiting = false;
        }

        static public void Update(){

            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1) {
                is_transiting = true;
            }

            if (is_transiting) {

                fade_out_timer += InternalCalls.GetDeltaTime();

                InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 7000, 7000, 0, 0, 0, fade_out_timer, InternalCalls.GetTopLayer());

                if (fade_out_timer > fade_out_duration)
                {
                    if (PlayerScript.high_score > PlayerScript.speed_run_timer)
                    {
                        PlayerScript.high_score = PlayerScript.speed_run_timer;
                    }

                    InternalCalls.LoadScene("Assets/Scenes/FlightLevel.insight");
                }
            }
        }
        
        static public void CleanUp(){


        }

    }
}

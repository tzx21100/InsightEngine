/*!
 * \file StartButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (90%), Matthew Ng, matthewdeen.ng@digipen.edu (10%)
 * \par Course: CSD2451
 * \brief
 * This header file contains the StartButtonScript class, used for the start button 
 * in the main menu
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
    class StartButtonScript
    {

        static public bool first_hover = false;
        static public bool click = false;
        static public float timer = 0f;

        static public void Init() {
            timer = 0.5f;
            click = false;
            InternalCalls.AttachCamera(0f, 0f);
            InternalCalls.CameraSetZoom(1f);
        }

        static public void Update(){
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hover) {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
                    first_hover = true;
                }
            }
            else
            {
                first_hover = false;
            }

            // clicking
            if (InternalCalls.GetButtonState() == 2) 
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                click = true;
                PlayerScript.CleanUp();
            }

            if (click)
            {
                timer -= InternalCalls.GetDeltaTime();
                if (timer <= 0f)
                {
                    InternalCalls.LoadScene("Assets/Scenes/CaveLevel.insight");
                }
            }

        }
        
        static public void CleanUp(){

        }

    }
}

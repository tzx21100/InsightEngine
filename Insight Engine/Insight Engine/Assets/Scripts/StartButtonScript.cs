/*!
 * \file StartButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (90%), Matthew Ng, matthewdeen.ng@digipen.edu (10%)
 * \par Course: CSD2451
 * \brief
 * This header file contains the StartButtonScript class, used for the start button 
 * in the main menu
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
    class StartButtonScript
    {
        static public bool first_hover = false;
        static public bool click = false;
        static public float timer = 0f;
        static public float font_size = MainMenuScript.NORMAL_FONT_SIZE;

        static public void Init() {
            timer = 0.5f;
            click = false;
            InternalCalls.AttachCamera(0f, 0f);
            InternalCalls.CameraSetZoom(1f);
        }

        static public void Update(){
            //hovered
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Hovered)
            {
                //hovering
                if (!first_hover)
                {
                    SettingsScript.PlayHoverSound();
                    first_hover = true;
                    font_size = MainMenuScript.HOVER_FONT_SIZE;
                }
            }
            else
            {
                first_hover = false;
                font_size = MainMenuScript.NORMAL_FONT_SIZE;
            }
            if (!InternalCalls.IsWindowFocused())
            {
                first_hover = true;
            }
            // clicking
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Pressed) 
            {
                SettingsScript.PlayClickSound();
                click = true;
                PlayerScript.CleanUp();
            }

            if (click)
            {
                timer -= InternalCalls.GetDeltaTime();
                if (timer <= 0f)
                {
                    InternalCalls.LoadScene("Assets/Scenes/CutScene.insight");
                }
            }

        }
        
        static public void CleanUp(){

        }

    }
}

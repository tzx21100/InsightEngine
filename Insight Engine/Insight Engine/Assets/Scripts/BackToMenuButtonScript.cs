/*!
 * \file BackToMenuButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (90%), Matthew Ng, matthewdeen.ng@digipen.edu (10%)
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the BackToMenuButtonScript class, used for the back 
 * to main menu button in pause menu in game
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
    class BackToMenuButtonScript
    {
        static public bool first_hover = false;
        static public float font_size = MainMenuScript.NORMAL_FONT_SIZE;
        static public void Init()
        {
        }

        static public void Update()
        {
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
                //click
                SettingsScript.PlayClickSound();
                PlayerScript.CleanUp();
                InternalCalls.LoadScene("Assets/Scenes/MainMenu.insight");
                InternalCalls.AudioStopAllSounds();
            }
        }


        static public void CleanUp()
        {

        }
    }
}

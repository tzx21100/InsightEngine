/*!
 * \file BackFromWinScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (90%), Matthew Ng, matthewdeen.ng@digipen.edu (10%)
 * \par Course: CSD2401
 * \brief
 * This header file contains the BackFromWinScript class, used for the back 
 * button in win scene
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
    class BackFromWinScript
    {
        static public bool first_hover = false;
        static public float font_size = MainMenuScript.NORMAL_FONT_SIZE;

        static public void Init()
        {
        }

        static public void Update()
        {
          
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {

                //hovering
                if (!first_hover)
                {
                    //InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
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

            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                //InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
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

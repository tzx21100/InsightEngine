/*!
 * \file BackFromSettingsButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the BackFromSettingsButtonScript class, used for the back 
 * button in settings
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
    class BackFromSettingsButtonScript
    {
        static public bool first_hover = false;
        static SimpleImage back_button_image;
        static SimpleImage back_button_hovered_image;

        static public void Init()
        {
            back_button_image = InternalCalls.GetSpriteImage("back_button.png");
            back_button_hovered_image = InternalCalls.GetSpriteImage("back_button_hovered.png");

        }

        static public void Update()
        {
            InternalCalls.SetSpriteImage(back_button_image);
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                InternalCalls.SetSpriteImage(back_button_hovered_image);
                //hovering
                if (!first_hover)
                {
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
                //click
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                SettingsScript.show_settings = false;
            }
        }


        static public void CleanUp()
        {

        }
    }
}

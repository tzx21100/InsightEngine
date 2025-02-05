/*!
 * \file BackFromSettingsButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2451
 * \brief
 * This header file contains the BackFromSettingsButtonScript class, used for the back 
 * button in settings
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
                    SettingsScript.PlayHoverSound();
                    first_hover = true;
                }
            }
            else
            {
                first_hover = false;
            }
            if (!InternalCalls.IsWindowFocused())
            {
                first_hover = true;
            }
            // clicking
            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                SettingsScript.PlayClickSound();
                SettingsScript.show_settings = false;
            }
        }


        static public void CleanUp()
        {

        }
    }
}

/*!
 * \file NoButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the NoButtonScript class, used for the no button in exit game prompt window
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
    class NoButtonScript
    {
        static public bool first_hovering = false;
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
                if (!first_hovering)
                {
                    SettingsScript.PlayHoverSound();
                    first_hovering = true;
                    font_size = MainMenuScript.HOVER_FONT_SIZE;
                }
            }
            else
            {
                first_hovering = false;
                font_size = MainMenuScript.NORMAL_FONT_SIZE;
            }
            if (!InternalCalls.IsWindowFocused())
            {
                first_hovering = true;
            }

            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                SettingsScript.PlayClickSound();
                ExitButtonScript.exit_confirmation = false;
            }
        }


        static public void CleanUp()
        {

        }
    }
}

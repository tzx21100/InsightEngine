/*!
 * \file ResumeButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the ResumeButtonScript class, used for the resume button 
 * in the pause menu
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
    class ResumeButtonScript
    {
        public const float NORMAL_FONT_SIZE = 11f;
        public const float HOVER_FONT_SIZE = 13f;
        static public bool first_hovering = false;
        static public float font_size = NORMAL_FONT_SIZE;

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
                    font_size = HOVER_FONT_SIZE;
                }
            }
            else
            {
                first_hovering = false;
                font_size = NORMAL_FONT_SIZE;
            }
            if (!InternalCalls.IsWindowFocused())
            {
                first_hovering = true;
            }
            // clicking
            if (InternalCalls.GetButtonState() == 2)
            {
                PauseButtonScript.light_switch = !PauseButtonScript.light_switch;
                InternalCalls.SetLightsToggle(PauseButtonScript.light_switch);
                //click
                SettingsScript.PlayClickSound();
                PauseButtonScript.paused = false;
            }
            
        }


        static public void CleanUp()
        {

        }
    }
}

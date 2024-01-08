/*!
 * \file BackToMenuButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the BackToMenuButtonScript class, used for the back 
 * to main menu button in pause menu in game
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
    class BackToMenuButtonScript
    {
        static public bool first_hovering = false;
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
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
                    first_hovering = true;
                }
            }
            else
            {
                first_hovering = false;
            }

            // clicking
            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                InternalCalls.LoadScene("Assets/Scenes/Main Menu.insight");
                InternalCalls.AudioStopAllSounds();
            }
        }


        static public void CleanUp()
        {

        }
    }
}

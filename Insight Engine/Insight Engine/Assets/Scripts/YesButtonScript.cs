/*!
 * \file YesButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the YesButtonScript class, used for the yes button in exit game prompt window
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
    class YesButtonScript
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
                InternalCalls.Exit();
            }
        }


        static public void CleanUp()
        {

        }
    }
}

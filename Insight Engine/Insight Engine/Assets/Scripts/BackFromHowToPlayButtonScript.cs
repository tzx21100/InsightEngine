/*!
 * \file BackFromHowToPlayButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2451
 * \brief
 * This header file contains the BackFromHowToPlayButtonScript class, used for the back 
 * button in how to play
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
    class BackFromHowToPlayButtonScript
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
                HowToPlayScript.show_how_to_play = false;
            }
        }


        static public void CleanUp()
        {

        }
    }
}

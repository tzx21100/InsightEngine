/*!
 * \file VFXCheckboxScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This C# file contains the VFXCheckboxScript class, used for the VFXCheckbox in settings page. When it is toggled on means audio on, when toggled off means audio off.
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
    class VFXCheckboxScript
    {
        static public bool first_hover = false;
        static public bool toggled = true;
        static public SimpleImage checkbox_image = InternalCalls.GetSpriteImage("checkbox.png");
        static public SimpleImage toggled_image = InternalCalls.GetSpriteImage("checkbox_toggled.png");
        static public bool clicked = false;

        static public void Init()
        {
            
        }

        static public void Update()
        {
            if (!toggled)
            {
                InternalCalls.SetSpriteImage(checkbox_image);
                SettingsScript.vfx_multiplier = 0f;
            }
            else
            {
                InternalCalls.SetSpriteImage(toggled_image);
                SettingsScript.vfx_multiplier = VFXSliderKnobScript.normalised_adjustment;
            }


            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hover)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
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
                //clicked = !clicked;
                //click
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
                toggled = !toggled;
            }

            
        }


        static public void CleanUp()
        {

        }
    }
}

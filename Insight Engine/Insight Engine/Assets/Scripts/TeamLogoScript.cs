/*!
 * \file TeamLogoScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This C# file contains the TeamLogoScript class, used for the main menu team logo gimick when click
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Numerics;
namespace IS
{
    class TeamLogoScript
    {
        static public bool first_hover = false;
        static public bool is_lighted = false;
        static private int entity_id;

        static private List<Vector3> hues = new List<Vector3>();
        static private Vector3 original_hue;

        static private Random random = new Random(); //for diff hues
        static private Vector3 rand_hue;
        static public void Init()
        {
            entity_id = InternalCalls.GetCurrentEntityID();

            hues.Add(new Vector3(1.0f, 0.505f, 0f)); //orange
            hues.Add(new Vector3(1.0f, 0f, 0f)); //red
            hues.Add(new Vector3(0f, 1.0f, 0f)); //green
            hues.Add(new Vector3(0f, 0f, 1.0f)); //blue
            hues.Add(new Vector3(1.0f, 0f, 0.937f)); //pink

            original_hue = hues[0];

            InternalCalls.AttachLightComponentToEntityWithOffset(entity_id, original_hue.x, original_hue.y, original_hue.z, 1.0f, 400.0f, new SimpleVector2D(5.0f,60.0f));
            InternalCalls.SetLightToggleEntity(entity_id, is_lighted);
            //InternalCalls.SetButtonSize(entity_id, new SimpleVector2D(684.29f, 286.05f));


        }

        static public void Update()
        {
            if ( SettingsScript.show_settings || ExitButtonScript.exit_confirmation) {
                InternalCalls.ButtonComponentRemove(entity_id);
            }
            else
            {
                if (!InternalCalls.ButtonComponentExists(entity_id))
                {
                    InternalCalls.ButtonComponentAdd(entity_id, 0.8f, 1.0f, 0.9f, 1.0f, 1.150f);
                    //InternalCalls.SetButtonSize(entity_id, new SimpleVector2D(684.29f, 286.05f));
                }
            }

            //click
            if (InternalCalls.GetButtonState() == 2)
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                rand_hue = GetRandomHue();
                InternalCalls.SetLightHueEntity(entity_id, rand_hue.x, rand_hue.y, rand_hue.z);
                is_lighted = !is_lighted;
                InternalCalls.SetLightToggleEntity(entity_id, is_lighted);
            }
        }


        static public void CleanUp()
        {

        }

       static private Vector3 GetRandomHue()
        {
            if (hues.Count == 0)
            {
                throw new InvalidOperationException("The hues list is empty.");
            }

            int rand = random.Next(hues.Count); // Generates a random index between 0 and hues.Count - 1
            return hues[rand];
        }
    }
}
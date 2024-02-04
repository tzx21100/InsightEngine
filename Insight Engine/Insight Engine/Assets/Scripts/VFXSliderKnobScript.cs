using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class VFXSliderKnobScript
    {
        static public bool first_hover = false;
        //static public int targetedEntity=InternalCalls.GetCurrentEntityID();
        static private float adjustment;
        static public float normalised_adjustment;
        static public void Init()
        {
            Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());
            adjustment = Math.Min(111.36f, Math.Max(-111.36f, mouse_pos.x));
            normalised_adjustment = (adjustment + 111.36f) / (111.36f + 111.36f);
        }

        static public void Update()
        {
            Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hover)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
                    first_hover = true;
                }
                if (InternalCalls.MouseHeld(0)==true)
                {
                    adjustment = Math.Min(111.36f, Math.Max(-111.36f, mouse_pos.x));
                    InternalCalls.TransformSetPosition(adjustment, InternalCalls.GetTransformPosition().y);
                    SettingsScript.vfx_slider_knob_pos.x = adjustment;
                    normalised_adjustment = (adjustment + 111.36f) / (111.36f + 111.36f);

                    /*if (MasterCheckboxScript.toggled == true)
                    {
                        SettingsScript.vfx_multiplier = 0f;
                    }
                    else
                    {
                        SettingsScript.vfx_multiplier = normalised_adjustment;
                    }*/
                     
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
                //Console.WriteLine(SettingsScript.master_multiplier);
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
            }

            if (MasterCheckboxScript.toggled)
            {
                SettingsScript.master_multiplier = 0f;
            }
            SettingsScript.vfx_multiplier = normalised_adjustment;

        }


        static public void CleanUp()
        {

        }
    }
}

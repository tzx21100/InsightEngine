using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class BGMSliderKnobScript
    {
        static public bool first_hover = false;
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
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f * SettingsScript.vfx_vol);
                    first_hover = true;
                }
                if (InternalCalls.MouseHeld(0) == true)
                {
                    adjustment = Math.Min(111.36f, Math.Max(-111.36f, mouse_pos.x));
                    InternalCalls.TransformSetPosition(adjustment, InternalCalls.GetTransformPosition().y);
                    SettingsScript.bgm_slider_knob_pos.x = adjustment;
                    normalised_adjustment = (adjustment + 111.36f) / (111.36f + 111.36f);



                }

            }
            else
            {
                first_hover = false;
            }
            if (MasterCheckboxScript.toggled)
            {
                SettingsScript.master_multiplier = 0f;
            }
            // clicking
            if (InternalCalls.GetButtonState() == 2)
            {
                //Console.WriteLine(MasterCheckboxScript.toggled);
                //Console.WriteLine(SettingsScript.master_multiplier); 
                //click
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f * SettingsScript.vfx_vol);
            }
            
           
            SettingsScript.bgm_multiplier = normalised_adjustment;
        }


        static public void CleanUp()
        {

        }
    }
}

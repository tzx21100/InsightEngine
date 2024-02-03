using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class VFXSliderKnobScript
    {
        static public bool first_hover = false;
        static public int targetedEntity=InternalCalls.GetCurrentEntityID();
        static private int vfx_slider_knob_entity;
        static public float adjustment;
        static public void Init()
        {
            vfx_slider_knob_entity = InternalCalls.GetCurrentEntityID();
        }

        static public void Update()
        {
            //vfx_slider_knob_pos = InternalCalls.GetTransformPositionEntity(vfx_slider_knob_entity);
            Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hover)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
                    first_hover = true;
                }
                if (InternalCalls.MouseHeld(0)==true)
                {
                    InternalCalls.TransformSetPosition(Math.Min(111.36f, Math.Max(-111.36f, mouse_pos.x)), InternalCalls.GetTransformPosition().y);
                    SettingsScript.vfx_slider_knob_pos.x = Math.Min(111.36f, Math.Max(-111.36f, mouse_pos.x));

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
            }




        }


        static public void CleanUp()
        {

        }
    }
}

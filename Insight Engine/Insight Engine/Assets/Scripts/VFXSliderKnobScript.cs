using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class VFXSliderKnobScript
    {
        static public bool first_hover = false;
        static private float adjustment;
        static public float normalised_adjustment;
        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public float lower_limit_vfx_knob;
        static public float upper_limit_vfx_knob;
        static public void Init()
        {
            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            lower_limit_vfx_knob = origin.x + (0.442f * win_dimension.x);
            upper_limit_vfx_knob = origin.x + (0.558f * win_dimension.x);
            Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());
            adjustment = Math.Min(upper_limit_vfx_knob, Math.Max(lower_limit_vfx_knob, mouse_pos.x));
            normalised_adjustment = (adjustment + upper_limit_vfx_knob) / (upper_limit_vfx_knob + upper_limit_vfx_knob);
            //InternalCalls.TransformSetPosition(origin.x + (0.5f * win_dimension.x), origin.y + (0.323f * win_dimension.y));

        }

        static public void Update()
        {
            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            lower_limit_vfx_knob = origin.x + (0.442f * win_dimension.x);
            upper_limit_vfx_knob = origin.x + (0.558f * win_dimension.x);
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
                    adjustment = Math.Min(upper_limit_vfx_knob, Math.Max(lower_limit_vfx_knob, mouse_pos.x));
                    InternalCalls.TransformSetPosition(adjustment, origin.y + (0.323f * win_dimension.y));
                    SettingsScript.vfx_slider_knob_pos.x = adjustment;
                    normalised_adjustment = (adjustment + upper_limit_vfx_knob) / (upper_limit_vfx_knob + upper_limit_vfx_knob);

                }

            }
            else
            {
                first_hover = false;
            }

            // clicking
            if (MasterCheckboxScript.toggled)
            {
                SettingsScript.master_multiplier = 0f;
            }
            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                //Console.WriteLine(SettingsScript.master_multiplier);
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
            }
            if (SettingsScript.show_settings)
            {
                InternalCalls.TransformSetPosition(adjustment, origin.y + (0.323f * win_dimension.y));
            }
            if (!SettingsScript.show_settings)
            {
                InternalCalls.TransformSetPosition(9999f, 9999f);
            }
            SettingsScript.vfx_multiplier = normalised_adjustment;

        }


        static public void CleanUp()
        {

        }
    }
}

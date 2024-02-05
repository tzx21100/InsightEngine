using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class BGMSliderKnobScript
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

        static public void Init()
        {
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);
            
            Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());
            adjustment = Math.Min(111.36f, Math.Max(-111.36f, mouse_pos.x));
            normalised_adjustment = (adjustment + 111.36f) / (111.36f + 111.36f);
        }

        static public void Update()
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

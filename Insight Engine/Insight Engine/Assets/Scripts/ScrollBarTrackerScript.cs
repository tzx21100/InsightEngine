using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class ScrollBarTrackerScript
    {
        static public bool first_hover = false;
        static private int id;
        static private float diff_y;
        static private float adjustment;
        static private bool first_open_settings = false;

        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public float upper_limit_scroll_track;
        static public float lower_limit_scroll_track;

        static public float virtual_y;
        static public void Init()
        {
            id = InternalCalls.GetCurrentEntityID();
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

            upper_limit_scroll_track = origin.y + (0.532f * win_dimension.y);
            lower_limit_scroll_track = origin.y + (0.365f * win_dimension.y);

            first_open_settings = false;
            diff_y = 0.532f;
            InternalCalls.SetButtonHoverScale(id, 0.95f);
        }

        static public void Update()
        {
            //InternalCalls.TransformSetPosition(origin.x + (0.5f * win_dimension.x), origin.y + (0.433f * win_dimension.y));
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
            upper_limit_scroll_track = origin.y + (0.532f * win_dimension.y);
            lower_limit_scroll_track = origin.y + (0.365f * win_dimension.y);
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
                    adjustment = Math.Min(upper_limit_scroll_track, Math.Max(lower_limit_scroll_track, mouse_pos.y));
                    diff_y = (adjustment - origin.y) / win_dimension.y;
                    InternalCalls.TransformSetPosition(origin.x + (0.593f * win_dimension.x), adjustment);
                    SettingsScript.scroll_bar_tracker_pos.y = adjustment;

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
                //click
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f * SettingsScript.vfx_vol);
            }

            if (SettingsScript.show_settings)
            {
                if (!first_open_settings)
                {
                    adjustment = origin.y + diff_y * win_dimension.y;
                    first_open_settings = true;
                }
                InternalCalls.TransformSetPosition(origin.x + (0.593f * win_dimension.x), adjustment);
                
            }
            if (!SettingsScript.show_settings)
            {
                first_open_settings = false;
                InternalCalls.TransformSetPosition(9999f, 9999f);
            }

        }


        static public void CleanUp()
        {

        }
    }
}

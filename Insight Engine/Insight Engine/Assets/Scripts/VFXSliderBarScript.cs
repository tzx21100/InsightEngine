using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class VFXSliderBarScript
    {
        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public float x_pos;
        static public float y_pos;

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
            x_pos = origin.x + (0.5f * win_dimension.x);
            y_pos = origin.y + (0.321f * win_dimension.y) - ScrollBarTrackerScript.virtual_y;
            if (SettingsScript.show_settings)
            {
                InternalCalls.TransformSetPosition(x_pos, y_pos);

            }
            if (!SettingsScript.show_settings|| y_pos > (origin.y + (0.7f * win_dimension.y)))
            {
                InternalCalls.TransformSetPosition(9999f, 9999f);
            }


        }


        static public void CleanUp()
        {

        }
    }
}

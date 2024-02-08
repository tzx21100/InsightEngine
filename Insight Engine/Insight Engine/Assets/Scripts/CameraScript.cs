using System;
using System.Runtime.CompilerServices;
namespace IS
{

    class CameraScript
    {


        //cam shake
        static public float camera_shake_duration = 0f;
        static public float camera_shake_duration_set = 0.2f;
        static public float camera_shake_timer = 0.02f;
        static public float camera_shake_set = 0.02f;
        static public Vector2D camera_shake_dir = new Vector2D(0, 0);
        static public float camera_shake_angle = 0f;

        static public Vector2D camera_pos = new Vector2D(0, 0);
        static public Vector2D target_pos = new Vector2D(0, 0);
        static public float camera_zoom = 1f;

        //zoom
        static public bool zoom_enable = false;
        static public float camera_zoom_duration = 0f;
        static private float targeted_zoom_level;

         

        static public void Init(){
            //camera_zoom = 1f;
            //camera zoom
            //InternalCalls.CameraSetZoom(camera_zoom);
            InternalCalls.AttachCamera(PlayerScript.player_pos.x, PlayerScript.player_pos.y);
        }

        static public void Update() {
            if (zoom_enable)
            {
                CameraDoZoom();
            }

            if (!CameraShake(camera_shake_duration))
            {
                InternalCalls.AttachCamera(camera_pos.x, camera_pos.y);
                InternalCalls.CameraSetZoom(camera_zoom);
            }

        }
        
        static public void CleanUp(){

        }

        static public bool CameraShake(float speed)
        {
            if (camera_shake_duration > 0)
            {
                camera_shake_duration -= InternalCalls.GetDeltaTime();

                camera_shake_timer -= InternalCalls.GetDeltaTime();

                if (camera_shake_timer <= 0)
                {
                    camera_shake_dir = Vector2D.DirectionFromAngle(camera_shake_angle);
                    camera_shake_timer = camera_shake_set;
                    camera_shake_angle += CustomMath.PI / 4;
                }
                InternalCalls.AttachCamera(camera_pos.x + 10 * camera_shake_dir.x, camera_pos.y + 10 * camera_shake_dir.y);

                return true;
            }

            return false;
        }


        static public void CameraTargetZoom(float zoom_level, float zoom_speed)
        {
            targeted_zoom_level = zoom_level;
            float zoom_difference = Math.Abs(camera_zoom - zoom_level);

            // Ensure zoom_speed is positive and not zero
            zoom_speed = Math.Max(zoom_speed, 0.01f);

            camera_zoom_duration = zoom_difference / zoom_speed;
            zoom_enable = true;
        }

        static private void CameraDoZoom()
        {
            if (camera_zoom_duration > 0)
            {
                float zoom_step = (targeted_zoom_level - camera_zoom) / camera_zoom_duration * InternalCalls.GetDeltaTime();
                camera_zoom += zoom_step;

                camera_zoom_duration -= InternalCalls.GetDeltaTime();

                if (camera_zoom_duration <= 0)
                {
                    camera_zoom = targeted_zoom_level; // Ensure exact zoom level is set at the end
                    zoom_enable = false;
                }
            }
        }


    }
}

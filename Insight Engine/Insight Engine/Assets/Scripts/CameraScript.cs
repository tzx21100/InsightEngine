using System;
using System.Runtime.CompilerServices;
namespace IS
{

    class CameraScript
    {
        //cam offset
        static public float cam_x_offset = 0;
        static public float cam_y_offset = 0;

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

        static public float camera_shake_intensity = 1f;

        //zoom
        static public bool zoom_enable = false;
        static public float camera_zoom_duration = 0f;
        static private float targeted_zoom_level;

        // camera panning 
        static public bool panning_enable = false;
        static public Vector2D panning_target_pos = new Vector2D(0, 0);
        static public float panning_speed = 1f;
        static private Vector2D pan_camera_pos= new Vector2D(0, 0);
        static private float pan_time=-1f;
        
        static public void Init(){
            //camera_zoom = 1f;
            //camera zoom
            //InternalCalls.CameraSetZoom(camera_zoom);
            InternalCalls.AttachCamera(camera_pos.x,camera_pos.y);
        }

        static public void Update() {
           // CameraShake(camera_shake_duration);
            if (zoom_enable)
            {
                CameraDoZoom();
                InternalCalls.CameraSetZoom(camera_zoom);
            }

            if (panning_enable)
            {
                
                CameraDoPan();

                return;
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
                InternalCalls.AttachCamera(camera_pos.x  + (10*camera_shake_intensity) * camera_shake_dir.x, camera_pos.y + (10 * camera_shake_intensity) * camera_shake_dir.y);

                return true;
            }

            camera_shake_intensity = 1f;
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

        static public void CameraPanTo(Vector2D targetPos, float speed)
        {
            pan_camera_pos = camera_pos; // Set the initial pan camera to normal camera
            panning_target_pos = targetPos;
            panning_speed = Math.Max(speed, 0.01f); // Ensure speed is positive and not zero
            panning_enable = true;
        }

        static public void CameraPanToTime(Vector2D targetPos, float speed ,float time)
        {
            pan_camera_pos = camera_pos; // Set the initial pan camera to normal camera
            panning_target_pos = targetPos;
            panning_speed = Math.Max(speed, 0.01f); // Ensure speed is positive and not zero
            panning_enable = true;
            pan_time = time;
        }

        static public void StopCameraPan()
        {
            panning_enable=false;
            pan_time = -1f;
        }


        static private void CameraDoPan()
        {
            if (pan_time != -1f)
            {
                pan_time-=InternalCalls.GetDeltaTime();
                if (pan_time <= 0f){
                    panning_enable = false;
                    pan_time = -1f;
                }
                
            }



            if (panning_enable)
            {

            pan_camera_pos=Vector2D.Lerp(pan_camera_pos, panning_target_pos, panning_speed * InternalCalls.GetDeltaTime());
            InternalCalls.AttachCamera(pan_camera_pos.x, pan_camera_pos.y);
            camera_pos = pan_camera_pos;
            }
        }



        static public void SetCameraPosition(Vector2D camera)
        {
            if(!panning_enable)
            {
                camera_pos = camera;
            }
           
        }

        static public void CameraForcePosition(float x,float y)
        {
            
            CameraScript.camera_pos.x = x;
            CameraScript.camera_pos.y = y;
        }



    }
}

using System;
namespace IS
{
    class CameraZoomScript3
    {

        static private float zoom_out_timer = 3f;
        static private bool activated = false;
        static public void Init()
        {
            zoom_out_timer = 3f;
            activated = false;
        }

        static public void Update()
        {
            int entity = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity, PlayerScript.PLAYER_ID))
            {
                CameraScript.CameraTargetZoom(0.4f, 0.3f);
                InternalCalls.ChangeLightType(0);
                activated = true;

            }

            if(activated)
            {
                zoom_out_timer -= InternalCalls.GetDeltaTime();
                if (zoom_out_timer < 0)
                {
                    CameraScript.CameraTargetZoom(0.7f, 0.2f);
                    PlayerScript.hideHealth = false;
                    InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());
                }
            }
            


        }

        static public void CleanUp()
        {

        }

    }
}
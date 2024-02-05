using System;
namespace IS
{
    class CameraZoomScript3
    {

        static public void Init()
        {

        }

        static public void Update()
        {
            int entity = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity, PlayerScript.PLAYER_ID))
            {
                CameraScript.CameraTargetZoom(0.5f, 0.3f);
                InternalCalls.ChangeLightType(0);
            }
        }

        static public void CleanUp()
        {

        }

    }
}
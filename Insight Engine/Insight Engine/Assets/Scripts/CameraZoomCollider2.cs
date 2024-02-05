using System;
namespace IS
{
    class CameraZoomCollider2
    {

        static public void Init()
        {

        }

        static public void Update()
        {
            int entity = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity, PlayerScript.PLAYER_ID))
            {
                CameraScript.CameraTargetZoom(0.7f, 0.3f);
           
            }
        }

        static public void CleanUp()
        {

        }

    }
}

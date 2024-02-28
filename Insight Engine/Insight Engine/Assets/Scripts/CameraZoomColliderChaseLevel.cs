using System.Runtime.CompilerServices;
namespace IS
{
    class CameraZoomColliderChaseLevel
    {

        static public void Init(){
            PlayerScript.respawn_x = -600;
            PlayerScript.respawn_y = -500;
        }

        static public void Update(){
            int entity = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity, PlayerScript.PLAYER_ID)) {
                CameraScript.CameraTargetZoom(0.5f,0.5f);
                InternalCalls.ChangeLightType(1);
                PlayerScript.PLAYER_LIGHT = 1;
                PlayerScript.hideHealth = false;
            }
        }
        
        static public void CleanUp(){


        }

    }
}

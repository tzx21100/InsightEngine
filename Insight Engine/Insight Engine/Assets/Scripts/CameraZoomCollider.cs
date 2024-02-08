using System.Runtime.CompilerServices;
namespace IS
{
    class CameraZoomCollider
    {

        static public void Init(){
            PlayerScript.respawn_x = -807;
            PlayerScript.respawn_y = 1810;
        }

        static public void Update(){
            int entity = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity, PlayerScript.PLAYER_ID)) {
                CameraScript.CameraTargetZoom(1f,0.5f);
                InternalCalls.ChangeLightType(1);
                PlayerScript.PLAYER_LIGHT = 1;
                PlayerScript.hideHealth = true;
            }
        }
        
        static public void CleanUp(){


        }

    }
}

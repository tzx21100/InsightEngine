using System.Runtime.CompilerServices;
namespace IS
{
    class CameraZoomCollider
    {

        static public void Init(){
            PlayerScript.respawn_x = -8585;
            PlayerScript.respawn_y = 1047;
        }

        static public void Update(){
            int entity = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity, PlayerScript.PLAYER_ID)) {
                CameraScript.CameraTargetZoom(1f,0.5f);
                InternalCalls.ChangeLightType(1);
            }
        }
        
        static public void CleanUp(){


        }

    }
}

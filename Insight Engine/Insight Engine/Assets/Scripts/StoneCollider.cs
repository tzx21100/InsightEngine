using System.Runtime.CompilerServices;
namespace IS
{
    class StoneCollider
    {
        static public void Init(){
            
        }

        static public void Update(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity_id)) {
                if (InternalCalls.GetCollidingEntityCheck(entity_id, PlayerScript.PLAYER_ID))
                {
                    PlayerScript.isOnGrass = false;
                }
            }
        }
        
        static public void CleanUp(){

        }

    }
}

using System.Runtime.CompilerServices;
namespace IS
{
    class SpikeScript
    {
        static public void Init(){

        }

        static public void Update(){

            int entity_id = InternalCalls.GetCurrentEntityID();
            if (InternalCalls.EntityCheckCollide(entity_id) && !InternalCalls.CollidingObjectIsStatic(entity_id)) {
                if (InternalCalls.GetCollidingEntityCheck(entity_id, PlayerScript.PLAYER_ID))
                {
                    PlayerScript.isDead = true;
                }
            }
        }
        
        static public void CleanUp(){

        }

    }
}

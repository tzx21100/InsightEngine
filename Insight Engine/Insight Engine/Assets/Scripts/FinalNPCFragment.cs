using System.Runtime.CompilerServices;
namespace IS
{
    class FinalNPCFragment
    {
        static public void Init(){

        }

        static public void Update(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {
                PlayerScript.NPC_Transformation = true;
                InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());
                InternalCalls.AudioPlaySound("StartClick.wav", false, 0.05f);
                PlayerScript.initialPowerUp = true;
            }
        }
        
        static public void CleanUp(){

        }

    }
}

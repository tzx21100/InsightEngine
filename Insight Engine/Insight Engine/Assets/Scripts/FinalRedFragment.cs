using System.Runtime.CompilerServices;
namespace IS
{
    class FinalRedFragment
    {
        static public void Init(){

        }

        static public void Update(){
            int entity_id = InternalCalls.GetCurrentEntityID();
            SimpleArray array = InternalCalls.GetCollidingEntityArray(entity_id);

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1)
            {


            }
        }
        
        static public void CleanUp(){

        }

    }
}

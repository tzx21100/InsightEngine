using System.Runtime.CompilerServices;
namespace IS
{
    class PushableNPC
    {
        static public void Init(){

        }

        static public void Update()
        {

            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            for (int i = 0; i < array.length; i++)
            {
                int entity =array.FindIndex(i);
                if(entity==InGameCutsceneRED.FINAL_SPIKE)
                {
                    InternalCalls.DestroyEntity(InternalCalls.GetCurrentEntityID());
                }
            }


        }

        static public void CleanUp(){

        }

    }
}

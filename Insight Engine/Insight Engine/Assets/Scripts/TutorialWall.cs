using System.Runtime.CompilerServices;
namespace IS
{
    class TutorialWall
    {
        static public int ID = InternalCalls.GetCurrentEntityID();

        static public void Init(){
            ID = InternalCalls.GetCurrentEntityID();
        }

        static public void Update(){
            PlayerScript.Health = PlayerScript.Max_Health;
            if (PlayerScript.ENEMY_SLAIN > 0)
            {
                InternalCalls.DestroyEntity(ID);
            }
        }
        
        static public void CleanUp(){

        }

    }
}

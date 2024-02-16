using System.Runtime.CompilerServices;
namespace IS
{
    class TutorialEnemy
    {
        static public int ID = InternalCalls.GetCurrentEntityID();
        static public void Init(){
            ID = InternalCalls.GetCurrentEntityID();
        }

        static public void Update(){
            
        }
        
        static public void CleanUp(){

        }

    }
}

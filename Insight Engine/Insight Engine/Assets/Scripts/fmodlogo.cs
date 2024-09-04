using System.Runtime.CompilerServices;
namespace IS
{
    class fmodlogo
    {

        static float timer = 1f;

        static public void Init(){
            timer = 1f;
        }

        static public void Update(){
            
            if(timer > 0f)
            {
                timer -= InternalCalls.GetDeltaTime();
                return;
            }

            InternalCalls.LoadScene("Assets/Scenes/MainMenu.insight");


        }
        
        static public void CleanUp(){

        }

    }
}

using System.Runtime.CompilerServices;
namespace IS
{
    class StartButtonScript
    {
        
        static public void Init(){

        }

        static public void Update(){
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {

                //hovering
            }
            if (InternalCalls.GetButtonState() == 2) {
                
                InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_1.wav");
                InternalCalls.LoadScene("Assets/Scenes/GameLevel.insight");
            }
        }
        
        static public void CleanUp(){

        }

    }
}

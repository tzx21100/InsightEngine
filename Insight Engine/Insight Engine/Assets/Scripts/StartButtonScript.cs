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
                InternalCalls.NativeLog("Button Hovered", 0);
                //Console.WriteLine("hovering!");
            }
            if (InternalCalls.GetButtonState() == 2) {
                InternalCalls.NativeLog("Button Clicked", 0);
                InternalCalls.AudioPlaySound("Footsteps-Grass-Far-Small_1.wav");
                //InternalCalls.LoadScene("../Scenes/GameLevel.insight");
                //InternalCalls.SwitchScene(0);
            }
        }
        
        static public void CleanUp(){

        }

    }
}

using System.Runtime.CompilerServices;
namespace IS
{
    class ButtonScript
    {
        
        static public void Init(){

        }

        static public void Update(){
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                InternalCalls.NativeLog("Button Hovered", 0);
            }
            if (InternalCalls.GetButtonState() == 2) {
                InternalCalls.NativeLog("Button Clicked", 0);
            }
        }
        
        static public void CleanUp(){

        }

    }
}

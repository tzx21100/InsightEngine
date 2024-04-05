using System.Runtime.CompilerServices;
namespace IS
{
    class ButtonScript
    {
        
        static public void Init(){

        }

        static public void Update(){
            //hovered
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Hovered)
            {
                SettingsScript.PlayHoverSound();
            }
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Pressed)
            {
                SettingsScript.PlayClickSound();
            }
        }
        
        static public void CleanUp(){

        }

    }
}

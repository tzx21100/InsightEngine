using System.Runtime.CompilerServices;
namespace IS
{
    class ResumeButtonScript
    {
        static public void Init()
        {
        }

        static public void Update()
        {
            if (PauseButtonScript.pause_enable)
            {
                //hovered
                if (InternalCalls.GetButtonState() == 1)
                {
                    //hovering
                }
                if (InternalCalls.GetButtonState() == 2)
                {
                    //click
                    InternalCalls.NativeLog("hello world!",1);
                    InternalCalls.AudioPlaySound("StartClick.wav");
                    PauseButtonScript.pause_enable = false;
                }
            }
        }


        static public void CleanUp()
        {

        }
    }
}

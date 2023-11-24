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

                //hovered
                if (InternalCalls.GetButtonState() == 1)
                {
                    //hovering
                }
                if (InternalCalls.GetButtonState() == 2)
                {
                    //click
                    PauseButtonScript.pause_enable = false;
                }
            
        }


        static public void CleanUp()
        {

        }
    }
}

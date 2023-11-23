using System.Runtime.CompilerServices;
namespace IS
{
    class HowToPlayScript
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
                }
            }
        }


        static public void CleanUp()
        {

        }
    }
}

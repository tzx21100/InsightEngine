using System.Runtime.CompilerServices;
namespace IS
{
    class NoButtonScript
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
                ExitButtonScript.exit_confirmation = false;
            }
        }


        static public void CleanUp()
        {

        }
    }
}

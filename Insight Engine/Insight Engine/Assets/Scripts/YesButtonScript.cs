using System.Runtime.CompilerServices;
namespace IS
{
    class YesButtonScript
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
                InternalCalls.Exit();
            }
        }


        static public void CleanUp()
        {

        }
    }
}

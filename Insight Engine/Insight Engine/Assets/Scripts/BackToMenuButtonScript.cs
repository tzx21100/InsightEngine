using System.Runtime.CompilerServices;
namespace IS
{
    class BackToMenuButtonScript
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
                //InternalCalls.LoadScene("Assets/Scenes/Main Menu.insight");
            }
        }


        static public void CleanUp()
        {

        }
    }
}

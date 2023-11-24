using System.Runtime.CompilerServices;
namespace IS
{
    class BackFromTutorialButtonScript
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
                HowToPlayScript.how_to_play_enable = false;
            }
        }


        static public void CleanUp()
        {

        }
    }
}

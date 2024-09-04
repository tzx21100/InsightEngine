using System.Runtime.CompilerServices;
namespace IS
{
    class NoButtonScript2
    {
        static public bool first_hovering = false;
        static public float font_size = MainMenuScript.NORMAL_FONT_SIZE;

        static public void Init()
        {
        }

        static public void Update()
        {
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hovering)
                {
                    SettingsScript.PlayHoverSound();
                    first_hovering = true;
                    font_size = MainMenuScript.HOVER_FONT_SIZE;
                }
            }
            else
            {
                first_hovering = false;
                font_size = MainMenuScript.NORMAL_FONT_SIZE;
            }
            if (!InternalCalls.IsWindowFocused())
            {
                first_hovering = true;
            }

            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                SettingsScript.PlayClickSound();
                BackToMenuButtonScript.exit_confirmation = false;
            }
        }


        static public void CleanUp()
        {

        }
    }
}

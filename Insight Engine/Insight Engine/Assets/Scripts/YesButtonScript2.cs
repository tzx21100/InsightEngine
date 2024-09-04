using System.Runtime.CompilerServices;
namespace IS
{
    class YesButtonScript2
    {
        static public bool first_hovering = false;
        static public float font_size = MainMenuScript.NORMAL_FONT_SIZE;

        static public void Init()
        {
        }

        static public void Update()
        {
            //hovered
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Hovered)
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
            // clicking
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Pressed)
            {
                //click
                SettingsScript.PlayClickSound();
                PlayerScript.CleanUp();
                InternalCalls.LoadScene("Assets/Scenes/MainMenu.insight");
                InternalCalls.AudioStopAllSounds();
            }
        }


        static public void CleanUp()
        {

        }
    }
}

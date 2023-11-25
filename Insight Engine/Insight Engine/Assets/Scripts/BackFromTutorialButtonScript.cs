using System.Runtime.CompilerServices;
namespace IS
{
    class BackFromTutorialButtonScript
    {
        static public bool first_hovering = false;

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
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
                    first_hovering = true;
                }
            }
            else
            {
                first_hovering = false;
            }

            // clicking
            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                HowToPlayScript.how_to_play_enable = false;
            }
        }


        static public void CleanUp()
        {

        }
    }
}

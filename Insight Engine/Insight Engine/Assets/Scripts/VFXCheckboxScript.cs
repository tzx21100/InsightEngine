using System.Runtime.CompilerServices;
namespace IS
{
    class VFXCheckboxScript
    {
        static public bool first_hover = false;
        static public bool toggled = false;
        static public SimpleImage checkbox_image = InternalCalls.GetSpriteImage("checkbox.png");
        static public SimpleImage toggled_image = InternalCalls.GetSpriteImage("checkbox_toggled.png");

        static public void Init()
        {

        }

        static public void Update()
        {
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hover)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
                    first_hover = true;
                }
            }
            else
            {
                first_hover = false;
            }

            // clicking
            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                toggled = !toggled;
            }
        }


        static public void CleanUp()
        {

        }
    }
}

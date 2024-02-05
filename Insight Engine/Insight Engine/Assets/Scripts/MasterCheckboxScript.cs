using System.Runtime.CompilerServices;
namespace IS
{
    class MasterCheckboxScript
    {
        static public bool first_hover = false;
        static public bool toggled = true;
        static public SimpleImage checkbox_image = InternalCalls.GetSpriteImage("checkbox.png");
        static public SimpleImage toggled_image = InternalCalls.GetSpriteImage("checkbox_toggled.png");
        static public bool clicked = false;
        static public void Init()
        {

        }

        static public void Update()
        {
            /*if (toggled)
            {
                BGMCheckboxScript.g
            }*/
            if (!toggled)
            {
                
                InternalCalls.SetSpriteImage(checkbox_image);
                SettingsScript.master_multiplier = 0f;

            }
            else
            {
                SettingsScript.master_multiplier= MasterSliderKnobScript.normalised_adjustment;
                InternalCalls.SetSpriteImage(toggled_image);
                

            }
            if (!BGMCheckboxScript.toggled||!VFXCheckboxScript.toggled) {
                toggled = false;
            }else if (BGMCheckboxScript.toggled && VFXCheckboxScript.toggled)
            {
                toggled= true;
            }
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hover)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
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
                //clicked = !clicked;
                //BGMCheckboxScript.clicked = clicked;
                //VFXCheckboxScript.clicked = clicked;
                toggled = !toggled;
                BGMCheckboxScript.toggled = toggled;
                VFXCheckboxScript.toggled = toggled;
                //click
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
            }
        }


        static public void CleanUp()
        {

        }
    }
}

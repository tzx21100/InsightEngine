using System.Runtime.CompilerServices;
namespace IS
{
    class HowToPlayButtonScript
    {

        static public bool first_hover = false;
        static public bool clicked = false;

        
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

            //click
            if (InternalCalls.GetButtonState() == 2)
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                //how_to_play_enabled = true;
                clicked = true;
            }
            
           
        }


        static public void CleanUp()
        {
        }


    }
}

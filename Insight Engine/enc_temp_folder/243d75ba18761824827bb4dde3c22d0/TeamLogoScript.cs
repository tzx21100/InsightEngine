using System.Runtime.CompilerServices;
namespace IS
{
    class TeamLogoScript
    {
        static public bool first_hover = false;
        static public bool is_lighted = false;
        static private int entity_id;

        static public void Init()
        {
            entity_id = InternalCalls.GetCurrentEntityID();
        }

        static public void Update()
        {
            /*//hovered
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
            }*/

            //click
            if (InternalCalls.GetButtonState() == 2)
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                InternalCalls.SetLightComponentToEntity(entity_id, 1, 1, 1, 1, 1);
            }
        }


        static public void CleanUp()
        {

        }
    }
}
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
            InternalCalls.SetLightToggleEntity(entity_id, is_lighted);
            InternalCalls.SetButtonSize(entity_id, new SimpleVector2D(684.29f, 286.05f));
        }

        static public void Update()
        {
            if (ExitButtonScript.exit_confirmation) {
                InternalCalls.ButtonComponentRemove(entity_id);
            }
            else
            {
                if (!InternalCalls.ButtonComponentExists(entity_id))
                {
                    InternalCalls.ButtonComponentAdd(entity_id, 0.8f, 1.0f, 0.9f, 1.0f, 1.150f);
                    InternalCalls.SetButtonSize(entity_id, new SimpleVector2D(684.29f, 286.05f));
                }
            }

            if (!InternalCalls.ButtonComponentExists(entity_id)) {
                InternalCalls.TransformSetScale(684.29f, 286.05f);
            }

            //click
            if (InternalCalls.GetButtonState() == 2)
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                is_lighted = !is_lighted;
                InternalCalls.SetLightToggleEntity(entity_id, is_lighted);
            }
        }


        static public void CleanUp()
        {

        }
    }
}
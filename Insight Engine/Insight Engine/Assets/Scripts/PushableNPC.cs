using System.Runtime.CompilerServices;
namespace IS
{
    class PushableNPC
    {

        static bool createdLastFragment=false;
        static bool textshow=false;
        static public void Init(){
            createdLastFragment = false;
            textshow=false;
            TextBox.Init();
        }

        static public void Update()
        {

/*            int entity_id=InternalCalls.GetCurrentEntityID();
            if (InternalCalls.GetCollidingEntityCheck(entity_id, PlayerScript.PLAYER_ID))
            {
                if (PlayerScript.isDashing)
                {
                    InternalCalls.RigidBodySetBodyTypeEntity(1, entity_id);
                }
                

            }*/

            if (InternalCalls.GetCollidingEntityCheck(InGameCutsceneRED.FINAL_SPIKE, PlayerScript.PLAYER_ID))
            {
                InternalCalls.LoadScene("Assets/Scenes/CreditsHappy.insight");
                return;
            }




            if (InternalCalls.GetTransformPosition().x < 780 &&!textshow)
            {
                InternalCalls.TransformSetPosition(-999999, 9999); //hide it away
                InternalCalls.DestroyEntity(InGameCutsceneRED.FINAL_SPIKE);

                TextBox.CreateTextBox("A BLUE fragment has appeared. You feel compelled to take it.");
                textshow = true;

                PlayerScript.isDashing = false;

            }

            if (TextBox.PAGE_NUMBER == 0 && textshow && TextBox.isVisible)
            {
                if (!createdLastFragment)
                {
                    int entt = InternalCalls.CreateEntityPrefab("NPCBlueFragment");
                    InternalCalls.TransformSetPositionEntity(700, -19, entt);
                    createdLastFragment = true;
                }
                Vector2D pan_position = new Vector2D(700, -19);
                CameraScript.CameraPanTo(pan_position, 1f);
            }

            if (TextBox.isVisible == false)
            {
                CameraScript.StopCameraPan();
            }


        }

        static public void CleanUp(){

        }

    }
}

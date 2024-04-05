using System.Runtime.CompilerServices;
namespace IS
{
    class PushableNPC
    {

        static bool createdLastFragment=false;
        static bool textshow=false;
        static bool happy_activated=false;
        static float tint_timer = 2f;
        static float tint_timer_set = 2f;
        static public void Init(){
            tint_timer = tint_timer_set;
            createdLastFragment = false;
            textshow=false;
            TextBox.Init();
            happy_activated = false;
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

            if (happy_activated)
            {
                if (tint_timer > 0f)
                {
                    tint_timer -= InternalCalls.GetDeltaTime();
                    InternalCalls.DrawImageAt(new SimpleVector2D(0, 0), 0, new SimpleVector2D(3840, 2160), InternalCalls.GetSpriteImage("credits_bg.jpg"), 1f,6);
                    float alpha = 1 - tint_timer / 2f;
                    if (alpha < 0.5f) { alpha = 0.5f; }
                    InternalCalls.SetLightsToggle(false);
                    InternalCalls.DrawSquare(0, 0, 20000, 20000, 0, 0, 0, alpha, 6);
                    CameraScript.CameraPanTo(new Vector2D(0, 0), 12f);
                    CameraScript.CameraTargetZoom(0.5f, 0.5f);
                    return;
                }
                CameraScript.StopCameraPan();
                InternalCalls.SetLightsToggle(true);
                InternalCalls.LoadScene("Assets/Scenes/CreditsHappy.insight");
                return;
            }

            if (InternalCalls.GetCollidingEntityCheck(InGameCutsceneRED.FINAL_SPIKE, PlayerScript.PLAYER_ID))
            {
               happy_activated =true;
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

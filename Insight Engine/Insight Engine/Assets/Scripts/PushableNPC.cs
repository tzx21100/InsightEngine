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
        static float tint_away = 2f;


        static public void Init(){
            tint_timer = tint_timer_set;
            tint_away = 0f;
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
                    InternalCalls.DrawImageAt(new SimpleVector2D(0, 0), 0, new SimpleVector2D(InternalCalls.GetWindowHeight()*3840 /2160, InternalCalls.GetWindowHeight()), InternalCalls.GetSpriteImage("credits_bg.jpg"), 1f,6);
                    InternalCalls.SetLightsToggle(false);
                    CameraScript.CameraPanTo(new Vector2D(0, 0), 12f);
                    InternalCalls.AttachCamera(0, 0);
                    CameraScript.CameraTargetZoom(1f, 0.5f);
                    if (tint_timer <= 0f)
                    {
                        TextBox.CreateTextBox("In your courageous sacrifice to become the final fragment to this broken world.");
                        TextBox.AddTextLines("You redeemed your loved ones, and tranquility returned to the village.");
                        TextBox.AddTextLines("Yet, admist the semblance of normalcy, there lingered a bittersweet ache.");
                        TextBox.AddTextLines("As if whispers of your presence were fading into the ether...");
                    }
                    return;
                }



                InternalCalls.DrawImageAt(new SimpleVector2D(0, 0), 0, new SimpleVector2D(InternalCalls.GetWindowHeight() * 3840 / 2160, InternalCalls.GetWindowHeight()), InternalCalls.GetSpriteImage("credits_bg.jpg"), 1f, InternalCalls.GetTopLayer()+2);

                CameraScript.CameraPanTo(new Vector2D(0, 0), 12f);
                CameraScript.CameraTargetZoom(1f, 0.5f);

                if (TextBox.isVisible == false)
                {
                    float alpha = 1f;
                    tint_away += InternalCalls.GetDeltaTime();
                    alpha = tint_away;
                    if (alpha > 0.5f) { alpha = 0.5f; }
                    InternalCalls.DrawSquare(0, 0, 20000, 20000, 0, 0, 0, alpha, InternalCalls.GetTopLayer()+2);
                    if (tint_away > 1f) {
                        CameraScript.StopCameraPan();
                        InternalCalls.SetLightsToggle(true);
                        InternalCalls.LoadScene("Assets/Scenes/CreditsHappy.insight");
                    }



                }

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

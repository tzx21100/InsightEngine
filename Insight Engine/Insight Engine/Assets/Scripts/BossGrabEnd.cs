using System.Runtime.CompilerServices;
namespace IS
{
    class BossGrabEnd
    {
        static float grow_timer = 2f;
        static float timer = 1f;
        static bool black_screen = false;

        static public void Init(){
            grow_timer = 2f;
            black_screen = false;
            timer = 0.65f;
            InternalCalls.ResetSpriteAnimationFrameEntity(InternalCalls.GetCurrentEntityID());
            InternalCalls.SetSpriteAlpha(0f);
        }

        static public void Update(){
            TextBox2.Update();

            if (black_screen)
            {
                InternalCalls.AttachCamera(0, 0);
                InternalCalls.DrawSquare(0, 0, 20000, 20000, 0, 0, 0, 1, 10);
                CameraScript.CameraPanTo(new Vector2D(0, 0), 20f);
                InternalCalls.SetLightsToggle(false);
                if (TextBox2.isVisible == false)
                {
                    InternalCalls.LoadScene("Assets/Scenes/Credits.insight");
                    InternalCalls.SetLightsToggle(true);
                    return;
                }

                return;
            }



            InternalCalls.TransformSetPosition(PlayerScript.player_pos.x, PlayerScript.player_pos.y);

            GrowAndAppear();
            if (grow_timer > 0f) { InternalCalls.ResetSpriteAnimationFrameEntity(InternalCalls.GetCurrentEntityID());  return; }

           timer -=InternalCalls.GetDeltaTime();
            if(InternalCalls.GetCurrentAnimationEntity(InternalCalls.GetCurrentEntityID()) == 10 )
            {
                InternalCalls.SetEntityLayer(InternalCalls.GetTopLayer(), InternalCalls.GetCurrentEntityID());
            }

            if(timer < 0f)
            {
                black_screen = true;
                TextBox2.CreateTextBox("Welcome PM, initializing new world...");
               // InternalCalls.LoadScene("Assets/Scenes/Credits.insight");
            }
        }
        
        static public void CleanUp(){

        }


        static private void GrowAndAppear()
        {


            if (grow_timer > 0f)
            {
                grow_timer -= InternalCalls.GetDeltaTime();
                SimpleVector2D scale = InternalCalls.GetTransformScaling();
                InternalCalls.SetSpriteAlpha((2 - grow_timer) * 0.5f);
                InternalCalls.TransformSetScale(scale.x * 1.01f, scale.y * 1.01f);
                return;
            }


        }


    }
}

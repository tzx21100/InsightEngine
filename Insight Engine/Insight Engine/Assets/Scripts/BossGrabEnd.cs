using System.Runtime.CompilerServices;
namespace IS
{
    class BossGrabEnd
    {
        static float grow_timer = 2f;
        static float timer = 1f;

        static public void Init(){
            grow_timer = 2f;
            timer = 0.65f;
            InternalCalls.ResetSpriteAnimationFrameEntity(InternalCalls.GetCurrentEntityID());
            InternalCalls.SetSpriteAlpha(0f);
        }

        static public void Update(){

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
                InternalCalls.LoadScene("Assets/Scenes/Credits.insight");
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

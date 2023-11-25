using System.ComponentModel;
using System.Runtime.CompilerServices;
namespace IS
{
    class FragmentScript
    {
        static float glitch_timer = 1f;
        static public void Init(){

        }

        static public void Update(){
            int entity = InternalCalls.GetCurrentEntityID();
            Vector2D my_position = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());
            if (InternalCalls.EntityCheckCollide(entity) && InternalCalls.GetCollidingEntityCheck(entity,PlayerScript.PLAYER_ID)) {
                if (!PlayerScript.Reward_DoubleJump) { PlayerScript.Reward_DoubleJump = true; } else
                {
                    if (!PlayerScript.Reward_WallClimb) {
                        PlayerScript.Reward_WallClimb = true;
                    }
                    else
                    {
                        if (!PlayerScript.Reward_Dash) {
                            PlayerScript.Reward_Dash = true;
                        }
                        else
                        {
                            InternalCalls.CameraSetZoom(1f);
                            InternalCalls.AttachCamera(0, 0);
                            InternalCalls.LoadScene("Assets/Scenes/YouWin.insight");
                        }
                    }

                }


                InternalCalls.GlitchEnable(false);

                InternalCalls.DestroyEntity(entity);
                
            }





            if (Vector2D.Distance(my_position, PlayerScript.player_pos) < 1500f && !PlayerScript.Reward_Dash)
            {
                glitch_timer = 1f;
            }

            if (glitch_timer > 0)
            {
                InternalCalls.GlitchEnable(true);
                glitch_timer -= InternalCalls.GetDeltaTime();
            }
            else
            {

                InternalCalls.GlitchEnable(false);
            }


        }
        
        static public void CleanUp(){

        }

    }
}

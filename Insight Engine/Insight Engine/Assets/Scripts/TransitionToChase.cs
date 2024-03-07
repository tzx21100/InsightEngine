using System.Runtime.CompilerServices;
namespace IS
{
    class TransitionToChase
    {
        static private float fade_out_timer=0f;
        static public void Init(){
            fade_out_timer = 0f;
        }

        static public void Update(){

            SimpleArray array = InternalCalls.GetCollidingEntityArray(InternalCalls.GetCurrentEntityID());

            if (array.FindIndex(PlayerScript.PLAYER_ID) != -1) {

                fade_out_timer += InternalCalls.GetDeltaTime();
            }

                InternalCalls.DrawSquare(CameraScript.camera_pos.x, CameraScript.camera_pos.y, 7000, 7000, 0, 0, 0, fade_out_timer, InternalCalls.GetTopLayer());

            if (fade_out_timer > 1.2f)
            {
                InternalCalls.LoadScene("Assets/Scenes/ChaseLevel.insight");
            }
        }
        
        static public void CleanUp(){


        }

    }
}

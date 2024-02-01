using System.Runtime.CompilerServices;
namespace IS
{
    class TutorialScript
    {

        static private float first_tutorial_x;
        static private float first_tutorial_y;
        static private float second_tutorial_x;
        static private float second_tutorial_y;
        static private int tutorial_state=0;

        static public void Init(){
            tutorial_state = 0;
        }

        static public void Update(){
            if (PlayerScript.isGrounded == true && tutorial_state==0 ) { tutorial_state = 1; }

            if (tutorial_state == 1)
            {
                InternalCalls.RenderText("Press A and D to move", 0.5f, 0.5f, 20, (1f, 1f, 1f));
                if(InternalCalls.KeyPressed((int)KeyCodes.A)|| InternalCalls.KeyPressed((int)KeyCodes.D) && PlayerScript.player_pos.x > -7300)
                {
                    tutorial_state++;
                }
            }

            if (tutorial_state == 2)
            {
                InternalCalls.RenderText("Press Space To Jump", 0.5f, 0.5f, 20, (1f, 1f, 1f));
                if (InternalCalls.KeyHeld((int)KeyCodes.Space) && InternalCalls.KeyHeld((int)KeyCodes.D) && PlayerScript.player_pos.x>-7100)
                {
                    tutorial_state++;
                }
            }

            if (tutorial_state == 3)
            {
                InternalCalls.RenderText("Long Press Space To Jump Further!", 0.5f, 0.5f, 20, (1f, 1f, 1f));
                if (InternalCalls.KeyHeld((int)KeyCodes.Space) && InternalCalls.KeyHeld((int)KeyCodes.D) && PlayerScript.player_pos.x > -6300)
                {
                    tutorial_state++;
                }
            }


        }
        
        static public void CleanUp(){

        }

    }
}

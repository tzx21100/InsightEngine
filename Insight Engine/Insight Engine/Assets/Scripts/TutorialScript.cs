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
        static private SimpleImage textboximage = InternalCalls.GetSpriteImage("textbox.png");

        static public void Init(){
            tutorial_state = 0;
        }

        static public void Update(){
            if (PlayerScript.isGrounded == true && tutorial_state==0 ) { tutorial_state = 1; }

            if (tutorial_state == 1)
            {
                SimpleVector2D textboxposition= new SimpleVector2D(CameraScript.camera_pos.x,CameraScript.camera_pos.y-InternalCalls.GetWindowHeight()/4);
                SimpleVector2D textboxscale = new SimpleVector2D(InternalCalls.GetWindowWidth() * 0.8f, InternalCalls.GetWindowHeight() * 0.2f);

                InternalCalls.DrawImageAt(textboxposition, 0, textboxscale, textboximage, 1, InternalCalls.GetTopLayer());
                InternalCalls.RenderTextFont("Press A and D to move", "MedusaGothic_D", 0.5f, 0.2f, 12.5f, (1f, 1f, 1f));
                if (PlayerScript.player_pos.x > 1000)
                {
                    tutorial_state++;
                }
            }

            if (tutorial_state == 2)
            {

                InternalCalls.RenderTextFont("Press Space To Jump!", "MedusaGothic_D", 0.5f, 0.66f, 12.5f, (1f, 1f, 1f));
                if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                {
                    tutorial_state++;
                }

            }
            if (tutorial_state == 3)
            {


                if (PlayerScript.player_pos.x > -6300)
                {
                    tutorial_state++;
                }


            }

            if (tutorial_state == 4)
            {

                //InternalCalls.RenderText("Hold Space To Jump Further!", 0.5f, 0.3f, 20, (1f, 1f, 1f));
                if (PlayerScript.player_pos.x > -4100)
                {
                    tutorial_state++;
                }
            }


        }
        
        static public void CleanUp(){

        }

    }
}

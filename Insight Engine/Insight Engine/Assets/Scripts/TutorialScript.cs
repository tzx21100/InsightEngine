using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class TutorialScript
    {
        private const float GAME_TITLE_SCALE = 2f;
        private const float GAME_TITLE_AR = 92f / 512f;

        static private int tutorial_state = 0;
/*      static private float first_tutorial_x;
        static private float first_tutorial_y;
        static private float second_tutorial_x;
        static private float second_tutorial_y;
        static private SimpleImage textboximage = InternalCalls.GetSpriteImage("textbox.png");
        static private bool tutorial_over = false;*/
        static private SimpleImage teamlogo = InternalCalls.GetSpriteImage("team_logo.png");
        //static private SimpleImage fragmentslogo = InternalCalls.GetSpriteImage("fragments_game_logo.png");
        static private SimpleImage fragmentslogo = InternalCalls.GetSpriteImage("GameTitle Animated 11R4C.png");

        static private float fragment_logo_timer = 3.12f;
        static private float fragment_fade_in = 1.2f;

        static private bool play_audio = false;

        static public void Init()
        {
            play_audio = false;
            InternalCalls.ShowCursor();
            tutorial_state = 0;
            fragment_logo_timer = 3.12f;
            fragment_fade_in = 1.2f;
        }

        static public void Update(){

            bool controller_connected = InternalCalls.ControllerConnected();

            if (PlayerScript.isGrounded == true && tutorial_state==0 ) { tutorial_state = 1; }
            if (tutorial_state == 1)
            {
                //SimpleVector2D textboxposition= new SimpleVector2D(CameraScript.camera_pos.x,CameraScript.camera_pos.y-InternalCalls.GetWindowHeight()/3f);
                //SimpleVector2D textboxscale = new SimpleVector2D(InternalCalls.GetWindowWidth() * 0.8f, InternalCalls.GetWindowHeight() * 0.2f);

                //InternalCalls.DrawImageAt(textboxposition, 0, textboxscale, textboximage, 1, InternalCalls.GetTopLayer());
                if (controller_connected)
                {
                    InternalCalls.RenderTextFont("Press the Left Buttons / Left joystick To Move!", "Semplicita_Light", 0.5f, 0.2f, 24f, (1f, 1f, 1f, 1f));
                }
                else
                {
                    InternalCalls.RenderTextFont("Press 'A' and 'D' to move", "Semplicita_Light", 0.5f, 0.2f, 24f, (1f, 1f, 1f, 1f));
                }
                
                if (PlayerScript.player_pos.x > -283)
                {
                    tutorial_state++;
                }
            }

            if (tutorial_state == 2)
            {

                if (controller_connected)
                {
                    InternalCalls.RenderTextFont("Press 'A' To Jump", "Semplicita_Light", 0.5f, 0.2f, 24f, (1f, 1f, 1f, 1f));
                }
                else
                {
                    InternalCalls.RenderTextFont("Press Space To Jump", "Semplicita_Light", 0.5f, 0.2f, 24f, (1f, 1f, 1f, 1f));
                }
                
                if (PlayerScript.jump_trigger)
                {
                    tutorial_state++;
                }

            }
            if (tutorial_state == 3)
            {

                if (controller_connected)
                {
                    InternalCalls.RenderTextFont("Hold The Jump Key To Jump Further", "Semplicita_Light", 0.5f, 0.2f, 24f, (1f, 1f, 1f, 1f));
                }
                else
                {

                    InternalCalls.RenderTextFont("Hold Space To Jump Further", "Semplicita_Light", 0.5f, 0.2f, 24f, (1f, 1f, 1f, 1f));
                }

                if (PlayerScript.player_pos.x >2220)
                {
                    tutorial_state++;
                }


            }

            if (tutorial_state == 4)
            {

                InternalCalls.RenderTextFont("Prepare for combat..", "Semplicita_Light", 0.5f, 0.2f, 24f, (1f, 1f, 1f, 1f));
                if (PlayerScript.player_pos.x > 3450)
                {
                    tutorial_state++;
                }
            }

            if (tutorial_state == 5)
            {

                if (controller_connected)
                {
                    InternalCalls.RenderTextFont("Press 'X' or RB to attack", "Semplicita_Light", 0.5f, 0.2f, 24f, (1f, 1f, 1f, 1f));
                }
                else
                {
                    InternalCalls.RenderTextFont("Left click to attack", "Semplicita_Light", 0.5f, 0.2f, 24f, (1f, 1f, 1f, 1f));
                }

                
                if (PlayerScript.attack_trigger)
                {
                    tutorial_state++;
                }
            }

            if (tutorial_state == 6)
            {


                if (PlayerScript.player_pos.x > 7279)
                {
                    SimpleVector2D pos = new SimpleVector2D(CameraScript.camera_pos.x - InternalCalls.GetWindowWidth() / 3f, CameraScript.camera_pos.y);
                    InternalCalls.DrawImageAt(pos, 0, new SimpleVector2D(870, 540), teamlogo, 1, InternalCalls.GetTopLayer());
                    if (play_audio == false)
                    {
                        InternalCalls.AudioPlayMusic("ENCOURAGEMENT - Corporate MSCCRP1_60.wav", 0.5f);
                        play_audio = true;
                    }
                    if (PlayerScript.player_pos.x > 11500)
                    {
                        InternalCalls.RenderTextFont("Presents", "MedusaGothic_D", 0.8f, 0.5f, 17.5f, (1f, 1f, 1f, 1f));
                    }
                    if (PlayerScript.player_pos.x > 14500)
                    {
                        tutorial_state++;
                    }

                }

            }

            if (tutorial_state == 7)
            {
                SimpleVector2D pos = new SimpleVector2D(PlayerScript.player_pos.x, PlayerScript.player_pos.y +InternalCalls.GetWindowHeight()/1.8f);
                float alpha = fragment_logo_timer;
                if (fragment_logo_timer > 1) { alpha = 1; }
                if (fragment_fade_in > 0) { alpha = 1 - fragment_fade_in; };

                float delta_time = InternalCalls.GetDeltaTime();
                float game_title_width = InternalCalls.GetWindowWidth()  * GAME_TITLE_SCALE;
                float game_title_height = game_title_width * GAME_TITLE_AR;
                InternalCalls.DrawNonEnityAnimationSet(delta_time, pos, 0, new SimpleVector2D(game_title_width, game_title_height), fragmentslogo, alpha, InternalCalls.GetTopLayer(), 11, 4, 2f);
                fragment_logo_timer -= delta_time;
                fragment_fade_in -= delta_time;
                if(fragment_logo_timer <= 0) {


                    tutorial_state++;
                }

            }


        }
        
        static public void CleanUp(){

        }

    }
}

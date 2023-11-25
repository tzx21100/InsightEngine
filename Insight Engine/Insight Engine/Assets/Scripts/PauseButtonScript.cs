using System.Drawing;
using System.Runtime.CompilerServices;
namespace IS
{
    class PauseButtonScript
    {
        // Pause Button
        static public Vector2D pos = new Vector2D(0f, 0f);
        static public bool pause_enable = false;
        static public bool fullscreen_mode = false;
        //static SimpleImage pause_button_image;

        // Pause Menu
        static private int pause_menu_entity;
        static private int resume_entity;
        static private int how_to_play_entity;
        //static private int setting_entity;
        static private int home_entity;
        static private int quit_entity;
        

        static SimpleImage pause_menu_image;
        static SimpleImage resume_image;
        static SimpleImage how_to_play_image;
        //static SimpleImage setting_image;
        static SimpleImage home_image;
        static SimpleImage quit_image;

        static Vector2D pause_menu_pos = new Vector2D(0, 0);
        static Vector2D resume_pos = new Vector2D(0,0);
        static Vector2D how_to_play_pos = new Vector2D(0,0);
        //static Vector2D setting_pos = new Vector2D(0,0);
        static Vector2D home_pos = new Vector2D(0,0);
        static Vector2D quit_pos = new Vector2D(0,0);

        // Camera Pos
        static Vector2D camera_pos = new Vector2D(0,0);

        static public void Init()
        {
            pause_enable = false;

            // Pause Menu
            pause_menu_image    = InternalCalls.GetSpriteImage("pause_menu.png");
            resume_image        = InternalCalls.GetSpriteImage("button_frame.png");
            how_to_play_image   = InternalCalls.GetSpriteImage("button_frame.png");
            //setting_image     = InternalCalls.GetSpriteImage("button_frame.png");
            home_image          = InternalCalls.GetSpriteImage("button_frame.png");
            quit_image          = InternalCalls.GetSpriteImage("exit_button_frame.png");

            pause_menu_entity   = InternalCalls.CreateEntityUI("Pause Menu", pause_menu_image);
            resume_entity       = InternalCalls.CreateEntityButton("Resume Game", resume_image, "ResumeButtonScript", "resume game");
            how_to_play_entity  = InternalCalls.CreateEntityButton("How to Play", how_to_play_image, "HowToPlayScript", "how to play");
            //setting_entity    = InternalCalls.CreateEntityButton("Setting", setting_image, "SettingScript", "settings");
            home_entity         = InternalCalls.CreateEntityButton("Main Menu", home_image, "BackToMenuButtonScript", "main menu");
            quit_entity         = InternalCalls.CreateEntityButton("Quit Game", quit_image, "ExitButtonScript", "quit game");

            InternalCalls.TransformSetScale(1000f, 1000f);
            InternalCalls.TransformSetScaleEntity(2200f, 1250f, pause_menu_entity);
            InternalCalls.TransformSetScaleEntity(600f, 200f, resume_entity);
            InternalCalls.TransformSetScaleEntity(450f, 150f, how_to_play_entity);
            //InternalCalls.TransformSetScaleEntity(450f, 150f, setting_entity);
            InternalCalls.TransformSetScaleEntity(450f, 150f, home_entity);
            InternalCalls.TransformSetScaleEntity(300f, 100f, quit_entity);
        }

        static public void Update()
        {
            ToggleFullscreenMode();

            // Constants
            const float PADDING = 16f;

            // Params used for offset computation
            float camera_zoom   = (float)InternalCalls.CameraGetZoom();
            float window_width  = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            float window_height = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            float pause_width   = InternalCalls.GetTransformScaling().x;
            float pause_height  = InternalCalls.GetTransformScaling().y;
            bool is_fullscreen  = InternalCalls.IsFullscreen();

            // Compute pause button position offset
            float offset_xpos   = window_width - pause_width / 2f - PADDING;
            float offset_ypos   = window_height - pause_height / 2f - PADDING;

            // Get camera position
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Offset pause button position
            InternalCalls.TransformSetPosition(camera_pos.x + offset_xpos, camera_pos.y + offset_ypos);
            

            pause_menu_pos.Set(camera_pos.x, camera_pos.y);
            resume_pos.Set(camera_pos.x - 10f, camera_pos.y + 170f);
            how_to_play_pos.Set(camera_pos.x - 10f, camera_pos.y - 20);
            //setting_pos.Set(camera_pos.x - 10f, camera_pos.y - 190f);
            home_pos.Set(camera_pos.x - 10f, camera_pos.y - 190f);
            quit_pos.Set(camera_pos.x - 10f, camera_pos.y - 340f);

            if (InternalCalls.KeyPressed((int)KeyCodes.Escape))
            {
                pause_enable = !pause_enable;
                ExitButtonScript.exit_confirmation = false;
                HowToPlayScript.how_to_play_enable = false;
            }

            //mouse
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
            }
            if (InternalCalls.GetButtonState() == 2) 
            {    
                if (!pause_enable)
                {
                    //InternalCalls.SetGameStatus(false);
                    pause_enable = true;
                }
                else
                {
                    pause_enable = false;
                }
            }

            if (pause_enable) // if game paused
            {

                /*// RESUME BUTTON
                if (InternalCalls.GetEntityButtonState(resume_entity) == 1)
                {
                    //hovering
                }
                if (InternalCalls.GetEntityButtonState(resume_entity) == 2)
                {
                    //click
                }*/
                InternalCalls.GamePause(true);
                if (ExitButtonScript.exit_confirmation || HowToPlayScript.how_to_play_enable)
                {
                    // move away the pause button
                    InternalCalls.TransformSetPosition(9999f, 9999f);
                    MoveAwayPauseMenuPosition();
                }
                else
                {
                    SetPauseMenuPosition();
                }
                

            }
            else
            {
                InternalCalls.GamePause(false);
                MoveAwayPauseMenuPosition();
            }
        }

        static public void CleanUp()
        {
            // Empty for now
        }

        static public void SetPauseMenuPosition()
        {
            // move away the pause button
            InternalCalls.TransformSetPosition(9999f, 9999f);

            InternalCalls.TransformSetPositionEntity(pause_menu_pos.x, pause_menu_pos.y, pause_menu_entity);
            InternalCalls.TransformSetPositionEntity(resume_pos.x, resume_pos.y, resume_entity);
            InternalCalls.TransformSetPositionEntity(how_to_play_pos.x, how_to_play_pos.y, how_to_play_entity);
            //InternalCalls.TransformSetPositionEntity(setting_pos.x, setting_pos.y, setting_entity);
            InternalCalls.TransformSetPositionEntity(home_pos.x, home_pos.y, home_entity);
            InternalCalls.TransformSetPositionEntity(quit_pos.x, quit_pos.y, quit_entity);

            // draw text
            //InternalCalls.ButtonRenderText(paused_text, 0.49f, 0.72f, 30f, (1f, 1f, 1f));
            InternalCalls.RenderText("paused", 0.495f, 0.7f, 30f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(resume_entity, 0.495f, 0.595f, 19f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(how_to_play_entity, 0.495f, 0.475f, 14f, (1f, 1f, 1f));
            //InternalCalls.ButtonRenderText(setting_entity, 0.495f, 0.365f, 14f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(home_entity, 0.495f, 0.367f, 14f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(quit_entity, 0.495f, 0.276f, 9f, (1f, 1f, 1f));
        }

        static public void MoveAwayPauseMenuPosition()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, pause_menu_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, resume_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_entity);
            //InternalCalls.TransformSetPositionEntity(9999f, 9999f, setting_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, home_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, quit_entity);
        }

        static public void ToggleFullscreenMode()
        {
            if (InternalCalls.KeyPressed((int)KeyCodes.F11))
            {
                fullscreen_mode = !fullscreen_mode;
                InternalCalls.ToggleFullscreen(fullscreen_mode);
            }
        }

    }
}

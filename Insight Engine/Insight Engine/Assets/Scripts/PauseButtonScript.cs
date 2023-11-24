using System.Drawing;
using System.Runtime.CompilerServices;
namespace IS
{
    class PauseButtonScript
    {
        // Pause Button
        static public Vector2D pos = new Vector2D(0f, 0f);
        static public Vector2D pos_offset = new Vector2D(1200f, 710f);
        static public bool pause_enable = false;
        static public float title_bar_height = 0f;
        //static SimpleImage pause_button_image;

        // Pause Menu
        static private int pause_menu_entity;
        static private int resume_entity;
        static private int how_to_play_entity;
        static private int setting_entity;
        static private int home_entity;
        static private int quit_entity;

        static SimpleImage pause_menu_image;
        static SimpleImage resume_image;
        static SimpleImage how_to_play_image;
        static SimpleImage setting_image;
        static SimpleImage home_image;
        static SimpleImage quit_image;

        static Vector2D pause_menu_pos = new Vector2D(0, 0);
        static Vector2D resume_pos = new Vector2D(0,0);
        static Vector2D how_to_play_pos = new Vector2D(0,0);
        static Vector2D setting_pos = new Vector2D(0,0);
        static Vector2D home_pos = new Vector2D(0,0);
        static Vector2D quit_pos = new Vector2D(0,0);

        // Windows
        static int win_width;
        static int win_height;

        // Camera Pos
        static Vector2D camera_pos = new Vector2D(0,0);

        static public void Init(){

            title_bar_height = InternalCalls.GetTitleBarHeight();

            // Pause Menu
            pause_menu_image = InternalCalls.GetSpriteImage("pause_menu.png");
            resume_image = InternalCalls.GetSpriteImage("button_frame.png");
            how_to_play_image = InternalCalls.GetSpriteImage("button_frame.png");
            setting_image = InternalCalls.GetSpriteImage("button_frame.png");
            home_image = InternalCalls.GetSpriteImage("button_frame.png");
            quit_image = InternalCalls.GetSpriteImage("exit_button_frame.png");

            pause_menu_entity = InternalCalls.CreateEntityUI("Pause Menu", pause_menu_image);

            resume_entity = InternalCalls.CreateEntityButton("Resume Game", resume_image, "Assets/Scripts/ResumeButtonScript.cs", "Resume Game");
            how_to_play_entity = InternalCalls.CreateEntityButton("How to Play", how_to_play_image, "Assets/Scripts/HowToPlayScript.cs", "How To Play");
            setting_entity = InternalCalls.CreateEntityButton("Setting", setting_image, "Assets/Scripts/SettingScript.cs", "Setting");
            home_entity = InternalCalls.CreateEntityButton("Home", home_image, "Assets/Scripts/HomeButtonScript.cs", "Home");
            quit_entity = InternalCalls.CreateEntityButton("Quit Game", quit_image, "Assets/Scripts/ExitButtonScript.cs", "Quit Game");

            InternalCalls.TransformSetScale(1000f, 1000f);
            InternalCalls.TransformSetScaleEntity(2200f, 1300f, pause_menu_entity);
            InternalCalls.TransformSetScaleEntity(300f, 100f, resume_entity);
            InternalCalls.TransformSetScaleEntity(300f, 100f, how_to_play_entity);
            InternalCalls.TransformSetScaleEntity(300f, 100f, setting_entity);
            InternalCalls.TransformSetScaleEntity(300f, 100f, home_entity);
            InternalCalls.TransformSetScaleEntity(200f, 70f, quit_entity);

            // Windows
            win_width = InternalCalls.GetWindowWidth();
            win_height = InternalCalls.GetWindowHeight();
        }



        static public void Update(){

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            pos = camera_pos.Add(pos_offset);
            InternalCalls.TransformSetPosition(pos.x, pos.y - title_bar_height);
            

            pause_menu_pos.Set(camera_pos.x, camera_pos.y + 50f);
            resume_pos.Set(camera_pos.x - 10f, camera_pos.y + 350f);
            how_to_play_pos.Set(camera_pos.x - 10f, camera_pos.y + 200f);
            setting_pos.Set(camera_pos.x - 10f, camera_pos.y + 50f);
            home_pos.Set(camera_pos.x - 10f, camera_pos.y - 100f);
            quit_pos.Set(camera_pos.x - 10f, camera_pos.y - 250f);

            if (InternalCalls.KeyPressed((int)KeyCodes.Escape))
            {
                pause_enable = !pause_enable;
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
                SetPauseMenuPosition();

            }
            else
            {
                InternalCalls.GamePause(false);
                MoveAwayPauseMenuPosition();
            }
        }

        static public void CleanUp(){
            InternalCalls.DestroyEntity(pause_menu_entity);
            InternalCalls.DestroyEntity(resume_entity);
            InternalCalls.DestroyEntity(how_to_play_entity);
            InternalCalls.DestroyEntity(setting_entity);
            InternalCalls.DestroyEntity(home_entity);
            InternalCalls.DestroyEntity(quit_entity);
        }

        static public void SetPauseMenuPosition()
        {
            // move away the pause button
            InternalCalls.TransformSetPosition(9999f, 9999f);

            InternalCalls.TransformSetPositionEntity(pause_menu_pos.x, pause_menu_pos.y, pause_menu_entity);
            InternalCalls.TransformSetPositionEntity(resume_pos.x, resume_pos.y, resume_entity);
            InternalCalls.TransformSetPositionEntity(how_to_play_pos.x, how_to_play_pos.y, how_to_play_entity);
            InternalCalls.TransformSetPositionEntity(setting_pos.x, setting_pos.y, setting_entity);
            InternalCalls.TransformSetPositionEntity(home_pos.x, home_pos.y, home_entity);
            InternalCalls.TransformSetPositionEntity(quit_pos.x, quit_pos.y, quit_entity);

            InternalCalls.ButtonRenderText(resume_entity, 0.5f, (resume_pos.y + (float)win_height / 2f) / (float)win_height, 48f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(how_to_play_entity, (how_to_play_pos.x + (float)win_width / 2f) / (float)win_width, (how_to_play_pos.y + (float)win_height / 2f) / (float)win_height, 48f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(setting_entity, (setting_pos.x + (float)win_width / 2f) / (float)win_width, (setting_pos.y + (float)win_height / 2f) / (float)win_height, 48f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(home_entity, (home_pos.x + (float)win_width / 2f) / (float)win_width, (home_pos.y + (float)win_height / 2f) / (float)win_height, 48f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(quit_entity, (quit_pos.x + (float)win_width / 2f) / (float)win_width, (quit_pos.y + (float)win_height / 2f) / (float)win_height, 28f, (1f, 1f, 1f));
        }

        static public void MoveAwayPauseMenuPosition()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, pause_menu_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, resume_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, setting_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, home_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, quit_entity);
        }

        static public void DrawPauseMenu()
        {
            //InternalCalls.DrawImageAt(InternalCalls.GetTransformPositionEntity(pause_menu_entity), 0f, InternalCalls.GetTransformScalingEntity(pause_menu_entity), pause_menu_image, 1f, 4);
            /*InternalCalls.DrawImageAt(InternalCalls.GetTransformPositionEntity(resume_entity), 0f, InternalCalls.GetTransformScalingEntity(resume_entity), resume_image, 1f, 4);
            InternalCalls.DrawImageAt(InternalCalls.GetTransformPositionEntity(how_to_play_entity), 0f, InternalCalls.GetTransformScalingEntity(how_to_play_entity), how_to_play_image, 1f, 4);
            InternalCalls.DrawImageAt(InternalCalls.GetTransformPositionEntity(setting_entity), 0f, InternalCalls.GetTransformScalingEntity(setting_entity), setting_image, 1f, 4);
            InternalCalls.DrawImageAt(InternalCalls.GetTransformPositionEntity(home_entity), 0f, InternalCalls.GetTransformScalingEntity(home_entity), home_image, 1f, 4);
            InternalCalls.DrawImageAt(InternalCalls.GetTransformPositionEntity(quit_entity), 0f, InternalCalls.GetTransformScalingEntity(quit_entity), quit_image, 1f, 4);*/
        }

    }
}

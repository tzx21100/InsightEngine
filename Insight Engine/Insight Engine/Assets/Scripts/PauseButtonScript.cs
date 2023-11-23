using System.Runtime.CompilerServices;
namespace IS
{
    class PauseButtonScript
    {
        // Pause Button
        static public Vector2D pos = new Vector2D(0f, 0f);
        static public Vector2D pos_offset = new Vector2D(1100f, 500f);
        static public bool pause_enable = false;
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

        static public void Init(){

            // Pause Menu
            pause_menu_image = InternalCalls.GetSpriteImage("pause_menu.png");
            resume_image = InternalCalls.GetSpriteImage("resume.png");
            how_to_play_image = InternalCalls.GetSpriteImage("start_button.png");
            setting_image = InternalCalls.GetSpriteImage("start_button.png");
            home_image = InternalCalls.GetSpriteImage("start_button.png");
            quit_image = InternalCalls.GetSpriteImage("exit_button.png");

            pause_menu_entity = InternalCalls.CreateEntityUI("Pause Menu", pause_menu_image);

            resume_entity = InternalCalls.CreateEntityButton("Resume Game", resume_image, "ResumeButtonScript.cs");
            how_to_play_entity = InternalCalls.CreateEntityButton("How to Play", how_to_play_image, "HowToPlayScript.cs");
            setting_entity = InternalCalls.CreateEntityButton("Setting", setting_image, "SettingScript.cs");
            home_entity = InternalCalls.CreateEntityButton("Home", home_image, "HomeButtonScript.cs");
            quit_entity = InternalCalls.CreateEntityButton("Quit Game", quit_image, "ExitButtonScript.cs");

            InternalCalls.TransformSetScale(1000f, 1000f);
            InternalCalls.TransformSetScaleEntity(2200f, 1200f, pause_menu_entity);
            InternalCalls.TransformSetScaleEntity(300f, 100f, resume_entity);
            InternalCalls.TransformSetScaleEntity(300f, 100f, how_to_play_entity);
            InternalCalls.TransformSetScaleEntity(300f, 100f, setting_entity);
            InternalCalls.TransformSetScaleEntity(300f, 100f, home_entity);
            InternalCalls.TransformSetScaleEntity(200f, 70f, quit_entity);
        }

        static public void Update(){
            pos = PlayerScript.camera_pos.Add(pos_offset);
            InternalCalls.TransformSetPosition(pos.x, pos.y);
            
            

            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
            }
            if (InternalCalls.GetButtonState() == 2) {
                
                InternalCalls.AudioPlaySound("StartClick.wav");
                //InternalCalls.LoadScene("Assets/Scenes/GameLevel.insight");
                //InternalCalls.Exit();
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

            if (pause_enable)
            {
                //DrawPauseMenu();

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
            /*InternalCalls.DestroyEntity(resume_entity);
            InternalCalls.DestroyEntity(how_to_play_entity);
            InternalCalls.DestroyEntity(setting_entity);
            InternalCalls.DestroyEntity(home_entity);
            InternalCalls.DestroyEntity(quit_entity);*/
        }

        static public void SetPauseMenuPosition()
        {
            InternalCalls.TransformSetPositionEntity(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y, pause_menu_entity);
            InternalCalls.TransformSetPositionEntity(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y + 400f, resume_entity);
            InternalCalls.TransformSetPositionEntity(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y + 200f, how_to_play_entity);
            InternalCalls.TransformSetPositionEntity(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y + 0f, setting_entity);
            InternalCalls.TransformSetPositionEntity(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y - 200f, home_entity);
            InternalCalls.TransformSetPositionEntity(PlayerScript.camera_pos.x, PlayerScript.camera_pos.y - 400f, quit_entity);
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

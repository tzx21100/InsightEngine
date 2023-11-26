/*!
 * \file PauseButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the PauseButtonScript class, used for the pause button in game 
 * and trigger the pause menu
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
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
        static float camera_zoom = 0f;

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D orgin = new Vector2D(0, 0);

        static public void Init()
        {
            pause_enable = false;

            // Pause Menu
            pause_menu_image    = InternalCalls.GetSpriteImage("paused_menu_image.png");
            resume_image        = InternalCalls.GetSpriteImage("resume_game_button.png");
            how_to_play_image   = InternalCalls.GetSpriteImage("menu_how_to_play_button.png");
            //setting_image     = InternalCalls.GetSpriteImage("button_frame.png");
            home_image          = InternalCalls.GetSpriteImage("main_menu_button.png");
            quit_image          = InternalCalls.GetSpriteImage("quit_game_button.png");

            pause_menu_entity   = InternalCalls.CreateEntityUI("Pause Menu", pause_menu_image);
            resume_entity       = InternalCalls.CreateEntityButton("Resume Game", resume_image, "ResumeButtonScript", "");
            how_to_play_entity  = InternalCalls.CreateEntityButton("How to Play", how_to_play_image, "HowToPlayScript", "");
            //setting_entity    = InternalCalls.CreateEntityButton("Setting", setting_image, "SettingScript", "settings");
            home_entity         = InternalCalls.CreateEntityButton("Main Menu", home_image, "BackToMenuButtonScript", "");
            quit_entity         = InternalCalls.CreateEntityButton("Quit Game", quit_image, "ExitButtonScript", "");

            
            
        }

        static public void Update()
        {
            ToggleFullscreenMode();

            // Constants
            //const float PADDING = 32f;

            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            // Get camera position
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() * 2f / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() * 2f / camera_zoom;

            orgin.x = camera_pos.x - (win_dimension.x / 2f);
            orgin.y = camera_pos.y - (win_dimension.y / 2f);

            // Dimensions
            InternalCalls.TransformSetScale(0.04f * win_dimension.x, 0.07f * win_dimension.y);
            InternalCalls.TransformSetScaleEntity(0.85f * win_dimension.x, 0.75f * win_dimension.y, pause_menu_entity);
            InternalCalls.SetButtonSize(resume_entity, new SimpleVector2D(0.21f * win_dimension.x, 0.12f * win_dimension.y));
            InternalCalls.SetButtonSize(how_to_play_entity, new SimpleVector2D(0.15f * win_dimension.x, 0.09f * win_dimension.y));
            //InternalCalls.TransformSetScaleEntity(450f, 150f, setting_entity);
            InternalCalls.SetButtonSize(home_entity, new SimpleVector2D(0.15f * win_dimension.x, 0.09f * win_dimension.y));
            InternalCalls.SetButtonSize(quit_entity, new SimpleVector2D(0.13f * win_dimension.x, 0.07f * win_dimension.y));

            // Params used for offset computation
            /*float window_width = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            float window_height = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            float pause_width = InternalCalls.GetTransformScaling().x;
            float pause_height = InternalCalls.GetTransformScaling().y;*/

            // Compute pause button position offset
            float offset_xpos = 0.47f * win_dimension.x;
            float offset_ypos = 0.45f * win_dimension.y;

            // Offset pause button position
            InternalCalls.TransformSetPosition(camera_pos.x + offset_xpos, camera_pos.y + offset_ypos);
            
            // Positions
            pause_menu_pos.Set(camera_pos.x, camera_pos.y);
            resume_pos.Set(camera_pos.x - 0.005f * win_dimension.x, camera_pos.y + 0.1f * win_dimension.y);
            how_to_play_pos.Set(camera_pos.x - 0.005f * win_dimension.x, camera_pos.y - 0.01f * win_dimension.y);
            //setting_pos.Set(camera_pos.x - 10f, camera_pos.y - 190f);
            home_pos.Set(camera_pos.x - 0.005f * win_dimension.x, camera_pos.y - 0.105f * win_dimension.y);
            quit_pos.Set(camera_pos.x - 0.005f * win_dimension.x, camera_pos.y - 0.19f * win_dimension.y);

            if (InternalCalls.KeyPressed((int)KeyCodes.Escape))
            {
                pause_enable = !pause_enable;
                ExitButtonScript.exit_confirmation = false;
                HowToPlayScript.how_to_play_enable = false;
            }

            if (!InternalCalls.IsWindowFocused())
            {
                pause_enable = true;
            }

            //mouse
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                InternalCalls.TransformSetScale(0.06f * win_dimension.x, 0.1f * win_dimension.y);
            }
            if (InternalCalls.GetButtonState() == 2) 
            {
                InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
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
            //InternalCalls.RenderText("paused", 0.5f, 0.69f, 30f, (1f, 1f, 1f));
            /*InternalCalls.ButtonRenderText(resume_entity, 0.495f, 0.595f, 19f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(how_to_play_entity, 0.495f, 0.475f, 14f, (1f, 1f, 1f));
            //InternalCalls.ButtonRenderText(setting_entity, 0.495f, 0.365f, 14f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(home_entity, 0.495f, 0.367f, 14f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(quit_entity, 0.495f, 0.276f, 9f, (1f, 1f, 1f));*/
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

/*!
 * \file PauseButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (70%), Matthew Ng, matthewdeen.ng@digipen.edu (30%)
 * \par Course: CSD2451
 * \brief
 * This header file contains the PauseButtonScript class, used for the pause button in game 
 * and trigger the pause menu
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System.Runtime.CompilerServices;
using System;
using System.Collections.Generic;
namespace IS
{
    class PauseButtonScript
    {
        // Pause Button
        static public bool first_hover = false;
        static public Vector2D pause_button_pos = new Vector2D(0f, 0f);
        static public bool paused = false;
        static public SimpleImage paused_button_image = InternalCalls.GetSpriteImage("pause_button.png");
        static public SimpleImage paused_button_clicked_image = InternalCalls.GetSpriteImage("pause_button_clicked.png");
        // Pause Menu
        static private int pause_menu_entity;
        static private int resume_button_entity;
        static private int how_to_play_button_entity;
        static private int settings_button_entity;
        static private int back_to_menu_button_entity;
        static private int exit_button_entity;


        static SimpleImage pause_menu_image;
        static SimpleImage button_frame;

        static Vector2D pause_menu_pos = new Vector2D(0, 0);
        static Vector2D resume_button_pos = new Vector2D(0, 0);
        static Vector2D how_to_play_button_pos = new Vector2D(0, 0);
        static Vector2D settings_button_pos = new Vector2D(0, 0);
        static Vector2D back_to_menu_button_pos = new Vector2D(0, 0);
        static Vector2D exit_button_pos = new Vector2D(0, 0);

        // Camera Pos
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D orgin = new Vector2D(0, 0);
        static public bool light_switch = true;

        static public void Init()
        {
            paused = false;
            light_switch = true;

            // Pause Menu
            pause_menu_image = InternalCalls.GetSpriteImage("pause_menu_image.png");
            button_frame = InternalCalls.GetSpriteImage("button_frame.png");
            pause_menu_entity = InternalCalls.CreateEntityUI("Pause Menu", pause_menu_image);
            resume_button_entity = InternalCalls.CreateEntityButtonNoText("Resume Game", button_frame, "ResumeButtonScript");
            settings_button_entity = InternalCalls.CreateEntityButtonNoText("Setting", button_frame, "SettingsScript");
            how_to_play_button_entity = InternalCalls.CreateEntityButtonNoText("How to Play Button", button_frame, "HowToPlayScript");
            back_to_menu_button_entity = InternalCalls.CreateEntityButtonNoText("Main Menu", button_frame, "BackToMenuButtonScript");
            exit_button_entity = InternalCalls.CreateEntityButtonNoText("Quit Game", button_frame, "ExitButtonScript");



        }

        static public void Update()
        {
            int pause_button = InternalCalls.GetCurrentEntityID();
            InternalCalls.SetEntityLayer(InternalCalls.GetTopLayer(), pause_button);
            ToggleFullscreenMode();

            //set the image
            InternalCalls.SetSpriteImage(paused_button_image);

            // Camera
            camera_zoom = CameraScript.camera_zoom;

            // Get camera position 
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            orgin.x = camera_pos.x - (win_dimension.x / 2f);
            orgin.y = camera_pos.y - (win_dimension.y / 2f);

            // Size
            const float PAUSE_BUTTON_SCALE = 0.08f;
            float width = PAUSE_BUTTON_SCALE * win_dimension.x;
            InternalCalls.SetButtonSize(pause_button, new SimpleVector2D(width, width));
            InternalCalls.TransformSetScaleEntity(1.25f * win_dimension.x, 1.15f * win_dimension.y, pause_menu_entity);
            InternalCalls.SetButtonSize(resume_button_entity, new SimpleVector2D(0.21f * win_dimension.x, 0.12f * win_dimension.y));
            InternalCalls.SetButtonSize(settings_button_entity, new SimpleVector2D(0.15f * win_dimension.x, 0.09f * win_dimension.y));
            InternalCalls.SetButtonSize(how_to_play_button_entity, new SimpleVector2D(0.15f * win_dimension.x, 0.09f * win_dimension.y));
            InternalCalls.SetButtonSize(back_to_menu_button_entity, new SimpleVector2D(0.15f * win_dimension.x, 0.09f * win_dimension.y));
            InternalCalls.SetButtonSize(exit_button_entity, new SimpleVector2D(0.13f * win_dimension.x, 0.07f * win_dimension.y));

            // Compute pause button position offset
            float offset_xpos = 0.47f * win_dimension.x;
            float offset_ypos = 0.4f * win_dimension.y;

            // Offset pause button position
            InternalCalls.TransformSetPosition(camera_pos.x + offset_xpos, camera_pos.y + offset_ypos);

            // Positions
            pause_menu_pos.Set(camera_pos.x, camera_pos.y);
            resume_button_pos.Set(camera_pos.x - 0.005f * win_dimension.x, camera_pos.y + 0.12f * win_dimension.y);
            settings_button_pos.Set(camera_pos.x - 0.005f * win_dimension.x, camera_pos.y + 0.015f * win_dimension.y);
            how_to_play_button_pos.Set(camera_pos.x - 0.005f * win_dimension.x, camera_pos.y - 0.07f * win_dimension.y);
            back_to_menu_button_pos.Set(camera_pos.x - 0.005f * win_dimension.x, camera_pos.y - 0.155f * win_dimension.y);
            exit_button_pos.Set(camera_pos.x - 0.005f * win_dimension.x, camera_pos.y - 0.235f * win_dimension.y);

            if (InternalCalls.KeyPressed((int)KeyCodes.Escape) || InternalCalls.ControllerKeyPressed((int)KeyCodes.Button_START))
            {
                paused = !paused;
                SettingsScript.show_settings = false;
                ExitButtonScript.exit_confirmation = false;
                HowToPlayScript.show_how_to_play = false;
                light_switch = !light_switch;
                InternalCalls.SetLightsToggle(light_switch);
            }

            if (!InternalCalls.IsWindowFocused())
            {
                paused = true;

            }
            //hovered
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Hovered)
            {
                InternalCalls.TransformSetScale(0.06f * win_dimension.x, 0.1f * win_dimension.y);
                InternalCalls.SetSpriteImage(paused_button_clicked_image);
                //hovering
                if (!first_hover)
                {
                    SettingsScript.PlayHoverSound();
                    first_hover = true;
                }
            }
            else
            {
                first_hover = false;
            }
            if (!InternalCalls.IsWindowFocused())
            {
                first_hover = true;
            }

            //click
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Pressed)
            {
                //light_switch = !light_switch;
                InternalCalls.SetLightsToggle(false);

                SettingsScript.PlayClickSound();
                InternalCalls.SetSpriteImage(paused_button_clicked_image);

                if (!paused)
                {
                    paused = true;

                }
                else
                {
                    paused = false;

                    return;
                }
            }
            
            if (paused) // if game paused
            {
                light_switch = false;
                //InternalCalls.SetLightsToggle(light_switch);
                InternalCalls.SetLightsToggle(false);

                InternalCalls.GamePause(true);
                if (SettingsScript.show_settings||HowToPlayScript.show_how_to_play||ExitButtonScript.exit_confirmation)
                {
                    // move away the pause button
                    InternalCalls.TransformSetPosition(9999f, 9999f);
                    HidePauseMenu();

                }
                else
                {
                    DrawPauseMenu();
                }
                

            }
            else
            {
                light_switch = true;
                //InternalCalls.SetLightsToggle(light_switch); 
                InternalCalls.GamePause(false);
                HidePauseMenu();
            }
            

        }

        static public void CleanUp()
        {
            // Empty for now
        }

        static public void DrawPauseMenu()
        {
            // move away the pause button
            InternalCalls.TransformSetPosition(9999f, 9999f);

            InternalCalls.TransformSetPositionEntity(pause_menu_pos.x, pause_menu_pos.y, pause_menu_entity);
            InternalCalls.TransformSetPositionEntity(resume_button_pos.x, resume_button_pos.y, resume_button_entity);
            InternalCalls.TransformSetPositionEntity(settings_button_pos.x, settings_button_pos.y, settings_button_entity);
            InternalCalls.TransformSetPositionEntity(how_to_play_button_pos.x, how_to_play_button_pos.y, how_to_play_button_entity);
            InternalCalls.TransformSetPositionEntity(back_to_menu_button_pos.x, back_to_menu_button_pos.y, back_to_menu_button_entity);
            InternalCalls.TransformSetPositionEntity(exit_button_pos.x, exit_button_pos.y, exit_button_entity);

            //InternalCalls.RenderTextFont("PAUSED", "MedusaGothic_D", 0.497f, 0.72f, 12f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("RESUME GAME", SettingsScript.TITLE_FONT_NAME, 0.497f, 0.625f, ResumeButtonScript.font_size, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("SETTINGS", SettingsScript.TITLE_FONT_NAME, 0.497f, 0.52f, SettingsScript.font_size, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("HOW TO PLAY", SettingsScript.TITLE_FONT_NAME, 0.497f, 0.433f, HowToPlayScript.font_size, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("MAIN MENU", SettingsScript.TITLE_FONT_NAME, 0.497f, 0.35f, BackToMenuButtonScript.font_size, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("EXIT", SettingsScript.TITLE_FONT_NAME, 0.497f, 0.267f, ExitButtonScript.font_size, (1f, 1f, 1f, 1f));

        }

        static public void HidePauseMenu()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, pause_menu_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, resume_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, settings_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, back_to_menu_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, exit_button_entity);
        }

        static public void ToggleFullscreenMode()
        {
            if (InternalCalls.KeyPressed((int)KeyCodes.F11))
            {
                InternalCalls.ToggleFullscreen();
            }
        }

    }
}

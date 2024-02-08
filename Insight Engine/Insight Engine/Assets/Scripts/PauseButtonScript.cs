/*!
 * \file PauseButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (70%), Matthew Ng, matthewdeen.ng@digipen.edu (30%)
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
using System.Runtime.CompilerServices;
using System;
using System.Collections.Generic;
namespace IS
{
    class PauseButtonScript
    {
        // Pause Button
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
        static SimpleImage resume_button_image;
        static SimpleImage how_to_play_button_image;
        static SimpleImage settings_button_image;
        static SimpleImage back_to_menu_button_image;
        static SimpleImage exit_button_image;

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

        static public void Init()
        {
            paused = false;

            // Pause Menu
            pause_menu_image = InternalCalls.GetSpriteImage("pause_menu_image.png");
            resume_button_image = InternalCalls.GetSpriteImage("resume_game_button.png");
            settings_button_image = InternalCalls.GetSpriteImage("menu_settings_button.png");
            how_to_play_button_image = InternalCalls.GetSpriteImage("menu_how_to_play_button.png");
            back_to_menu_button_image = InternalCalls.GetSpriteImage("main_menu_button.png");
            exit_button_image = InternalCalls.GetSpriteImage("quit_game_button.png");

            pause_menu_entity = InternalCalls.CreateEntityUI("Pause Menu", pause_menu_image);
            resume_button_entity = InternalCalls.CreateEntityButtonNoText("Resume Game", resume_button_image, "ResumeButtonScript");
            settings_button_entity = InternalCalls.CreateEntityButtonNoText("Setting", settings_button_image, "SettingsScript");
            how_to_play_button_entity = InternalCalls.CreateEntityButtonNoText("How to Play Button", how_to_play_button_image, "HowToPlayScript");
            back_to_menu_button_entity = InternalCalls.CreateEntityButtonNoText("Main Menu", back_to_menu_button_image, "BackToMenuButtonScript");
            exit_button_entity = InternalCalls.CreateEntityButtonNoText("Quit Game", exit_button_image, "ExitButtonScript");



        }

        static public void Update()
        {
            ToggleFullscreenMode();

            //set the image
            InternalCalls.SetSpriteImage(paused_button_image);

            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            // Get camera position 
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            orgin.x = camera_pos.x - (win_dimension.x / 2f);
            orgin.y = camera_pos.y - (win_dimension.y / 2f);

            // Size
            InternalCalls.TransformSetScale(0.08f * win_dimension.x, 0.08f * win_dimension.x);
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

            if (InternalCalls.KeyPressed((int)KeyCodes.Escape))
            {
                paused = !paused;
                SettingsScript.show_settings = false;
                ExitButtonScript.exit_confirmation = false;
                HowToPlayScript.show_how_to_play = false;
            }

            if (!InternalCalls.IsWindowFocused())
            {
                paused = true;
            }

            //mouse hovering
            if (InternalCalls.GetButtonState() == 1)
            {
                InternalCalls.TransformSetScale(0.06f * win_dimension.x, 0.1f * win_dimension.y);
                InternalCalls.SetSpriteImage(paused_button_clicked_image);
            }
            //click
            if (InternalCalls.GetButtonState() == 2)
            {
                InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
                InternalCalls.SetSpriteImage(paused_button_clicked_image);

                if (!paused)
                {
                    paused = true;
                }
                else
                {
                    paused = false;
                }
            }

            if (paused) // if game paused
            {
                InternalCalls.GamePause(true);
                InternalCalls.SetLightsToggle(false);
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
                InternalCalls.GamePause(false);
                InternalCalls.SetLightsToggle(true);
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

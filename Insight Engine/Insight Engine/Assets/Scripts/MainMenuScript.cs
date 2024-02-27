/*!
 * \file MainMenuScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (70%), Matthew Ng, matthewdeen.ng@digipen.edu (30%)
 * \par Course: CSD2451
 * \brief
 * This c# file contains the MainMenuScript class, used for main menu of the game
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class MainMenuScript
    {
        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static private int game_title_entity;
        static private int start_button_entity;
        static private int settings_button_entity;
        static private int how_to_play_button_entity;
        static private int exit_button_entity;

        static SimpleImage game_title_image;
        static SimpleImage button_frame;

        static Vector2D game_title_pos = new Vector2D(0, 0);
        static Vector2D start_button_pos = new Vector2D(0, 0);
        static Vector2D settings_button_pos = new Vector2D(0, 0);
        static Vector2D how_to_play_button_pos = new Vector2D(0, 0);
        static Vector2D exit_button_pos = new Vector2D(0, 0);

        static public void Init()
        {
            game_title_image = InternalCalls.GetSpriteImage("fragments_game_logo.png");
            button_frame = InternalCalls.GetSpriteImage("button_frame.png");

            game_title_entity = InternalCalls.CreateEntityUI("Game Title", game_title_image);
            start_button_entity = InternalCalls.CreateEntityButtonNoText("Start Button", button_frame, "StartButtonScript"); //look thru
            settings_button_entity = InternalCalls.CreateEntityButtonNoText("Settings Button", button_frame, "SettingsScript"); //done for now
            how_to_play_button_entity = InternalCalls.CreateEntityButtonNoText("How To Play Button", button_frame, "HowToPlayScript"); //done
            exit_button_entity = InternalCalls.CreateEntityButtonNoText("Exit Button", button_frame, "ExitButtonScript"); //done
        }

        static public void Update()
        {
            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);
            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);
            Vector2D button = new Vector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y);

            // set sizes
            InternalCalls.TransformSetScaleEntity(0.57f * win_dimension.x, 0.14f * win_dimension.y, game_title_entity);
            InternalCalls.SetButtonSize(start_button_entity, new SimpleVector2D(button.x,button.y));
            InternalCalls.SetButtonSize(settings_button_entity, new SimpleVector2D(button.x, button.y));
            InternalCalls.SetButtonSize(how_to_play_button_entity, new SimpleVector2D(button.x, button.y));
            InternalCalls.SetButtonSize(exit_button_entity, new SimpleVector2D  (button.x, button.y));

            // set pos
            game_title_pos.Set(origin.x + (0.33f * win_dimension.x), origin.y + (0.76f * win_dimension.y));
            start_button_pos.Set(origin.x + (0.31f * win_dimension.x), origin.y + (0.58f * win_dimension.y));
            settings_button_pos.Set(origin.x + (0.31f * win_dimension.x), origin.y + (0.46f * win_dimension.y));
            how_to_play_button_pos.Set(origin.x + (0.31f * win_dimension.x), origin.y + (0.34f * win_dimension.y));
            exit_button_pos.Set(origin.x + (0.31f * win_dimension.x), origin.y + (0.22f * win_dimension.y));

            if (SettingsScript.show_settings || HowToPlayScript.show_how_to_play || ExitButtonScript.exit_confirmation)
            {
                InternalCalls.SetLightsToggle(false);
                HideMenuButtons();
            }
            else //draw 
            {
                InternalCalls.SetLightsToggle(true);
                DrawMenuButtons();
            }
        }

        static public void CleanUp()
        {
        }

        static public void DrawMenuButtons()
        {
            InternalCalls.TransformSetPositionEntity(game_title_pos.x, game_title_pos.y, game_title_entity);
            InternalCalls.TransformSetPositionEntity(start_button_pos.x, start_button_pos.y, start_button_entity);
            InternalCalls.TransformSetPositionEntity(settings_button_pos.x, settings_button_pos.y, settings_button_entity);
            InternalCalls.TransformSetPositionEntity(how_to_play_button_pos.x, how_to_play_button_pos.y, how_to_play_button_entity);
            InternalCalls.TransformSetPositionEntity(exit_button_pos.x, exit_button_pos.y, exit_button_entity);

            InternalCalls.RenderTextFont("START", "MedusaGothic_D", 0.3118f, 0.583f, 7.5f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("SETTINGS", "MedusaGothic_D", 0.3118f, 0.463f, 7.5f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("HOW TO PLAY", "MedusaGothic_D", 0.3118f, 0.343f, 7.5f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("EXIT", "MedusaGothic_D", 0.3118f, 0.223f, 7.5f, (1f, 1f, 1f, 1f));

        }

        static public void HideMenuButtons()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, game_title_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, start_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, settings_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, exit_button_entity);
        }
    }
}
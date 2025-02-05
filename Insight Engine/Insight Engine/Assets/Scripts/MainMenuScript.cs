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
        private const float GAME_TITLE_SCALE = .5f;
        private const float GAME_TITLE_AR = 92f / 512f;
        //public const float HOVER_FONT_SIZE = 8.5f;
        //public const float NORMAL_FONT_SIZE = 7.5f;

        public const float HOVER_FONT_SIZE = 11.5f;
        public const float NORMAL_FONT_SIZE = 10f;

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
        static private int team_logo_entity;

        static SimpleImage game_title_image;
        static SimpleImage team_logo_image;
        static SimpleImage button_frame;

        static Vector2D game_title_pos = new Vector2D(0, 0);
        static Vector2D start_button_pos = new Vector2D(0, 0);
        static Vector2D settings_button_pos = new Vector2D(0, 0);
        static Vector2D how_to_play_button_pos = new Vector2D(0, 0);
        static Vector2D exit_button_pos = new Vector2D(0, 0);
        static Vector2D team_logo_pos = new Vector2D(0, 0);

        static public void Init()
        {
            //game_title_image = InternalCalls.GetSpriteImage("fragments_game_logo.png");
            game_title_image = InternalCalls.GetSpriteImage("GameTitle Animated 11R4C.png");
            button_frame = InternalCalls.GetSpriteImage("button_frame.png");
            team_logo_image = InternalCalls.GetSpriteImage("team_logo.png");

            game_title_entity = InternalCalls.CreateEntityUI("Game Title", game_title_image);
            start_button_entity = InternalCalls.CreateEntityButtonNoText("Start Button", button_frame, "StartButtonScript"); 
            settings_button_entity = InternalCalls.CreateEntityButtonNoText("Settings Button", button_frame, "SettingsScript"); 
            how_to_play_button_entity = InternalCalls.CreateEntityButtonNoText("How To Play Button", button_frame, "HowToPlayScript"); 
            exit_button_entity = InternalCalls.CreateEntityButtonNoText("Exit Button", button_frame, "ExitButtonScript"); 
            team_logo_entity = InternalCalls.CreateEntityButtonNoText("Team Logod", team_logo_image, "TeamLogoScript");

            win_dimension.x = (float)InternalCalls.GetWindowWidth();
            win_dimension.y = (float)InternalCalls.GetWindowHeight();
            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);
            InternalCalls.CreateAnimationFromSpriteEntity(11, 4, 2f, game_title_entity);
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

            //dimensions
            Vector2D team_logo = new Vector2D(0.4f * win_dimension.x, 0.2f * win_dimension.x);
            Vector2D button = new Vector2D(0.16f * win_dimension.x, 0.09f * win_dimension.y);

            // set sizes
            float game_title_width = win_dimension.x * GAME_TITLE_SCALE;
            float game_title_height = game_title_width * GAME_TITLE_AR;

            InternalCalls.TransformSetScaleEntity(game_title_width, game_title_height, game_title_entity);
            InternalCalls.SetButtonSize(start_button_entity, new SimpleVector2D(button.x,button.y));
            InternalCalls.SetButtonSize(settings_button_entity, new SimpleVector2D(button.x, button.y));
            InternalCalls.SetButtonSize(how_to_play_button_entity, new SimpleVector2D(button.x, button.y));
            InternalCalls.SetButtonSize(exit_button_entity, new SimpleVector2D  (button.x, button.y));
            InternalCalls.SetButtonSize(team_logo_entity, new SimpleVector2D  (team_logo.x, team_logo.y));

            // set pos
            game_title_pos.Set(origin.x + (0.33f * win_dimension.x), origin.y + (0.77f * win_dimension.y));
            start_button_pos.Set(origin.x + (0.31f * win_dimension.x), origin.y + (0.58f * win_dimension.y));
            settings_button_pos.Set(origin.x + (0.31f * win_dimension.x), origin.y + (0.46f * win_dimension.y));
            how_to_play_button_pos.Set(origin.x + (0.31f * win_dimension.x), origin.y + (0.34f * win_dimension.y));
            exit_button_pos.Set(origin.x + (0.31f * win_dimension.x), origin.y + (0.22f * win_dimension.y));
            team_logo_pos.Set(origin.x + (0.875f * win_dimension.x), origin.y + (0.175f * win_dimension.y));

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
            InternalCalls.TransformSetPositionEntity(team_logo_pos.x, team_logo_pos.y, team_logo_entity);

            InternalCalls.RenderTextFont("START", "Cinzel_SemiBold", 0.3118f, 0.576f, StartButtonScript.font_size, SettingsScript.FONT_COLOR);
            InternalCalls.RenderTextFont("SETTINGS", "Cinzel_SemiBold", 0.3118f, 0.456f, SettingsScript.font_size, SettingsScript.FONT_COLOR);
            InternalCalls.RenderTextFont("HOW TO PLAY", "Cinzel_SemiBold", 0.3118f, 0.336f, HowToPlayScript.font_size, SettingsScript.FONT_COLOR);
            InternalCalls.RenderTextFont("EXIT", "Cinzel_SemiBold", 0.3118f, 0.216f, ExitButtonScript.font_size, SettingsScript.FONT_COLOR);
        }

        static public void HideMenuButtons()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, game_title_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, start_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, settings_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, exit_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, team_logo_entity);
        }
    }
}
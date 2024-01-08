/*!
 * \file MenuButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the MenuButtonScript class, used for the layout and function
 * for the main menu in game
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System.Runtime.CompilerServices;
namespace IS
{

    class MenuButtonScript
    {

        static private int start_entity;
        static private int setting_entity;
        static private int how_to_play_entity;
        static private int exit_entity;
        static private int game_title_entity;
        static private int team_logo_entity;

        static SimpleImage start_image;
        static SimpleImage setting_image;
        static SimpleImage how_to_play_image;
        static SimpleImage exit_image;
        static SimpleImage game_title_image;
        static SimpleImage team_logo_image;

        static Vector2D start_pos = new Vector2D(0, 0);
        static Vector2D setting_pos = new Vector2D(0, 0);
        static Vector2D how_to_play_pos = new Vector2D(0, 0);
        static Vector2D exit_pos = new Vector2D(0, 0);
        static Vector2D game_title_pos = new Vector2D(0, 0);
        static Vector2D team_logo_pos = new Vector2D(0, 0);

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D orgin = new Vector2D(0, 0);

        static public void Init()
        {
            start_image = InternalCalls.GetSpriteImage("menu_start_button.png");
            setting_image = InternalCalls.GetSpriteImage("menu_settings_button.png");
            how_to_play_image = InternalCalls.GetSpriteImage("menu_how_to_play_button.png");
            exit_image = InternalCalls.GetSpriteImage("menu_exit_button.png");
            game_title_image = InternalCalls.GetSpriteImage("fragments_game_logo.png");
            team_logo_image = InternalCalls.GetSpriteImage("team_logo.png");

            start_entity = InternalCalls.CreateEntityButton("Start Button", start_image, "StartButtonScript", "");
            setting_entity = InternalCalls.CreateEntityButton("Setting Button", setting_image, "SettingScript", "");
            how_to_play_entity = InternalCalls.CreateEntityButton("How To Play Button", how_to_play_image, "HowToPlayScript", "");
            exit_entity = InternalCalls.CreateEntityButton("Exit Button", exit_image, "ExitButtonScript", "");
            game_title_entity = InternalCalls.CreateEntityUI("Game Title", game_title_image);
            team_logo_entity = InternalCalls.CreateEntityUI("Team Logo", team_logo_image);

            
            /*start_pos.Set(-900f, 205f);
            setting_pos.Set(-900f, -70f);
            how_to_play_pos.Set(-900f, -345f);
            exit_pos.Set(-900f, -620f);*/

            
            //InternalCalls.NativeLog("start_x: ", start_pos.x);
            //start_pos = start_pos.Add(win_dimension.x / 2f, win_dimension.y / 2f);
            //start_pos = start_pos.Divide(win_dimension.x, win_dimension.y);
            //InternalCalls.NativeLog("start_x: ", start_pos.x);
        }

        static public void Update()
        {
            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() * 2f;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() * 2f;

            orgin.x = -(win_dimension.x / 2f);
            orgin.y = -(win_dimension.y / 2f);

            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);

            /*InternalCalls.TransformSetScaleEntity(0.14f * win_dimension.x, 0.08f * win_dimension.y, start_entity);
            InternalCalls.TransformSetScaleEntity(0.14f * win_dimension.x, 0.08f * win_dimension.y, setting_entity);
            InternalCalls.TransformSetScaleEntity(0.14f * win_dimension.x, 0.08f * win_dimension.y, how_to_play_entity);
            InternalCalls.TransformSetScaleEntity(0.14f * win_dimension.x, 0.08f * win_dimension.y, exit_entity);*/

            // Dimensions
            InternalCalls.SetButtonSize(start_entity, new SimpleVector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y));
            InternalCalls.SetButtonSize(setting_entity, new SimpleVector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y));
            InternalCalls.SetButtonSize(how_to_play_entity, new SimpleVector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y));
            InternalCalls.SetButtonSize(exit_entity, new SimpleVector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y));
            InternalCalls.TransformSetScaleEntity(0.57f * win_dimension.x, 0.14f * win_dimension.y, game_title_entity);
            InternalCalls.TransformSetScaleEntity(0.44f * win_dimension.x, 0.35f * win_dimension.y, team_logo_entity);

            // Positions
            start_pos.Set(orgin.x + (0.31f * win_dimension.x), orgin.y + (0.58f * win_dimension.y));
            setting_pos.Set(orgin.x + (0.31f * win_dimension.x), orgin.y + (0.46f * win_dimension.y));
            how_to_play_pos.Set(orgin.x + (0.31f * win_dimension.x), orgin.y + (0.34f * win_dimension.y));
            exit_pos.Set(orgin.x + (0.31f * win_dimension.x), orgin.y + (0.22f * win_dimension.y));
            game_title_pos.Set(orgin.x + (0.33f * win_dimension.x), orgin.y + (0.76f * win_dimension.y));
            team_logo_pos.Set(orgin.x + (0.9f * win_dimension.x), orgin.y + (0.13f * win_dimension.y));

            //InternalCalls.NativeLog("win_x: ", win_dimension.x);
            //InternalCalls.NativeLog("win_y: ", win_dimension.y);
            if (ExitButtonScript.exit_confirmation || HowToPlayScript.how_to_play_enable) // if exit confirmation is active, move away the menu buttons
            {
                MoveAwayMenuButton();
            }
            else
            {
                DrawMenuButton();
            }

            
        }


        static public void CleanUp()
        {
        }

        static public void DrawMenuButton()
        {
            InternalCalls.TransformSetPositionEntity(start_pos.x, start_pos.y, start_entity);
            InternalCalls.TransformSetPositionEntity(setting_pos.x, setting_pos.y, setting_entity);
            InternalCalls.TransformSetPositionEntity(how_to_play_pos.x, how_to_play_pos.y, how_to_play_entity);
            InternalCalls.TransformSetPositionEntity(exit_pos.x, exit_pos.y, exit_entity);
            InternalCalls.TransformSetPositionEntity(game_title_pos.x, game_title_pos.y, game_title_entity);
            InternalCalls.TransformSetPositionEntity(team_logo_pos.x, team_logo_pos.y, team_logo_entity);

            // draw buttons
            /*InternalCalls.ButtonRenderText(start_entity, 0.265f, 0.575f, 23f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(setting_entity, 0.265f, 0.458f, 21f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(how_to_play_entity, 0.266f, 0.34f, 16f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(exit_entity, 0.265f, 0.23f, 20f, (1f, 1f, 1f));*/
        }

        static public void MoveAwayMenuButton()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, start_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, setting_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, exit_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, game_title_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, team_logo_entity);
        }
    }
}

using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class MainMenuScript
    {
        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D orgin = new Vector2D(0, 0);

        static private int game_title_entity;
        static private int team_logo_entity;
        static private int start_button_entity;
        static private int settings_button_entity;
        static private int how_to_play_button_entity;
        static private int exit_button_entity;

        static SimpleImage game_title_image;
        static SimpleImage team_logo_image;
        static SimpleImage start_button_image;
        static SimpleImage settings_button_image;
        static SimpleImage how_to_play_button_image;
        static SimpleImage exit_button_image;

        static Vector2D game_title_pos = new Vector2D(0, 0);
        static Vector2D team_logo_pos = new Vector2D(0, 0);
        static Vector2D start_button_pos = new Vector2D(0, 0);
        static Vector2D settings_button_pos = new Vector2D(0, 0);
        static Vector2D how_to_play_button_pos = new Vector2D(0, 0);
        static Vector2D exit_button_pos = new Vector2D(0, 0);

        static public void Init()
        {
            game_title_image = InternalCalls.GetSpriteImage("fragments_game_logo.png");
            team_logo_image = InternalCalls.GetSpriteImage("team_logo.png");
            start_button_image = InternalCalls.GetSpriteImage("menu_start_button.png");
            settings_button_image = InternalCalls.GetSpriteImage("menu_settings_button.png");
            how_to_play_button_image = InternalCalls.GetSpriteImage("menu_how_to_play_button.png");
            exit_button_image = InternalCalls.GetSpriteImage("menu_exit_button.png");

            game_title_entity = InternalCalls.CreateEntityUI("Game Title", game_title_image);
            team_logo_entity = InternalCalls.CreateEntityButtonNoText("Team Logo", team_logo_image, "TeamLogoScript"); //for gimick
            start_button_entity = InternalCalls.CreateEntityButtonNoText("Start Button", start_button_image, "StartButtonScript"); //look thru
            settings_button_entity = InternalCalls.CreateEntityButtonNoText("Settings Button", settings_button_image, "SettingsButtonScript");
            how_to_play_button_entity = InternalCalls.CreateEntityButtonNoText("How To Play Button", how_to_play_button_image, "HowToPlayButtonScript"); 
            exit_button_entity = InternalCalls.CreateEntityButtonNoText("Exit Button", exit_button_image, "ExitButtonScript");
        }

        static public void Update()
        {
            // get window size
            win_dimension.x = (float)InternalCalls.GetWindowWidth();
            win_dimension.y = (float)InternalCalls.GetWindowHeight();
            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);

            // set origin
            orgin.x = -(win_dimension.x / 2f);
            orgin.y = -(win_dimension.y / 2f);

            // set sizes
            InternalCalls.TransformSetScaleEntity(0.57f * win_dimension.x, 0.14f * win_dimension.y, game_title_entity);
            InternalCalls.SetButtonSize(team_logo_entity, new SimpleVector2D(0.44f * win_dimension.x, 0.35f * win_dimension.y));
            InternalCalls.SetButtonSize(start_button_entity, new SimpleVector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y));
            InternalCalls.SetButtonSize(settings_button_entity, new SimpleVector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y));
            InternalCalls.SetButtonSize(how_to_play_button_entity, new SimpleVector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y));
            InternalCalls.SetButtonSize(exit_button_entity, new SimpleVector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y));

            // set pos
            game_title_pos.Set(orgin.x + (0.33f * win_dimension.x), orgin.y + (0.76f * win_dimension.y));
            team_logo_pos.Set(orgin.x + (0.9f * win_dimension.x), orgin.y + (0.13f * win_dimension.y)); 
            start_button_pos.Set(orgin.x + (0.31f * win_dimension.x), orgin.y + (0.58f * win_dimension.y));
            settings_button_pos.Set(orgin.x + (0.31f * win_dimension.x), orgin.y + (0.46f * win_dimension.y));
            how_to_play_button_pos.Set(orgin.x + (0.31f * win_dimension.x), orgin.y + (0.34f * win_dimension.y));
            exit_button_pos.Set(orgin.x + (0.31f * win_dimension.x), orgin.y + (0.22f * win_dimension.y));

            if (ExitButtonScript.exit_confirmation)
            {
                //add settings when settings ui done
                HideMenuButtons();
            }
            else //draw 
            {
                InternalCalls.TransformSetPositionEntity(game_title_pos.x, game_title_pos.y, game_title_entity);
                InternalCalls.TransformSetPositionEntity(team_logo_pos.x, team_logo_pos.y, team_logo_entity);
                InternalCalls.TransformSetPositionEntity(start_button_pos.x, start_button_pos.y, start_button_entity);
                InternalCalls.TransformSetPositionEntity(settings_button_pos.x, settings_button_pos.y, settings_button_entity);
                InternalCalls.TransformSetPositionEntity(how_to_play_button_pos.x, how_to_play_button_pos.y, how_to_play_button_entity);
                InternalCalls.TransformSetPositionEntity(exit_button_pos.x, exit_button_pos.y, exit_button_entity);
            }
        }

        static public void CleanUp()
        {
        }

        static public void HideMenuButtons()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, game_title_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, team_logo_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, start_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, settings_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_button_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, exit_button_entity);
        }
    }
}

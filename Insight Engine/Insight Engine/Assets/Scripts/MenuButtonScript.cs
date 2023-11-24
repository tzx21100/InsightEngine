using System.Runtime.CompilerServices;
namespace IS
{

    class MenuButtonScript
    {

        static private int start_entity;
        static private int setting_entity;
        static private int how_to_play_entity;
        static private int exit_entity;


        static SimpleImage start_image;
        static SimpleImage setting_image;
        static SimpleImage how_to_play_image;
        static SimpleImage exit_image;

        static Vector2D start_pos = new Vector2D(0, 0);
        static Vector2D setting_pos = new Vector2D(0, 0);
        static Vector2D how_to_play_pos = new Vector2D(0, 0);
        static Vector2D exit_pos = new Vector2D(0, 0);

        static public void Init()
        {
            start_image = InternalCalls.GetSpriteImage("button_frame.png");
            setting_image = InternalCalls.GetSpriteImage("button_frame.png");
            how_to_play_image = InternalCalls.GetSpriteImage("button_frame.png");
            exit_image = InternalCalls.GetSpriteImage("button_frame.png");

            start_entity = InternalCalls.CreateEntityButton("Start Button", start_image, "StartButtonScript", "start");
            setting_entity = InternalCalls.CreateEntityButton("Setting Button", start_image, "SettingScript", "settings");
            how_to_play_entity = InternalCalls.CreateEntityButton("How To Play Button", how_to_play_image, "HowToPlayScript", "how to play");
            exit_entity = InternalCalls.CreateEntityButton("Exit Button", start_image, "ExitButtonScript", "exit");

            InternalCalls.TransformSetScaleEntity(629f, 207f, start_entity);
            InternalCalls.TransformSetScaleEntity(629f, 207f, setting_entity);
            InternalCalls.TransformSetScaleEntity(629f, 207f, how_to_play_entity);
            InternalCalls.TransformSetScaleEntity(629f, 207f, exit_entity);

            start_pos.Set(-900f, 205f);
            setting_pos.Set(-900f, -70f);
            how_to_play_pos.Set(-900f, -345f);
            exit_pos.Set(-900f, -620f);
        }

        static public void Update()
        {

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

            // draw buttons
            InternalCalls.ButtonRenderText(start_entity, 0.265f, 0.575f, 23f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(setting_entity, 0.265f, 0.458f, 21f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(how_to_play_entity, 0.266f, 0.34f, 16f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(exit_entity, 0.265f, 0.23f, 20f, (1f, 1f, 1f));
        }

        static public void MoveAwayMenuButton()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, start_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, setting_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, exit_entity);
        }
    }
}

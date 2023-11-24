using System.Runtime.CompilerServices;
namespace IS
{

    class MenuButtonScript
    {

        static private int start_entity;
        static private int setting_entity;
        static private int exit_entity;


        static SimpleImage start_image;
        static SimpleImage setting_image;
        static SimpleImage exit_image;

        static Vector2D start_pos = new Vector2D(0, 0);
        static Vector2D setting_pos = new Vector2D(0, 0);
        static Vector2D exit_pos = new Vector2D(0, 0);

        static public void Init()
        {
            start_image = InternalCalls.GetSpriteImage("button_frame.png");
            setting_image = InternalCalls.GetSpriteImage("button_frame.png");
            exit_image = InternalCalls.GetSpriteImage("button_frame.png");

            start_entity = InternalCalls.CreateEntityButton("Start Button", start_image, "StartButtonScript", "start");
            setting_entity = InternalCalls.CreateEntityButton("Setting Button", start_image, "SettingButtonScript", "settings");
            exit_entity = InternalCalls.CreateEntityButton("Exit Button", start_image, "ExitButtonScript", "exit");

            InternalCalls.TransformSetScaleEntity(1048f, 345f, start_entity);
            InternalCalls.TransformSetScaleEntity(1048f, 345f, setting_entity);
            InternalCalls.TransformSetScaleEntity(444f, 211f, exit_entity);

            start_pos.Set(-364f, 121f);
            setting_pos.Set(-364f, -340f);
            exit_pos.Set(-1549f, -884f);
        }

        static public void Update()
        {
            InternalCalls.TransformSetPositionEntity(start_pos.x, start_pos.y, start_entity);
            InternalCalls.TransformSetPositionEntity(setting_pos.x, setting_pos.y, setting_entity);
            InternalCalls.TransformSetPositionEntity(exit_pos.x, exit_pos.y, exit_entity);

            // draw buttons
            InternalCalls.ButtonRenderText(start_entity, 0.405f, 0.54f, 32f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(setting_entity, 0.405f, 0.34f, 32f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(exit_entity, 0.095f, 0.105f, 16f, (1f, 1f, 1f));
        }


        static public void CleanUp()
        {
        }
    }
}

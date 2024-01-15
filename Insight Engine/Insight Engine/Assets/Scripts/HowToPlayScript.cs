using System.Runtime.CompilerServices;
namespace IS
{ 
    class HowToPlayScript
    {
        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D orgin = new Vector2D(0, 0);

        static private int back_button_entity;

        static SimpleImage back_button_image;

        static Vector2D back_button_pos = new Vector2D(0, 0);

        // Camera Pos
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public void Init()
        {
            back_button_image = InternalCalls.GetSpriteImage("back_button.png");
            back_button_entity = InternalCalls.CreateEntityButtonNoText("Back Button", back_button_image, "BackButtonScript"); //todo script
            
        }

        static public void Update()
        {
            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;
            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);

            orgin.x = camera_pos.x - (win_dimension.x / 2f);
            orgin.y = camera_pos.y - (win_dimension.y / 2f);

            InternalCalls.SetButtonSize(back_button_entity, new SimpleVector2D(win_dimension.x, win_dimension.y));

            // Positions    
            back_button_pos.Set(orgin.x + (win_dimension.x), orgin.y + (win_dimension.y));

            DrawBackButton();
        }


        static public void CleanUp()
        {
        }

        static public void DrawBackButton()
        {
            InternalCalls.TransformSetPositionEntity(back_button_pos.x, back_button_pos.y, back_button_entity);
        }

    }
}

using System.Runtime.CompilerServices;
namespace IS
{
    class YouWinScript
    {
        static public bool first_hovering = false;

        // Win Menu
        //static private int pause_menu_entity;
        static private int back_entity;

        static SimpleImage back_image;

        static Vector2D back_pos = new Vector2D(0, 0);

        // Camera Pos
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D orgin = new Vector2D(0, 0);

        static public void Init()
        {
            back_image = InternalCalls.GetSpriteImage("back_button.png");

            back_entity = InternalCalls.CreateEntityButton("Back Button", back_image, "BackFromScript", "");

        }

        static public void Update()
        {
            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() * 2f / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() * 2f / camera_zoom;

            orgin.x = camera_pos.x - (win_dimension.x / 2f);
            orgin.y = camera_pos.y - (win_dimension.y / 2f);

            // Dimensions
            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);
            Vector2D back = new Vector2D(0.23f * win_dimension.x, 0.13f * win_dimension.y);

            InternalCalls.SetButtonSize(back_entity, new SimpleVector2D(back.x, back.y));

            // Positions    
            InternalCalls.TransformSetPosition(camera_pos.x, camera_pos.y);
            back_pos.Set(orgin.x + (0.16f * win_dimension.x), orgin.y + (0.85f * win_dimension.y));
            InternalCalls.TransformSetPositionEntity(back_pos.x, back_pos.y, back_entity);

            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {

                //hovering
                if (!first_hovering)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
                    first_hovering = true;
                }
            }
            else
            {
                first_hovering = false;
            }


            if (InternalCalls.GetButtonState() == 2)
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
                //click
            }
        }


        static public void CleanUp()
        {

        }
    }
}

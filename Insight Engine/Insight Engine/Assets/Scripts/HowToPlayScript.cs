using System.Runtime.CompilerServices;
namespace IS
{
    class HowToPlayScript
    {
        static public bool how_to_play_enable = false;

        // How To Play Panel
        static private int how_to_play_menu_entity;
        static private int back_entity;

        static SimpleImage how_to_play_menu_image;
        static SimpleImage back_image;

        static Vector2D how_to_play_menu_pos = new Vector2D(0, 0);
        static Vector2D back_pos = new Vector2D(0, 0);

        // Camera Pos
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public void Init()
        {
            how_to_play_enable = false;

            how_to_play_menu_image = InternalCalls.GetSpriteImage("how_to_play_image.png");
            back_image = InternalCalls.GetSpriteImage("button_frame.png");

            how_to_play_menu_entity = InternalCalls.CreateEntityUI("How To Play", how_to_play_menu_image);
            back_entity = InternalCalls.CreateEntityButton("Back Button", back_image, "BackFromTutorialButtonScript", "back");


            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            Vector2D how_to_play = new Vector2D(3840f, 2160f);
            Vector2D back = new Vector2D(600f, 200f);
            how_to_play = how_to_play.Divide(camera_zoom);
            back = back.Divide(camera_zoom);

            InternalCalls.TransformSetScaleEntity(how_to_play.x, how_to_play.y, how_to_play_menu_entity);
            InternalCalls.TransformSetScaleEntity(back.x, back.y, back_entity);
        }

        static public void Update()
        {
            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            how_to_play_menu_pos.Set(camera_pos.x + 0f / camera_zoom, camera_pos.y + 0f / camera_zoom);
            back_pos.Set(camera_pos.x - 1300f / camera_zoom, camera_pos.y + 800f / camera_zoom);

            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
            }
            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                how_to_play_enable = true;
            }

            if (how_to_play_enable)
            {
                InternalCalls.SetLightsToggle(false);
                DrawHowToPlay();
            }
            else
            {
                InternalCalls.SetLightsToggle(true);
                MoveAwayHowToPlay();
            }
        }


        static public void CleanUp()
        {
        }

        static public void DrawHowToPlay()
        {
            InternalCalls.TransformSetPositionEntity(how_to_play_menu_pos.x, how_to_play_menu_pos.y, how_to_play_menu_entity);
            InternalCalls.TransformSetPositionEntity(back_pos.x, back_pos.y, back_entity);

            // draw text
            InternalCalls.ButtonRenderText(back_entity, 0.16f, 0.82f, 23f, (1f, 1f, 1f));
        }

        static public void MoveAwayHowToPlay()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_menu_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, back_entity);
        }
    }
}

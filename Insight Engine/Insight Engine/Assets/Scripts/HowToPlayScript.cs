using System.Runtime.CompilerServices;
namespace IS
{
    class HowToPlayScript
    {
        static public bool how_to_play_enable = false;
        static public bool first_hovering = false;

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

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D orgin = new Vector2D(0, 0);

        static public void Init()
        {
            how_to_play_enable = false;

            how_to_play_menu_image = InternalCalls.GetSpriteImage("how_to_play_image.png");
            back_image = InternalCalls.GetSpriteImage("back_button.png");

            how_to_play_menu_entity = InternalCalls.CreateEntityUI("How To Play", how_to_play_menu_image);
            back_entity = InternalCalls.CreateEntityButton("Back Button", back_image, "BackFromTutorialButtonScript", "");
            
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
            Vector2D how_to_play = new Vector2D(win_dimension.x, win_dimension.y);
            Vector2D back = new Vector2D(0.23f * win_dimension.x, 0.13f * win_dimension.y);           

            InternalCalls.TransformSetScaleEntity(how_to_play.x, how_to_play.y, how_to_play_menu_entity);
            InternalCalls.SetButtonSize(back_entity, new SimpleVector2D(back.x, back.y));

            // Positions    
            how_to_play_menu_pos.Set(camera_pos.x, camera_pos.y);
            back_pos.Set(orgin.x + (0.16f * win_dimension.x), orgin.y + (0.85f * win_dimension.y));

            //how_to_play_menu_pos = how_to_play_menu_pos.Divide(camera_zoom);
            //back_pos = back_pos.Divide(camera_zoom);

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

            // clicking
            if (InternalCalls.GetButtonState() == 2)
            {
                //click
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
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
            //InternalCalls.ButtonRenderText(back_entity, 0.16f, 0.82f, 23f, (1f, 1f, 1f));
        }

        static public void MoveAwayHowToPlay()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_menu_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, back_entity);
        }
    }
}

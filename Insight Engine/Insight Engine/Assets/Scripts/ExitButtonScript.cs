using System.Runtime.CompilerServices;
namespace IS
{
    class ExitButtonScript
    {
        static public bool exit_confirmation = false;

        // Confirmation Panel
        static private int confirmation_menu_entity;
        static private int no_entity;
        static private int yes_entity;

        static SimpleImage confirmation_menu_image;
        static SimpleImage no_image;
        static SimpleImage yes_image;

        static Vector2D confirmation_menu_pos = new Vector2D(0, 0);
        static Vector2D no_pos = new Vector2D(0, 0);
        static Vector2D yes_pos = new Vector2D(0, 0);

        // Camera Pos
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public void Init()
        {
            exit_confirmation = false;

            // Confirmation Panel
            confirmation_menu_image = InternalCalls.GetSpriteImage("pause_menu.png");
            no_image = InternalCalls.GetSpriteImage("button_frame.png");
            yes_image = InternalCalls.GetSpriteImage("button_frame.png");

            confirmation_menu_entity = InternalCalls.CreateEntityUI("Confirmation Menu", confirmation_menu_image);
            no_entity = InternalCalls.CreateEntityButton("No Button", no_image, "NoButtonScript", "no");
            yes_entity = InternalCalls.CreateEntityButton("Yes Button", yes_image, "YesButtonScript", "yes");

            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            Vector2D confirmation = new Vector2D(2200f, 1250f);
            Vector2D no = new Vector2D(600f, 200f);
            Vector2D yes = new Vector2D(600f, 200f);
            confirmation = confirmation.Divide(camera_zoom);
            no = no.Divide(camera_zoom);
            yes = yes.Divide(camera_zoom);

            InternalCalls.TransformSetScaleEntity(confirmation.x, confirmation.y, confirmation_menu_entity);
            InternalCalls.TransformSetScaleEntity(no.x, no.y, no_entity);
            InternalCalls.TransformSetScaleEntity(yes.x, yes.y, yes_entity);
        }

        static public void Update(){

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            confirmation_menu_pos.Set(camera_pos.x + 0f / camera_zoom, camera_pos.y + 10f / camera_zoom);
            no_pos.Set(camera_pos.x + 600f / camera_zoom, camera_pos.y - 500f / camera_zoom);
            yes_pos.Set(camera_pos.x - 600f / camera_zoom, camera_pos.y - 500f / camera_zoom);

            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
            }
            if (InternalCalls.GetButtonState() == 2) 
            {
                //InternalCalls.Exit();
                /*if (!exit_confirmation)
                {
                    //InternalCalls.SetGameStatus(false);
                    exit_confirmation = true;
                }
                else
                {
                    pause_enable = false;
                }*/
                exit_confirmation = true;
            }

            if (exit_confirmation)
            {
                DrawConfirmationMenu();
            }
            else
            {
                MoveAwayConfirmationMenu();
            }
        }
        
        static public void CleanUp(){

        }

        static public void DrawConfirmationMenu()
        {
            InternalCalls.TransformSetPositionEntity(confirmation_menu_pos.x, confirmation_menu_pos.y, confirmation_menu_entity);
            InternalCalls.TransformSetPositionEntity(no_pos.x, no_pos.y, no_entity);
            InternalCalls.TransformSetPositionEntity(yes_pos.x, yes_pos.y, yes_entity);

            // draw text
            InternalCalls.RenderText("exit?", 0.5f, 0.5f, 23f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(no_entity, 0.658f, 0.28f, 18f, (1f, 1f, 1f));
            InternalCalls.ButtonRenderText(yes_entity, 0.34f, 0.28f, 18f, (1f, 1f, 1f));

        }

        static public void MoveAwayConfirmationMenu()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, confirmation_menu_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, no_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, yes_entity);
        }

    }
}

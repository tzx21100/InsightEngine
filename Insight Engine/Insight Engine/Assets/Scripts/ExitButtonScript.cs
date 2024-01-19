/*!
 * \file ExitButtonScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the ExitButtonScript class, used for the exit button 
 * and it will trigger out the confirmation window prompt
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
    class ExitButtonScript
    {
        static public bool exit_confirmation = false;
        static public bool first_hovering = false;

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

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        static public void Init()
        {
            exit_confirmation = false;

            // Confirmation Panel
            confirmation_menu_image = InternalCalls.GetSpriteImage("exit_menu_image.png");
            no_image = InternalCalls.GetSpriteImage("no_button.png");
            yes_image = InternalCalls.GetSpriteImage("yes_button.png");

            confirmation_menu_entity = InternalCalls.CreateEntityUI("Confirmation Menu", confirmation_menu_image);
            no_entity = InternalCalls.CreateEntityButton("No Button", no_image, "NoButtonScript", "no");
            yes_entity = InternalCalls.CreateEntityButton("Yes Button", yes_image, "YesButtonScript", "yes");

            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

        }

        static public void Update(){

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x -(win_dimension.x / 2f);
            origin.y = camera_pos.y -(win_dimension.y / 2f);

            // Dimensions
            /*Vector2D confirmation = new Vector2D(2200f, 1250f);
            Vector2D no = new Vector2D(600f, 200f);
            Vector2D yes = new Vector2D(600f, 200f);*/
            Vector2D confirmation = new Vector2D(0.85f * win_dimension.x, 0.75f * win_dimension.y);
            Vector2D no = new Vector2D(0.18f * win_dimension.x, 0.1f * win_dimension.y);
            Vector2D yes = new Vector2D(0.18f * win_dimension.x, 0.1f * win_dimension.y);
            /*confirmation = confirmation.Divide(camera_zoom);
            no = no.Divide(camera_zoom);
            yes = yes.Divide(camera_zoom);*/

            InternalCalls.TransformSetScaleEntity(confirmation.x, confirmation.y, confirmation_menu_entity);
            InternalCalls.SetButtonSize(no_entity, new SimpleVector2D(no.x, no.y));
            InternalCalls.SetButtonSize(yes_entity, new SimpleVector2D(yes.x, yes.y));

            // Positions
            confirmation_menu_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.5f * win_dimension.y));
            no_pos.Set(origin.x + (0.68f * win_dimension.x), origin.y + (0.22f * win_dimension.y));
            yes_pos.Set(origin.x + (0.32f * win_dimension.x), origin.y + (0.22f * win_dimension.y));

            /*confirmation_menu_pos = confirmation_menu_pos.Divide(camera_zoom);
            no_pos = no_pos.Divide(camera_zoom);
            yes_pos = yes_pos.Divide(camera_zoom);*/

            /*confirmation_menu_pos.Set(camera_pos.x + 0f / camera_zoom, camera_pos.y + 10f / camera_zoom);
            no_pos.Set(camera_pos.x + 600f / camera_zoom, camera_pos.y - 500f / camera_zoom);
            yes_pos.Set(camera_pos.x - 600f / camera_zoom, camera_pos.y - 500f / camera_zoom);*/

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
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
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
            //InternalCalls.RenderText("exit?", 0.5f, 0.5f, 23f, (1f, 1f, 1f));
            //InternalCalls.ButtonRenderText(no_entity, 0.658f, 0.28f, 18f, (1f, 1f, 1f));
            //InternalCalls.ButtonRenderText(yes_entity, 0.34f, 0.28f, 18f, (1f, 1f, 1f));

        }

        static public void MoveAwayConfirmationMenu()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, confirmation_menu_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, no_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, yes_entity);
        }

    }
}

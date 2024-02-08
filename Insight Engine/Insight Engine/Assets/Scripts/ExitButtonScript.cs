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
        static public bool first_hover = false;

        // Confirmation Panel
        static private int exit_overlay_entity;
        static private int no_entity;
        static private int yes_entity;

        static SimpleImage exit_overlay_image;
        static SimpleImage no_image;
        static SimpleImage yes_image;

        static Vector2D exit_overlay_pos = new Vector2D(0, 0);
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
            exit_overlay_image = InternalCalls.GetSpriteImage("exit_overlay.png");
            //exit_overlay_image = InternalCalls.GetSpriteImage("temp_exit_overlay.png");
            no_image = InternalCalls.GetSpriteImage("no_button.png");
            yes_image = InternalCalls.GetSpriteImage("yes_button.png");

            exit_overlay_entity = InternalCalls.CreateEntityUI("Confirmation Menu", exit_overlay_image);
            no_entity = InternalCalls.CreateEntityButtonNoText("No Button", no_image, "NoButtonScript");
            yes_entity = InternalCalls.CreateEntityButtonNoText("Yes Button", yes_image, "YesButtonScript");

            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

        }

        static public void Update(){

            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x -(win_dimension.x / 2f);
            origin.y = camera_pos.y -(win_dimension.y / 2f);

            Vector2D overlay = new Vector2D(win_dimension.x, win_dimension.y);
            Vector2D no = new Vector2D(0.2f * win_dimension.x, 0.2f * win_dimension.y);
            Vector2D yes = new Vector2D(0.2f * win_dimension.x, 0.2f * win_dimension.y);

            InternalCalls.TransformSetScaleEntity(overlay.x, overlay.y, exit_overlay_entity);
            InternalCalls.SetButtonSize(no_entity, new SimpleVector2D(no.x, no.y));
            InternalCalls.SetButtonSize(yes_entity, new SimpleVector2D(yes.x, yes.y));

            // Positions
            exit_overlay_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.5f * win_dimension.y));
            no_pos.Set(origin.x + (0.6f * win_dimension.x), origin.y + (0.46f * win_dimension.y));
            yes_pos.Set(origin.x + (0.4f * win_dimension.x), origin.y + (0.46f * win_dimension.y));


            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {

                if (!first_hover)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f);
                    first_hover = true;
                }
            }
            else
            {
                first_hover = false;
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
                HideConfirmationMenu();
            }
        }
        
        static public void CleanUp(){

        }

        static public void DrawConfirmationMenu()
        {
            InternalCalls.TransformSetPositionEntity(exit_overlay_pos.x, exit_overlay_pos.y, exit_overlay_entity);
            InternalCalls.TransformSetPositionEntity(no_pos.x, no_pos.y, no_entity);
            InternalCalls.TransformSetPositionEntity(yes_pos.x, yes_pos.y, yes_entity);
            InternalCalls.RenderTextFont("CONFIRM EXIT?", "MedusaGothic_D", 0.5f, 0.535f, 13f, (1f, 1f, 1f));

        }

        static public void HideConfirmationMenu()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, exit_overlay_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, no_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, yes_entity);
        }

    }
}

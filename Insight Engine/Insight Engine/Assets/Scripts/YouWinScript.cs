/*!
 * \file YouWinScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This header file contains the YouWinScript class, used for the win scene
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
    class YouWinScript
    {
        static public bool first_hover = false;
;
        static private int back_button_entity;

        static SimpleImage back_button_image;

        static Vector2D back_button_pos = new Vector2D(0, 0);

        // Camera Pos
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        static public void Init()
        {
            back_button_image = InternalCalls.GetSpriteImage("back_button.png");

            back_button_entity = InternalCalls.CreateEntityButtonNoText("Back Button", back_button_image, "BackFromScript");

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

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            // Dimensions
            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);

            Vector2D back = new Vector2D(0.23f * win_dimension.x, 0.13f * win_dimension.y);

            InternalCalls.SetButtonSize(back_button_entity, new SimpleVector2D(back.x, back.y));

            // Positions    
            InternalCalls.TransformSetPosition(camera_pos.x, camera_pos.y);

            back_button_pos.Set(origin.x + (0.16f * win_dimension.x), origin.y + (0.85f * win_dimension.y));
            InternalCalls.TransformSetPositionEntity(back_button_pos.x, back_button_pos.y, back_button_entity);

            //hover
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


             //click
            if (InternalCalls.GetButtonState() == 2)
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f);
            }
        }


        static public void CleanUp()
        {

        }
    }
}

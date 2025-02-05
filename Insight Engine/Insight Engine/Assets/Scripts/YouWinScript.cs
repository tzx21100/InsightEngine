/*!
 * \file YouWinScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (90%), Matthew Ng, matthewdeen.ng@digipen.edu (10%)
 * \par Course: CSD2451
 * \brief
 * This header file contains the YouWinScript class, used for the win scene
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
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

        static private int back_button_entity;

        static SimpleImage button_frame;

        static Vector2D back_button_pos = new Vector2D(0, 0);

        // Camera Pos
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        static public void Init()
        {
            button_frame = InternalCalls.GetSpriteImage("button_frame.png");
            back_button_entity = InternalCalls.CreateEntityButtonNoText("Back Button", button_frame, "BackFromWinScript");

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

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            // Dimensions
            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);

            Vector2D button = new Vector2D(0.14f * win_dimension.x, 0.08f * win_dimension.y);

            InternalCalls.SetButtonSize(back_button_entity, new SimpleVector2D(button.x, button.y));

            // Positions    
            InternalCalls.TransformSetPosition(camera_pos.x, camera_pos.y);

            back_button_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.35f * win_dimension.y));
            InternalCalls.RenderTextFont("EXIT", "Cinzel_SemiBold", 0.5f, 0.355f, 9.5f, (1f, 1f, 1f, 1f));
            //InternalCalls.RenderTextFont("BACK", "Cinzel_SemiBold", 0.163f, 0.856f, 14f, (1f, 1f, 1f, 1f));

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
            if (!InternalCalls.IsWindowFocused())
            {
                first_hover = true;
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

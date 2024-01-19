/*!
 * \file SettingsScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (10%), Matthew Ng, matthewdeen.ng@digipen.edu (90%)
 * \par Course: CSD2451
 * \brief
 * This c# file contains the SettingsScript class, used for settings menu of the game
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
    class SettingsScript
    {
        static public bool show_settings = false;
        static public bool first_hover = false;

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static private int not_available_entity;
        static SimpleImage not_available_image;
        static Vector2D not_available_pos = new Vector2D(0, 0);


        static public void Init()
        {
            show_settings = false;
            not_available_image = InternalCalls.GetSpriteImage("not_available.png");
            not_available_entity = InternalCalls.CreateEntityUI("Not Available", not_available_image);
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

            Vector2D not_available = new Vector2D(0.85f * win_dimension.x, 0.75f * win_dimension.y);
            InternalCalls.TransformSetScaleEntity(not_available.x, not_available.y, not_available_entity);
            not_available_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.5f * win_dimension.y));


            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
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
                show_settings = true;
                  
            }

            if(InternalCalls.KeyPressed(32)) //press space to get rid of popup 
            {
                show_settings = false;
            }

            if (show_settings)
            {
                DrawNotAvailable(); //temp
            }
            else
            {
                HideNotAvailable(); //temp
            }
        }

        static public void CleanUp()
        {

        }
        static private void DrawNotAvailable()
        {
            InternalCalls.TransformSetPositionEntity(not_available_pos.x, not_available_pos.y, not_available_entity);
        }

        static private void HideNotAvailable()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, not_available_entity);
        }


    }
}

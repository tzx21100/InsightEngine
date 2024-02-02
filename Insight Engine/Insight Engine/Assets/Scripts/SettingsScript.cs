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

        static private int background_entity;
        static private int settings_overlay_entity;
        static private int back_button_entity;

        static SimpleImage background_image;
        static SimpleImage settings_overlay_image;
        static SimpleImage back_button_image;

        static Vector2D background_pos = new Vector2D(0, 0);
        static Vector2D settings_overlay_pos = new Vector2D(0, 0);
        static Vector2D back_button_pos = new Vector2D(0, 0);


        static public void Init()
        {
            show_settings = false;

            background_image = InternalCalls.GetSpriteImage("main_menu_bg.jpeg");
            settings_overlay_image = InternalCalls.GetSpriteImage("settings_overlay.png");
            back_button_image = InternalCalls.GetSpriteImage("back_button.png");

            background_entity = InternalCalls.CreateEntityUI("Background", background_image);
            settings_overlay_entity = InternalCalls.CreateEntityUI("Settings Overlay", settings_overlay_image);
            back_button_entity = InternalCalls.CreateEntityButtonNoText("Back Button", back_button_image, "BackFromSettingsButtonScript");
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
            Vector2D background = new Vector2D(win_dimension.x, win_dimension.y);
            Vector2D settings_overlay = new Vector2D(win_dimension.x, win_dimension.y);
            Vector2D back = new Vector2D(0.35f * win_dimension.x, 0.35f * win_dimension.y);

            InternalCalls.TransformSetScaleEntity(background.x, background.y, background_entity);
            InternalCalls.TransformSetScaleEntity(settings_overlay.x, settings_overlay.y, settings_overlay_entity);
            InternalCalls.SetButtonSize(back_button_entity, new SimpleVector2D(back.x, back.y));

            // Positions    
            background_pos.Set(camera_pos.x, camera_pos.y);
            settings_overlay_pos.Set(camera_pos.x, camera_pos.y);
            back_button_pos.Set(origin.x + (0.08f * win_dimension.x), origin.y + (0.9f * win_dimension.y));

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

            if (show_settings)
            {
                InternalCalls.SetLightsToggle(false);
                DrawSettings(); 
            }
            else
            {
                InternalCalls.SetLightsToggle(true);
                HideSettings(); 
            }
        }

        static public void CleanUp()
        {

        }
        static private void DrawSettings()
        {
            InternalCalls.TransformSetPositionEntity(background_pos.x, background_pos.y, background_entity);
            InternalCalls.TransformSetPositionEntity(settings_overlay_pos.x, settings_overlay_pos.y, settings_overlay_entity);
            InternalCalls.TransformSetPositionEntity(back_button_pos.x, back_button_pos.y, back_button_entity); 
        }

        static private void HideSettings()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, background_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, settings_overlay_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, back_button_entity); 
        }


    }
}

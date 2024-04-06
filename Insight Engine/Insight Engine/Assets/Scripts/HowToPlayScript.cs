/*!
 * \file MainMenuScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (60%), Matthew Ng, matthewdeen.ng@digipen.edu (40%)
 * \par Course: CSD2451
 * \brief
 * This c# file contains the HowToPlayScript class, used for "How To Play" of the game
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class HowToPlayScript
    {
        static public bool show_how_to_play = false;
        static public bool first_hover = false;
        static public float font_size = MainMenuScript.NORMAL_FONT_SIZE;

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        // How To Play Panel
        static private int background_entity;
        static private int how_to_play_overlay_entity;
        static private int back_button_entity;

        static SimpleImage background_image;
        static SimpleImage how_to_play_overlay_image;
        static SimpleImage back_button_image;

        static Vector2D background_pos = new Vector2D(0, 0);
        static Vector2D how_to_play_overlay_pos = new Vector2D(0, 0);
        static Vector2D back_button_pos = new Vector2D(0, 0);

        static public void Init()
        {
            show_how_to_play = false;

            background_image = InternalCalls.GetSpriteImage("main_menu_bg.jpeg");
            how_to_play_overlay_image = InternalCalls.GetSpriteImage("how_to_play_overlay.png");
            //how_to_play_overlay_image = InternalCalls.GetSpriteImage("temp_how_to_play_overlay.png");
            back_button_image = InternalCalls.GetSpriteImage("back_button.png");

            background_entity = InternalCalls.CreateEntityUI("Background", background_image);
            how_to_play_overlay_entity = InternalCalls.CreateEntityUI("How To Play Overlay", how_to_play_overlay_image);
            back_button_entity = InternalCalls.CreateEntityButtonNoText("Back Button", back_button_image, "BackFromHowToPlayButtonScript");
            
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
            Vector2D how_to_play_overlay = new Vector2D(win_dimension.x, win_dimension.y);
            Vector2D back = new Vector2D(0.12f * win_dimension.x, 0.12f * win_dimension.x);     

            InternalCalls.TransformSetScaleEntity(background.x, background.y, background_entity);
            InternalCalls.TransformSetScaleEntity(how_to_play_overlay.x, how_to_play_overlay.y, how_to_play_overlay_entity);
            InternalCalls.SetButtonSize(back_button_entity, new SimpleVector2D(back.x, back.y));

            // Positions    
            background_pos.Set(camera_pos.x, camera_pos.y);
            how_to_play_overlay_pos.Set(camera_pos.x, camera_pos.y);
            back_button_pos.Set(origin.x + (0.08f * win_dimension.x), origin.y + (0.9f * win_dimension.y));

            //InternalCalls.RenderTextFont("H", "Times_New_Roman", 0.1f, 0.5f, 20f, (1f, 1f, 1f));
            //InternalCalls.RenderTextFont("H", "BRUSHSCI", 0.2f, 0.5f, 20f, (1f, 1f, 1f));
            //InternalCalls.RenderTextFont("H", "Poiret_One_Regular", 0.3f, 0.5f, 20f, (1f, 1f, 1f));


            //hover
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Hovered)
            {
                if (!first_hover)
                {
                    SettingsScript.PlayHoverSound();
                    first_hover = true;
                    font_size = MainMenuScript.HOVER_FONT_SIZE;
                }
            }
            else
            {
                first_hover = false;
                font_size = MainMenuScript.NORMAL_FONT_SIZE;
            }
            if (!InternalCalls.IsWindowFocused())
            {
                first_hover = true;
            }
            //click
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Pressed)
            {
                SettingsScript.PlayClickSound();
                show_how_to_play = true;
            }

            if (show_how_to_play)
            {

                //InternalCalls.SetLightsToggle(false);
                DrawHowToPlay();
            }
            else
            {
                //InternalCalls.SetLightsToggle(true);
                HideHowToPlay();
            }
        }


        static public void CleanUp()
        {
        }

        static public void DrawHowToPlay()
        {
            InternalCalls.TransformSetPositionEntity(background_pos.x, background_pos.y, background_entity);
            InternalCalls.TransformSetPositionEntity(how_to_play_overlay_pos.x, how_to_play_overlay_pos.y, how_to_play_overlay_entity);
            InternalCalls.TransformSetPositionEntity(back_button_pos.x, back_button_pos.y, back_button_entity);
         

            InternalCalls.RenderTextFont("HOW TO PLAY", "MedusaGothic_D", 0.5f, 0.67f, 12.5f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("A, D to move around", "Semplicita_Light", 0.5f, 0.58f, 10.5f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Spacebar to jump, hold for long jump!", "Semplicita_Light", 0.5f, 0.54f, 10.5f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Collect Blue Fragments for movement abilities,", "Semplicita_Light", 0.5f, 0.47f, 10.5f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Red Fragments to restore the world.", "Semplicita_Light", 0.5f, 0.43f, 10.5f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Activate Lampposts to set your", "Semplicita_Light", 0.5f, 0.36f, 10.5f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("respawn checkpoint", "Semplicita_Light", 0.5f, 0.32f, 10.5f, (1f, 1f, 1f, 1f));
        }

        static public void HideHowToPlay()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, background_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_overlay_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, back_button_entity);
        }
    }
}   

/*!
 * \file MainMenuScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (60%), Matthew Ng, matthewdeen.ng@digipen.edu (40%)
 * \par Course: CSD2451
 * \brief
 * This c# file contains the HowToPlayScript class, used for "How To Play" of the game
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
    class HowToPlayScript
    {
        static public bool show_how_to_play = false;
        static public bool first_hover = false;

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        // How To Play Panel
        static private int how_to_play_entity;
        static private int back_button_entity;

        static SimpleImage how_to_play_image;
        static SimpleImage back_button_image;

        static Vector2D how_to_play_pos = new Vector2D(0, 0);
        static Vector2D back_button_pos = new Vector2D(0, 0);

        static public void Init()
        {
            show_how_to_play = false;

            how_to_play_image = InternalCalls.GetSpriteImage("how_to_play_image.png");
            back_button_image = InternalCalls.GetSpriteImage("back_button.png");

            how_to_play_entity = InternalCalls.CreateEntityUI("How To Play", how_to_play_image);
            back_button_entity = InternalCalls.CreateEntityButtonNoText("Back Button", back_button_image, "BackFromTutorialButtonScript"); //see script
            
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
            Vector2D how_to_play = new Vector2D(win_dimension.x, win_dimension.y);
            Vector2D back = new Vector2D(0.23f * win_dimension.x, 0.13f * win_dimension.y);           

            InternalCalls.TransformSetScaleEntity(how_to_play.x, how_to_play.y, how_to_play_entity);
            InternalCalls.SetButtonSize(back_button_entity, new SimpleVector2D(back.x, back.y));

            // Positions    
            how_to_play_pos.Set(camera_pos.x, camera_pos.y);
            back_button_pos.Set(origin.x + (0.16f * win_dimension.x), origin.y + (0.8f * win_dimension.y));

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
                show_how_to_play = true;
            }

            if (show_how_to_play)
            {
                InternalCalls.SetLightsToggle(false);
                DrawHowToPlay();
            }
            else
            {
                InternalCalls.SetLightsToggle(true);
                HideHowToPlay();
            }
        }


        static public void CleanUp()
        {
        }

        static public void DrawHowToPlay()
        {
            InternalCalls.TransformSetPositionEntity(how_to_play_pos.x, how_to_play_pos.y, how_to_play_entity);
            InternalCalls.TransformSetPositionEntity(back_button_pos.x, back_button_pos.y, back_button_entity);

        }

        static public void HideHowToPlay()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, how_to_play_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, back_button_entity);
        }
    }
}

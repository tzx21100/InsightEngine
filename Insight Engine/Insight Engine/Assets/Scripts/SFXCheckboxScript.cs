/*!
 * \file SFXCheckboxScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This C# file contains the SFXCheckboxScript class, used for the SFXCheckbox in settings page. When it is toggled on means audio on, when toggled off means audio off.
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
    class SFXCheckboxScript
    {
        static public bool first_hover = false;
        static public SimpleImage checkbox_image = InternalCalls.GetSpriteImage("checkbox.png");
        static public SimpleImage toggled_image = InternalCalls.GetSpriteImage("checkbox_toggled.png");
        static public bool clicked = false;

        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public float x_pos;
        static public float y_pos;

        static public void Init()
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
        }

        static public void Update()
        {
            bool toggled = !InternalCalls.AudioIsSFXMute();
            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            InternalCalls.SetSpriteImage(!toggled ? checkbox_image : toggled_image);

            //hovered
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Hovered)
            {
                //hovering
                if (!first_hover)
                {
                    SettingsScript.PlayHoverSound();
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
            // clicking
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Pressed)
            {
                SettingsScript.PlayClickSound();
                toggled = !toggled;
                InternalCalls.AudioMuteSFX(!toggled);
            }

            x_pos = origin.x + (0.44f * win_dimension.x);
            y_pos = origin.y + (0.365f * win_dimension.y) - ScrollBarTrackerScript.virtual_y;

            if (SettingsScript.show_settings)
            {
                InternalCalls.TransformSetPosition(x_pos, y_pos);
            }
            if (!SettingsScript.show_settings || y_pos > (origin.y + (0.7f * win_dimension.y)) || y_pos < (origin.y + (0.25f * win_dimension.y)))
            {
                InternalCalls.TransformSetPosition(9999f, 9999f);
            }

        }


        static public void CleanUp()
        {

        }
    }
}

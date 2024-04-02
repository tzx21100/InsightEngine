/*!
 * \file ScrollBarTrackerScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This C# file contains the ScrollBarTrackerScript class, used for the settings Scroll Bar
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
    class ScrollBarTrackerScript
    {
        // Constants
        private const float SCROLL_SPEED = 4f;
        public const float LOWER_LIMIT_SCALE = 0.38f;
        public const float UPPER_LIMIT_SCALE = 0.525f;

        static public bool is_adjusting_scroll = false;
        static public bool first_hover = false;
        static private int id;
        static private float diff_y;
        static private float adjustment;
        static private bool first_open_settings = false;

        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public float upper_limit_scroll_track;
        static public float lower_limit_scroll_track;

        static public float x_pos;

        static public float virtual_y;
        static public void Init()
        {
            id = InternalCalls.GetCurrentEntityID();
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

            upper_limit_scroll_track = origin.y + (UPPER_LIMIT_SCALE * win_dimension.y);
            lower_limit_scroll_track = origin.y + (LOWER_LIMIT_SCALE * win_dimension.y);

            first_open_settings = false;
            diff_y = UPPER_LIMIT_SCALE;
            InternalCalls.SetButtonHoverScale(id, 0.95f);

            virtual_y = 0f;
        }

        static public void Update()
        {
            //InternalCalls.TransformSetPosition(origin.x + (0.5f * win_dimension.x), origin.y + (0.433f * win_dimension.y));
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
            upper_limit_scroll_track = origin.y + (UPPER_LIMIT_SCALE * win_dimension.y);
            lower_limit_scroll_track = origin.y + (LOWER_LIMIT_SCALE * win_dimension.y);
            Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());

            // Mouse Scroll
            float yoffset = (float)InternalCalls.GetMouseScrollYOffset();
            if (yoffset != 0 && InternalCalls.IsWindowFocused())
            {
                AdjustScroll(InternalCalls.GetTransformPosition().y + SCROLL_SPEED * yoffset);
                is_adjusting_scroll = true;
            }

            if (InternalCalls.MousePressed((int)MouseButton.Left) && InternalCalls.CheckMouseIntersectEntity(SettingsScript.scroll_bar_entity))
            {
                is_adjusting_scroll = true;
            }

            if (InternalCalls.MouseHeld((int)MouseButton.Left) && is_adjusting_scroll)
            {
                AdjustScroll(mouse_pos.y);
            }

            // If the mouse button is released, stop adjusting the slider
            if (yoffset == 0 && InternalCalls.MouseReleased((int)MouseButton.Left))
            {
                is_adjusting_scroll = false;
            }

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
                //click
                SettingsScript.PlayClickSound();
            }

            x_pos = origin.x + (0.593f * win_dimension.x);
            if (SettingsScript.show_settings)
            {
                if (!first_open_settings)
                {
                    adjustment = origin.y + diff_y * win_dimension.y;
                    first_open_settings = true;
                }
                InternalCalls.TransformSetPosition(x_pos, adjustment);
                virtual_y = adjustment - upper_limit_scroll_track;
                virtual_y *= 1.9f;
                //virtual_y = -virtual_y;

            }
            if (!SettingsScript.show_settings)
            {
                first_open_settings = false;
                InternalCalls.TransformSetPosition(9999f, 9999f);
            }

        }

        static public void CleanUp()
        {

        }

        static private void AdjustScroll(float ypos)
        {
            adjustment = Math.Clamp(ypos, lower_limit_scroll_track, upper_limit_scroll_track);
            diff_y = (adjustment - origin.y) / win_dimension.y;
            InternalCalls.TransformSetPosition(origin.x + (0.593f * win_dimension.x), adjustment);
            SettingsScript.scroll_bar_tracker_pos.y = adjustment;
            virtual_y = adjustment - upper_limit_scroll_track;
            virtual_y *= 1.85f;
        }
    }
}

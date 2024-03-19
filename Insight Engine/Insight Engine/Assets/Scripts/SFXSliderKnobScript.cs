/*!
 * \file SFXSliderKnobScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This C# file contains the SFXSliderKnobScript class, used for the SFXSliderKnob in settings page.
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
    class SFXSliderKnobScript
    {
        static private bool is_adjusting_slider = false;
        static public bool first_hover = false;
        static private int id;
        static private float diff_x;
        static private bool first_open_settings = false;
        static private float adjustment;
        static public float normalised_adjustment;
        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public float lower_limit_sfx_knob;
        static public float upper_limit_sfx_knob;
        static public float y_pos;

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

            lower_limit_sfx_knob = origin.x + (MasterSliderKnobScript.LOWER_LIMIT_SCALE * win_dimension.x);
            upper_limit_sfx_knob = origin.x + (MasterSliderKnobScript.UPPER_LIMIT_SCALE * win_dimension.x);

            first_open_settings = false;
            diff_x = 0.5f;

            //Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());
            adjustment = origin.x + diff_x * win_dimension.x;
            normalised_adjustment = (adjustment + upper_limit_sfx_knob) / (upper_limit_sfx_knob + upper_limit_sfx_knob);
            //InternalCalls.TransformSetPosition(origin.x + (0.5f * win_dimension.x), origin.y + (0.323f * win_dimension.y));
            InternalCalls.SetButtonHoverScale(id, 0.95f);

        }

        static public void Update()
        {
            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            lower_limit_sfx_knob = origin.x + (MasterSliderKnobScript.LOWER_LIMIT_SCALE * win_dimension.x);
            upper_limit_sfx_knob = origin.x + (MasterSliderKnobScript.UPPER_LIMIT_SCALE * win_dimension.x);
            Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());

            if (InternalCalls.MousePressed((int)MouseButton.Left) && InternalCalls.CheckMouseIntersectEntity(SettingsScript.sfx_slider_bar_entity))
            {
                is_adjusting_slider = true;
            }

            if (InternalCalls.MouseHeld((int)MouseButton.Left) && is_adjusting_slider)
            {
                AdjustSlider(mouse_pos.x);
                AdjustVolume();
            }

            // If the mouse button is released, stop adjusting the slider
            if (InternalCalls.MouseReleased((int)MouseButton.Left))
            {
                is_adjusting_slider = false;
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
                SettingsScript.PlayClickSound();
            }
            y_pos = origin.y + (0.321f * win_dimension.y) - ScrollBarTrackerScript.virtual_y;
            if (SettingsScript.show_settings)
            {
                if (!first_open_settings)
                {
                    adjustment = origin.x + diff_x * win_dimension.x;
                    first_open_settings = true;
                }
                InternalCalls.TransformSetPosition(adjustment, y_pos);
            }
            if (!SettingsScript.show_settings || y_pos > (origin.y + (0.7f * win_dimension.y)) || y_pos < (origin.y + (0.25f * win_dimension.y))) 
            {
                first_open_settings = false;
                InternalCalls.TransformSetPosition(9999f, 9999f);
            }
            SettingsScript.sfx_multiplier = normalised_adjustment;

        }


        static public void CleanUp()
        {

        }

        static public void AdjustSlider(float xpos)
        {
            adjustment = Math.Min(upper_limit_sfx_knob, Math.Max(lower_limit_sfx_knob, xpos));
            diff_x = (adjustment - origin.x) / win_dimension.x;
            InternalCalls.TransformSetPosition(adjustment, SettingsScript.sfx_slider_knob_pos.y);
            SettingsScript.sfx_slider_knob_pos.x = adjustment;
            normalised_adjustment = (adjustment + upper_limit_sfx_knob) / (upper_limit_sfx_knob + upper_limit_sfx_knob);
        }

        static public void AdjustVolume()
        {
            //sfx volume
            float xpos = InternalCalls.GetTransformPosition().x;
            float sfx_volume = (xpos - lower_limit_sfx_knob) / (lower_limit_sfx_knob - upper_limit_sfx_knob);
            InternalCalls.AudioSetSFX(sfx_volume);
        }
    }
}

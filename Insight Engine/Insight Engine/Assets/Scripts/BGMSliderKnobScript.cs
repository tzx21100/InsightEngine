/*!
 * \file BGMSliderKnobScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This C# file contains the BGMSliderKnobScript class, used for the BGMSliderKnob in settings page.
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
    class BGMSliderKnobScript
    {
        static public bool first_hover = false;
        static private int id;
        static private float diff_x;

        static private bool is_adjusting_slider = false;
        static private bool first_open_settings = false;
        static private float adjustment;
        static public float normalised_adjustment;

        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

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

            first_open_settings = false;

            float lower_limit = MasterSliderKnobScript.lower_limit_master_knob;
            float upper_limit = MasterSliderKnobScript.upper_limit_master_knob;

            diff_x = (upper_limit - lower_limit) * InternalCalls.AudioGetBGM();
            adjustment = diff_x;
            normalised_adjustment = (adjustment + lower_limit) / (upper_limit + lower_limit);

            InternalCalls.SetButtonHoverScale(id, 0.95f);

        }

        static public void Update()
        {
            float lower_limit = MasterSliderKnobScript.lower_limit_master_knob;
            float upper_limit = MasterSliderKnobScript.upper_limit_master_knob;

            diff_x = (upper_limit - lower_limit) * InternalCalls.AudioGetBGM();

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

            Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());

            if (InternalCalls.MousePressed((int)MouseButton.Left) && InternalCalls.CheckMouseIntersectEntity(SettingsScript.bgm_slider_bar_entity))
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

            y_pos = origin.y + (0.431f * win_dimension.y) - ScrollBarTrackerScript.virtual_y;
            if (SettingsScript.show_settings)
            {
                if (!first_open_settings)
                {
                    adjustment = MasterSliderKnobScript.lower_limit_master_knob + diff_x;
                    first_open_settings = true;
                }
                InternalCalls.TransformSetPosition(adjustment, y_pos);
                
            }
            if (!SettingsScript.show_settings || y_pos > (origin.y + (0.7f * win_dimension.y)) || y_pos < (origin.y + (0.25f * win_dimension.y)))
            {
                
                first_open_settings = false;
                InternalCalls.TransformSetPosition(9999f, 9999f);
            }
        }


        static public void CleanUp()
        {

        }

        static public void AdjustSlider(float xpos)
        {
            float lower_limit = MasterSliderKnobScript.lower_limit_master_knob;
            float upper_limit = MasterSliderKnobScript.upper_limit_master_knob;

            adjustment = Math.Min(upper_limit, Math.Max(lower_limit, xpos));
            diff_x = adjustment - lower_limit;
            InternalCalls.TransformSetPosition(adjustment, SettingsScript.bgm_slider_knob_pos.y);
            SettingsScript.bgm_slider_knob_pos.x = adjustment;
            normalised_adjustment = (adjustment + upper_limit) / (upper_limit + lower_limit);
        }

        static public void AdjustVolume()
        {
            float lower_limit = MasterSliderKnobScript.lower_limit_master_knob;
            float upper_limit = MasterSliderKnobScript.upper_limit_master_knob;

            float xpos = InternalCalls.GetTransformPosition().x;
            float bgm_volume = (xpos - lower_limit) / (upper_limit - lower_limit);
            InternalCalls.AudioSetBGM(bgm_volume);
            //Console.WriteLine("BGM: " + (int)(bgm_volume * 100) + "%");
        }
    }
}

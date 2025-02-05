/*!
 * \file SettingsScript.cs
 * \author Wu Zekai, zekai.wu@digipen.edu (40%), Matthew Ng, matthewdeen.ng@digipen.edu (70%)
 * \par Course: CSD2451
 * \brief
 * This c# file contains the SettingsScript class, used for settings menu of the game
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
    enum ButtonStates : int { Idle, Hovered, Pressed };

    enum MouseButton : int { Left, Right, Middle };

    class SettingsScript
    {
        public const string BODY_FONT_NAME = "Semplicita_Light";
        public const string HOVER_SFX_NAME = "Footsteps_Dirt-Gravel-Far-Small_1.wav";
        public const string CLICK_SFX_NAME = "QubieSFX3.wav";
        public const float HOVER_SFX_VOLUME = .15f;
        public const float CLICK_SFX_VOLUME = .4f;
        public const float TITLE_FONT_SIZE = 16.5f;
        public const float BODY_FONT_SIZE = 10f;
        static public (float, float, float, float) FONT_COLOR = (1f, 1f, 1f, 1f); // white color

        static public bool show_settings = false;
        static public bool first_hover = false;

        static public float font_size = MainMenuScript.NORMAL_FONT_SIZE;

        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public int background_entity;
        static public int settings_overlay_entity;
        static public int settings_overlay_top_entity;
        static public int settings_overlay_bot_entity;
        static public int master_checkbox_entity;
        static public int bgm_checkbox_entity;
        static public int sfx_checkbox_entity;
        static public int master_slider_bar_entity;
        static public int bgm_slider_bar_entity;
        static public int sfx_slider_bar_entity; 
        static public int master_slider_knob_entity;
        static public int bgm_slider_knob_entity;
        static public int sfx_slider_knob_entity;
        static public int fullscreen_checkbox_entity;
        static public int scroll_bar_entity;
        static public int scroll_bar_tracker_entity;
        static public int back_button_entity;

        static public SimpleImage background_image;
        static public SimpleImage settings_overlay_image;
        static public SimpleImage settings_overlay_top_image;
        static public SimpleImage settings_overlay_bot_image;
        static public SimpleImage checkbox_image;
        static public SimpleImage checkbox_toggled_image;
        static public SimpleImage slider_bar_image;
        static public SimpleImage slider_knob_image;
        static public SimpleImage scroll_bar_image;
        static public SimpleImage scroll_bar_tracker_image;
        static public SimpleImage back_button_image;

        static Vector2D background_pos = new Vector2D(0, 0);
        static Vector2D settings_overlay_pos = new Vector2D(0, 0);
        static Vector2D settings_overlay_top_pos = new Vector2D(0, 0);
        static Vector2D settings_overlay_bot_pos = new Vector2D(0, 0);

        static public Vector2D master_checkbox_pos = new Vector2D(0, 0);
        static public Vector2D bgm_checkbox_pos = new Vector2D(0, 0);
        static public Vector2D sfx_checkbox_pos = new Vector2D(0, 0);
        static public Vector2D master_slider_bar_pos = new Vector2D(0, 0);
        static public Vector2D bgm_slider_bar_pos = new Vector2D(0, 0);
        static public Vector2D sfx_slider_bar_pos = new Vector2D(0, 0);
        static public Vector2D master_slider_knob_pos = new Vector2D(0, 0);
        static public Vector2D bgm_slider_knob_pos = new Vector2D(0, 0);
        static public Vector2D sfx_slider_knob_pos = new Vector2D(0, 0);

        static public Vector2D scroll_bar_pos = new Vector2D(0, 0);
        static public Vector2D scroll_bar_tracker_pos = new Vector2D(0, 0);
        static public Vector2D back_button_pos = new Vector2D(0, 0);

        static public Vector2D settings_text_pos = new Vector2D(0, 0);
        static public Vector2D master_vol_text_pos = new Vector2D(0, 0);
        static public Vector2D bgm_vol_text_pos = new Vector2D(0, 0);
        static public Vector2D sfx_vol_text_pos = new Vector2D(0, 0);
        static public Vector2D fullscreen_text_pos = new Vector2D(0, 0);
        static public Vector2D fullscreen_checkbox_pos = new Vector2D(0, 0);

        static public void Init()
        {            
            show_settings = false;
            win_dimension.x = InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            background_image = InternalCalls.GetSpriteImage("main_menu_bg.jpeg");
            settings_overlay_image = InternalCalls.GetSpriteImage("settings_overlay.png");
            checkbox_image = InternalCalls.GetSpriteImage("checkbox.png");
            checkbox_toggled_image = InternalCalls.GetSpriteImage("checkbox_toggled.png");
            slider_bar_image = InternalCalls.GetSpriteImage("slider_bar.png");
            slider_knob_image = InternalCalls.GetSpriteImage("slider_knob.png");
            scroll_bar_image = InternalCalls.GetSpriteImage("scroll_bar.png");
            scroll_bar_tracker_image = InternalCalls.GetSpriteImage("scroll_bar_tracker.png");
            back_button_image = InternalCalls.GetSpriteImage("back_button.png");
            settings_overlay_top_image = InternalCalls.GetSpriteImage("settings_overlay_top.png");
            settings_overlay_bot_image = InternalCalls.GetSpriteImage("settings_overlay_bot.png");

            background_entity = InternalCalls.CreateEntityUI("Background", background_image);
            settings_overlay_entity = InternalCalls.CreateEntityUI("Settings Overlay", settings_overlay_image);
            master_checkbox_entity = InternalCalls.CreateEntityButtonNoText("Master Checkbox", InternalCalls.AudioIsMasterMute() ? checkbox_image : checkbox_toggled_image, "MasterCheckboxScript");
            bgm_checkbox_entity = InternalCalls.CreateEntityButtonNoText("BGM Checkbox", InternalCalls.AudioIsBGMMute() ? checkbox_image : checkbox_toggled_image, "BGMCheckboxScript");
            sfx_checkbox_entity = InternalCalls.CreateEntityButtonNoText("SFX Checkbox", InternalCalls.AudioIsSFXMute() ? checkbox_image : checkbox_toggled_image, "SFXCheckboxScript");
            master_slider_bar_entity = InternalCalls.CreateEntityUIScript("Master Slider Bar", slider_bar_image, "MasterSliderBarScript");
            bgm_slider_bar_entity = InternalCalls.CreateEntityUIScript("BGM Slider Bar", slider_bar_image, "BGMSliderBarScript");
            sfx_slider_bar_entity = InternalCalls.CreateEntityUIScript("SFX Slider Bar", slider_bar_image, "SFXSliderBarScript");
            master_slider_knob_entity = InternalCalls.CreateEntityButtonNoText("Master Slider Knob", slider_knob_image, "MasterSliderKnobScript");
            bgm_slider_knob_entity = InternalCalls.CreateEntityButtonNoText("BGM Slider Knob", slider_knob_image, "BGMSliderKnobScript");
            sfx_slider_knob_entity = InternalCalls.CreateEntityButtonNoText("SFX Slider Knob", slider_knob_image, "SFXSliderKnobScript");
            fullscreen_checkbox_entity = InternalCalls.CreateEntityButtonNoText("Fullscreen Checkbox", InternalCalls.IsFullscreen() ? checkbox_toggled_image : checkbox_image, "FullscreenCheckboxScript");

            scroll_bar_entity = InternalCalls.CreateEntityUIScript("Scroll Bar", scroll_bar_image, "ScrollBarScript");
            scroll_bar_tracker_entity = InternalCalls.CreateEntityButtonNoText("Scroll Bar Tracker", scroll_bar_tracker_image, "ScrollBarTrackerScript");
            back_button_entity = InternalCalls.CreateEntityButtonNoText("Back Button", back_button_image, "BackFromSettingsButtonScript");
            settings_overlay_top_entity = InternalCalls.CreateEntityUI("Settings Overlay Top", settings_overlay_top_image);
            settings_overlay_bot_entity = InternalCalls.CreateEntityUI("Settings Overlay Bottom", settings_overlay_bot_image);
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
            const float CHECKBOX_SCALE           = 0.02f;
            const float SLIDER_BAR_SCALE         = 0.16f;
            const float SLIDER_BAR_AR            = 86f / 609f;
            const float SLIDER_KNOB_SCALE        = 0.7f;
            const float SCROLL_BAR_SCALE         = 0.018f;
            const float SCROLL_BAR_AR            = 777f / 52f;
            const float SCROLL_BAR_TRACKER_SCALE = 25f / 52f;
            const float SCROLL_BAR_TRACKER_AR    = 570f / 24f;

            float checkbox_scaled_uniform           = CHECKBOX_SCALE * win_dimension.x;
            float slider_bar_scaled_width           = SLIDER_BAR_SCALE * win_dimension.x;
            float slider_bar_scaled_height          = SLIDER_BAR_AR * slider_bar_scaled_width;
            float slider_knob_scaled_uniform        = SLIDER_KNOB_SCALE * slider_bar_scaled_height;
            float scroll_bar_scaled_width           = SCROLL_BAR_SCALE * win_dimension.y;
            float scroll_bar_scaled_height          = SCROLL_BAR_AR * scroll_bar_scaled_width;
            float scroll_bar_tracker_scaled_width   = SCROLL_BAR_TRACKER_SCALE * scroll_bar_scaled_width;
            float scroll_bar_tracker_scaled_height  = SCROLL_BAR_TRACKER_AR * scroll_bar_tracker_scaled_width;

            Vector2D background = new Vector2D(win_dimension.x, win_dimension.y);
            Vector2D settings_overlay = new Vector2D(win_dimension.x, win_dimension.y);
            Vector2D settings_overlay_top = new Vector2D(0.8f * win_dimension.x, 0.224f * win_dimension.y);
            Vector2D settings_overlay_bot = new Vector2D(0.8f * win_dimension.x, 0.15f * win_dimension.y);

            SimpleVector2D checkbox_size = new SimpleVector2D(checkbox_scaled_uniform, checkbox_scaled_uniform);
            SimpleVector2D slider_knob_size = new SimpleVector2D(slider_knob_scaled_uniform, slider_knob_scaled_uniform);
            Vector2D back = new Vector2D(0.12f * win_dimension.x, 0.12f * win_dimension.x);

            InternalCalls.TransformSetScaleEntity(background.x, background.y, background_entity);
            InternalCalls.TransformSetScaleEntity(settings_overlay.x, settings_overlay.y, settings_overlay_entity);
            InternalCalls.TransformSetScaleEntity(settings_overlay_top.x, settings_overlay_top.y, settings_overlay_top_entity);
            InternalCalls.TransformSetScaleEntity(settings_overlay_bot.x, settings_overlay_bot.y, settings_overlay_bot_entity);
            InternalCalls.SetButtonSize(master_checkbox_entity, checkbox_size);
            InternalCalls.SetButtonSize(bgm_checkbox_entity, checkbox_size);
            InternalCalls.SetButtonSize(sfx_checkbox_entity, checkbox_size);
            InternalCalls.TransformSetScaleEntity(slider_bar_scaled_width, slider_bar_scaled_height, master_slider_bar_entity);
            InternalCalls.TransformSetScaleEntity(slider_bar_scaled_width, slider_bar_scaled_height, bgm_slider_bar_entity);
            InternalCalls.TransformSetScaleEntity(slider_bar_scaled_width, slider_bar_scaled_height, sfx_slider_bar_entity);
            InternalCalls.SetButtonSize(master_slider_knob_entity, slider_knob_size);
            InternalCalls.SetButtonSize(bgm_slider_knob_entity, slider_knob_size);
            InternalCalls.SetButtonSize(sfx_slider_knob_entity, slider_knob_size);
            InternalCalls.SetButtonSize(fullscreen_checkbox_entity, checkbox_size);
            InternalCalls.TransformSetScaleEntity(scroll_bar_scaled_width, scroll_bar_scaled_height, scroll_bar_entity);
            InternalCalls.SetButtonSize(scroll_bar_tracker_entity, new SimpleVector2D(scroll_bar_tracker_scaled_width, scroll_bar_tracker_scaled_height));
            InternalCalls.SetButtonSize(back_button_entity, new SimpleVector2D(back.x, back.y));

            // Positions    
            background_pos.Set(camera_pos.x, camera_pos.y);
            settings_overlay_pos.Set(camera_pos.x, camera_pos.y);
            settings_overlay_top_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.733f * win_dimension.y));
            settings_overlay_bot_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.222f * win_dimension.y));

            master_checkbox_pos.Set(MasterCheckboxScript.x_pos, MasterCheckboxScript.y_pos);
            bgm_checkbox_pos.Set(BGMCheckboxScript.x_pos, BGMCheckboxScript.y_pos);
            sfx_checkbox_pos.Set(SFXCheckboxScript.x_pos, SFXCheckboxScript.y_pos);

            master_slider_bar_pos.Set(MasterSliderBarScript.x_pos, MasterSliderBarScript.y_pos);
            bgm_slider_bar_pos.Set(BGMSliderBarScript.x_pos, BGMSliderBarScript.y_pos);
            sfx_slider_bar_pos.Set(SFXSliderBarScript.x_pos, SFXSliderBarScript.y_pos);

            master_slider_knob_pos.Set(master_slider_knob_pos.x, MasterSliderKnobScript.y_pos);
            bgm_slider_knob_pos.Set(bgm_slider_knob_pos.x, BGMSliderKnobScript.y_pos);
            sfx_slider_knob_pos.Set(sfx_slider_knob_pos.x, SFXSliderKnobScript.y_pos);

            fullscreen_checkbox_pos.Set(FullscreenCheckboxScript.x_pos, FullscreenCheckboxScript.y_pos);

            scroll_bar_pos.Set(ScrollBarScript.x_pos, ScrollBarScript.y_pos);
            scroll_bar_tracker_pos.Set(ScrollBarTrackerScript.x_pos, scroll_bar_tracker_pos.y);

            back_button_pos.Set(origin.x + (0.08f * win_dimension.x), origin.y + (0.9f * win_dimension.y));

            settings_text_pos   = ConvertAbsoluteToNormalizedPosition(origin.x + (0.5f * win_dimension.x), origin.y + (0.68f * win_dimension.y));
            master_vol_text_pos = ConvertAbsoluteToNormalizedPosition(origin.x + (0.505f * win_dimension.x), origin.y + (0.585f * win_dimension.y) - ScrollBarTrackerScript.virtual_y);
            bgm_vol_text_pos    = ConvertAbsoluteToNormalizedPosition(origin.x + (0.5f * win_dimension.x), origin.y + (0.475f * win_dimension.y) - ScrollBarTrackerScript.virtual_y);
            sfx_vol_text_pos    = ConvertAbsoluteToNormalizedPosition(origin.x + (0.495f * win_dimension.x), origin.y + (0.365f * win_dimension.y) - ScrollBarTrackerScript.virtual_y);
            fullscreen_text_pos = ConvertAbsoluteToNormalizedPosition(origin.x + (0.49f * win_dimension.x), origin.y + (0.255f * win_dimension.y) - ScrollBarTrackerScript.virtual_y);

            //hovered
            if (InternalCalls.GetButtonState() == (int)ButtonStates.Hovered)
            {
                //hovering
                if (!first_hover)
                {
                    PlayHoverSound();
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
                PlayClickSound();
                show_settings = true;

            }

            if (show_settings)
            {
                DrawSettings();
                InternalCalls.RenderTextFont("SETTINGS", "Cinzel_SemiBold", settings_text_pos.x, settings_text_pos.y, TITLE_FONT_SIZE, FONT_COLOR);

                float upper_limit_normalized = ConvertAbsoluteToNormalizedPosition(0f, origin.y + (0.615f * win_dimension.y)).y;
                float lower_limit_normalized = ConvertAbsoluteToNormalizedPosition(0f, origin.y + (0.315f * win_dimension.y)).y; //may need to recalc

                if (!(master_vol_text_pos.y > upper_limit_normalized) && !(master_vol_text_pos.y < lower_limit_normalized))
                    InternalCalls.RenderTextFont("Master Volume", BODY_FONT_NAME, master_vol_text_pos.x, master_vol_text_pos.y, BODY_FONT_SIZE, FONT_COLOR);
                if (!(bgm_vol_text_pos.y > upper_limit_normalized) && !(bgm_vol_text_pos.y < lower_limit_normalized))
                    InternalCalls.RenderTextFont("BGM Volume", BODY_FONT_NAME, bgm_vol_text_pos.x, bgm_vol_text_pos.y, BODY_FONT_SIZE, FONT_COLOR);
                if (!(sfx_vol_text_pos.y > upper_limit_normalized) && !(sfx_vol_text_pos.y < lower_limit_normalized))
                    InternalCalls.RenderTextFont("SFX Volume", BODY_FONT_NAME, sfx_vol_text_pos.x, sfx_vol_text_pos.y, BODY_FONT_SIZE, FONT_COLOR);
                if (!(fullscreen_text_pos.y > upper_limit_normalized) && !(fullscreen_text_pos.y < lower_limit_normalized))
                    InternalCalls.RenderTextFont("Fullscreen", BODY_FONT_NAME, fullscreen_text_pos.x, fullscreen_text_pos.y, BODY_FONT_SIZE, FONT_COLOR);
            }
            else
            {
                HideSettings(); 
            }
        }

        static public void CleanUp()
        {

        }

        //Method to convert an absolute position to a normalized position
        static public Vector2D ConvertAbsoluteToNormalizedPosition(float x, float y)
        {
            float normalized_x = (x - origin.x) / win_dimension.x;
            float normalized_y = (y - origin.y) / win_dimension.y;

            // Ensure the normalized values are clamped between 0 and 1 to avoid going out of bounds
            normalized_x = Math.Clamp(normalized_x, 0f, 1f);
            normalized_y = Math.Clamp(normalized_y, 0f, 1f);

            return new Vector2D(normalized_x, normalized_y);
        }

        static private void DrawSettings()
        {
            InternalCalls.TransformSetPositionEntity(background_pos.x, background_pos.y, background_entity);
            InternalCalls.TransformSetPositionEntity(settings_overlay_pos.x, settings_overlay_pos.y, settings_overlay_entity);
            InternalCalls.TransformSetPositionEntity(settings_overlay_top_pos.x, settings_overlay_top_pos.y, settings_overlay_top_entity);
            InternalCalls.TransformSetPositionEntity(settings_overlay_bot_pos.x, settings_overlay_bot_pos.y, settings_overlay_bot_entity);
            InternalCalls.TransformSetPositionEntity(back_button_pos.x, back_button_pos.y, back_button_entity);
        }


        static private void HideSettings()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, background_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, settings_overlay_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, settings_overlay_top_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, settings_overlay_bot_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, back_button_entity); 
            //InternalCalls.TransformSetPositionEntity(9999f, 9999f, trans_entity);
        }

        static public void PlayHoverSound()
        {
            InternalCalls.AudioPlaySoundSFX(HOVER_SFX_NAME, false, HOVER_SFX_VOLUME);
        }

        static public void PlayClickSound()
        {
            InternalCalls.AudioPlaySoundSFX(CLICK_SFX_NAME, false, CLICK_SFX_VOLUME);
        }
    }
}

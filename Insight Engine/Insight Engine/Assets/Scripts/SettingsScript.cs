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
using System;
namespace IS
{
    class SettingsScript
    {
        static public bool show_settings = false;
        static public bool first_hover = false;

        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static private int background_entity;
        static private int settings_overlay_entity;
        static private int master_checkbox_entity;
        static private int bgm_checkbox_entity;
        static private int vfx_checkbox_entity;
        static private int master_slider_bar_entity;
        static private int bgm_slider_bar_entity;
        static private int vfx_slider_bar_entity; 
        static private int master_slider_knob_entity;
        static private int bgm_slider_knob_entity;
        static private int vfx_slider_knob_entity;
        static private int back_button_entity;

        static SimpleImage background_image;
        static SimpleImage settings_overlay_image;
        static SimpleImage master_checkbox_image;
        static SimpleImage bgm_checkbox_image;
        static SimpleImage vfx_checkbox_image;
        static SimpleImage slider_bar_image;
        static SimpleImage slider_knob_image;
        static SimpleImage back_button_image;

        static Vector2D background_pos = new Vector2D(0, 0);
        static Vector2D settings_overlay_pos = new Vector2D(0, 0);
        static Vector2D master_checkbox_pos = new Vector2D(0, 0);
        static Vector2D bgm_checkbox_pos = new Vector2D(0, 0);
        static Vector2D vfx_checkbox_pos = new Vector2D(0, 0);
        static Vector2D master_slider_bar_pos = new Vector2D(0, 0);
        static Vector2D bgm_slider_bar_pos = new Vector2D(0, 0);
        static Vector2D vfx_slider_bar_pos = new Vector2D(0, 0);
        static public Vector2D master_slider_knob_pos = new Vector2D(0, 0);
        static public Vector2D bgm_slider_knob_pos = new Vector2D(0, 0);
        static public Vector2D vfx_slider_knob_pos = new Vector2D(0, 0);
        static Vector2D back_button_pos = new Vector2D(0, 0);

        static public float upper_limit = origin.x + (0.558f * win_dimension.x);
        static public float lower_limit = origin.x + (0.442f * win_dimension.x);

        static public float master_multiplier;
        static public float bgm_multiplier;
        static public float vfx_multiplier;

        static public float bgm_vol;
        static public float vfx_vol;
        static public void Init()
        {
            show_settings = false;
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            master_multiplier = MasterSliderKnobScript.normalised_adjustment;
            bgm_multiplier = BGMSliderKnobScript.normalised_adjustment;
            vfx_multiplier = VFXSliderKnobScript.normalised_adjustment;
            bgm_vol = master_multiplier * bgm_multiplier;
            vfx_vol = master_multiplier * vfx_multiplier;
            if (MasterCheckboxScript.toggled)
            {
                master_checkbox_image = MasterCheckboxScript.toggled_image;
            }
            else
            {
                master_checkbox_image = MasterCheckboxScript.checkbox_image;

            }

            if (BGMCheckboxScript.toggled)
            {
                bgm_checkbox_image = BGMCheckboxScript.toggled_image;
            }
            else
            {
                bgm_checkbox_image = BGMCheckboxScript.checkbox_image;

            }
            if (VFXCheckboxScript.toggled)
            {
                vfx_checkbox_image = VFXCheckboxScript.toggled_image;
            }
            else
            {
                vfx_checkbox_image = VFXCheckboxScript.checkbox_image;

            }
            background_image = InternalCalls.GetSpriteImage("main_menu_bg.jpeg");
            settings_overlay_image = InternalCalls.GetSpriteImage("settings_overlay.png");
            slider_bar_image = InternalCalls.GetSpriteImage("slider_bar.png");
            slider_knob_image = InternalCalls.GetSpriteImage("slider_knob.png");
            back_button_image = InternalCalls.GetSpriteImage("back_button.png");

            background_entity = InternalCalls.CreateEntityUI("Background", background_image);
            settings_overlay_entity = InternalCalls.CreateEntityUI("Settings Overlay", settings_overlay_image);
            master_checkbox_entity = InternalCalls.CreateEntityButtonNoText("Master Checkbox", master_checkbox_image, "MasterCheckboxScript");
            bgm_checkbox_entity = InternalCalls.CreateEntityButtonNoText("BGM Checkbox", bgm_checkbox_image, "BGMCheckboxScript");
            vfx_checkbox_entity = InternalCalls.CreateEntityButtonNoText("VFX Checkbox", vfx_checkbox_image, "VFXCheckboxScript");
            master_slider_bar_entity = InternalCalls.CreateEntityUI("Master Slider", slider_bar_image);
            bgm_slider_bar_entity = InternalCalls.CreateEntityUI("BGM Slider", slider_bar_image);
            vfx_slider_bar_entity = InternalCalls.CreateEntityUI("VFX Slider", slider_bar_image);
            master_slider_knob_entity = InternalCalls.CreateEntityButtonNoText("Master Slider Knob", slider_knob_image, "MasterSliderKnobScript");
            bgm_slider_knob_entity = InternalCalls.CreateEntityButtonNoText("BGM Slider Knob", slider_knob_image, "BGMSliderKnobScript");
            vfx_slider_knob_entity = InternalCalls.CreateEntityButtonNoText("VFX Slider Knob", slider_knob_image, "VFXSliderKnobScript");
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
            Vector2D master_checkbox = new Vector2D(0.08f * win_dimension.x, 0.08f * win_dimension.x);
            Vector2D bgm_checkbox = new Vector2D(0.08f * win_dimension.x, 0.08f * win_dimension.x);
            Vector2D vfx_checkbox = new Vector2D(0.08f * win_dimension.x, 0.08f * win_dimension.x);
            Vector2D master_slider_bar = new Vector2D(0.27f * win_dimension.x, 0.27f * win_dimension.y);
            Vector2D bgm_slider_bar = new Vector2D(0.27f * win_dimension.x, 0.27f * win_dimension.y);
            Vector2D vfx_slider_bar = new Vector2D(0.27f * win_dimension.x, 0.27f * win_dimension.y);
            Vector2D master_slider_knob = new Vector2D(0.04f * win_dimension.x, 0.04f * win_dimension.x);
            Vector2D bgm_slider_knob = new Vector2D(0.04f * win_dimension.x, 0.04f * win_dimension.x);
            Vector2D vfx_slider_knob = new Vector2D(0.04f * win_dimension.x, 0.04f * win_dimension.x);
            Vector2D back = new Vector2D(0.12f * win_dimension.x, 0.12f * win_dimension.x);

            InternalCalls.TransformSetScaleEntity(background.x, background.y, background_entity);
            InternalCalls.TransformSetScaleEntity(settings_overlay.x, settings_overlay.y, settings_overlay_entity);
            InternalCalls.SetButtonSize(master_checkbox_entity, new SimpleVector2D(master_checkbox.x, master_checkbox.y));
            InternalCalls.SetButtonSize(bgm_checkbox_entity, new SimpleVector2D(bgm_checkbox.x, bgm_checkbox.y));
            InternalCalls.SetButtonSize(vfx_checkbox_entity, new SimpleVector2D(vfx_checkbox.x, vfx_checkbox.y));
            InternalCalls.TransformSetScaleEntity(master_slider_bar.x, master_slider_bar.y, master_slider_bar_entity);
            InternalCalls.TransformSetScaleEntity(bgm_slider_bar.x, bgm_slider_bar.y, bgm_slider_bar_entity);
            InternalCalls.TransformSetScaleEntity(vfx_slider_bar.x, vfx_slider_bar.y, vfx_slider_bar_entity);
            InternalCalls.SetButtonSize(master_slider_knob_entity, new SimpleVector2D(master_slider_knob.x, master_slider_knob.y));
            InternalCalls.SetButtonSize(bgm_slider_knob_entity, new SimpleVector2D(bgm_slider_knob.x, bgm_slider_knob.y));
            InternalCalls.SetButtonSize(vfx_slider_knob_entity, new SimpleVector2D(vfx_slider_knob.x, vfx_slider_knob.y));
            InternalCalls.SetButtonSize(back_button_entity, new SimpleVector2D(back.x, back.y));

            // Positions    
            background_pos.Set(camera_pos.x, camera_pos.y);
            settings_overlay_pos.Set(camera_pos.x, camera_pos.y);
            master_checkbox_pos.Set(origin.x + (0.44f * win_dimension.x), origin.y + (0.585f * win_dimension.y));
            bgm_checkbox_pos.Set(origin.x + (0.44f * win_dimension.x), origin.y + (0.475f * win_dimension.y));
            vfx_checkbox_pos.Set(origin.x + (0.44f * win_dimension.x), origin.y + (0.365f * win_dimension.y));
            master_slider_bar_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.54f * win_dimension.y));
            bgm_slider_bar_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.43f * win_dimension.y));
            vfx_slider_bar_pos.Set(origin.x + (0.5f * win_dimension.x), origin.y + (0.32f * win_dimension.y));
            master_slider_knob_pos.Set(master_slider_knob_pos.x, origin.y + (0.543f * win_dimension.y));
            //bgm_slider_knob_pos.Set(origin.x + (0.442f * win_dimension.x), origin.y + (0.433f * win_dimension.y));
            bgm_slider_knob_pos.Set(bgm_slider_knob_pos.x, origin.y + (0.433f * win_dimension.y));
            vfx_slider_knob_pos.Set(vfx_slider_knob_pos.x, origin.y + (0.323f * win_dimension.y)); //442-558
            back_button_pos.Set(origin.x + (0.08f * win_dimension.x), origin.y + (0.9f * win_dimension.y));

          
      

            /*// Initial setting of images based on the toggled state of A (MasterCheckbox)
            if (MasterCheckboxScript.toggled)
            {
                // When A is on, B & C turn on
                master_checkbox_image = MasterCheckboxScript.toggled_image;
                bgm_checkbox_image = BGMCheckboxScript.toggled_image; // Ensure B is turned on
                vfx_checkbox_image = VFXCheckboxScript.toggled_image; // Ensure C is turned on

             *//*   if (BGMCheckboxScript.clicked)
                {
                    BGMCheckboxScript.toggled = false;
                    bgm_checkbox_image = BGMCheckboxScript.checkbox_image;
                }
                else
                {
                    BGMCheckboxScript.toggled=true;
                    bgm_checkbox_image= BGMCheckboxScript.checkbox_image;
                }*/
            /*if (VFXCheckboxScript.clicked)
            {
                VFXCheckboxScript.toggled = false;
                vfx_checkbox_image = VFXCheckboxScript.checkbox_image;
            }*//*
        }
        else
        {
            // When A is off, ensure images reflect the non-toggled state
            master_checkbox_image = MasterCheckboxScript.checkbox_image;
            bgm_checkbox_image = BGMCheckboxScript.checkbox_image;
            vfx_checkbox_image = VFXCheckboxScript.checkbox_image;

        }

        // Adjust logic for when A is off but B or C are toggled
        if (!MasterCheckboxScript.toggled)
        {
            if (BGMCheckboxScript.toggled)
            {
                // Only B is turned on/off based on its current state
                bgm_checkbox_image = BGMCheckboxScript.toggled_image;
            }
            else
            {
                bgm_checkbox_image = BGMCheckboxScript.checkbox_image;
            }

            if (VFXCheckboxScript.toggled)
            {
                // Only C is turned on/off based on its current state
                vfx_checkbox_image = VFXCheckboxScript.toggled_image;
            }
            else
            {
                vfx_checkbox_image = VFXCheckboxScript.checkbox_image;
            }
        }   

        // Ensure A is turned on if both B and C are toggled on separately
        if (BGMCheckboxScript.toggled && VFXCheckboxScript.toggled)
        {
            MasterCheckboxScript.toggled = true;
            master_checkbox_image = MasterCheckboxScript.toggled_image;
        }

        if (BGMCheckboxScript.toggled)
        {
            if (MasterCheckboxScript.clicked || VFXCheckboxScript.clicked)
            {
                MasterCheckboxScript.toggled = true;
                VFXCheckboxScript.toggled = true;
                master_checkbox_image = MasterCheckboxScript.toggled_image;
                vfx_checkbox_image = VFXCheckboxScript.toggled_image;
            }
            else
            {
                MasterCheckboxScript.toggled = false;
                VFXCheckboxScript.toggled = false;
                master_checkbox_image = MasterCheckboxScript.checkbox_image;
                vfx_checkbox_image = VFXCheckboxScript.checkbox_image;
            }
        }

        if (VFXCheckboxScript.toggled)
        {
            if (MasterCheckboxScript.clicked || BGMCheckboxScript.clicked)
            {
                MasterCheckboxScript.toggled = true;
                BGMCheckboxScript.toggled = true;
                master_checkbox_image = MasterCheckboxScript.toggled_image;
                bgm_checkbox_image = BGMCheckboxScript.toggled_image;
            }
            else
            {
                MasterCheckboxScript.toggled = false;
                BGMCheckboxScript.toggled = false;
                master_checkbox_image = MasterCheckboxScript.checkbox_image;
                bgm_checkbox_image = BGMCheckboxScript.checkbox_image;
            }
        }*/
            /*            if (BGMCheckboxScript.toggled && !VFXCheckboxScript.toggled)
                        {
                            MasterCheckboxScript.toggled = false;
                            master_checkbox_image = MasterCheckboxScript.checkbox_image;
                        }
                        if (!BGMCheckboxScript.toggled && VFXCheckboxScript.toggled)
                        {
                            MasterCheckboxScript.toggled = false;
                            master_checkbox_image = MasterCheckboxScript.checkbox_image;
                        }*/
            bgm_vol = master_multiplier * bgm_multiplier;
            vfx_vol = master_multiplier * vfx_multiplier;
            InternalCalls.SetSpriteImageEntity(master_checkbox_image, master_checkbox_entity);
            InternalCalls.SetSpriteImageEntity(bgm_checkbox_image, bgm_checkbox_entity);
            InternalCalls.SetSpriteImageEntity(vfx_checkbox_image, vfx_checkbox_entity);
            bgm_vol = master_multiplier * bgm_multiplier;
            vfx_vol = master_multiplier * vfx_multiplier;
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
            InternalCalls.TransformSetPositionEntity(master_checkbox_pos.x, master_checkbox_pos.y, master_checkbox_entity);
            InternalCalls.TransformSetPositionEntity(bgm_checkbox_pos.x, bgm_checkbox_pos.y, bgm_checkbox_entity);
            InternalCalls.TransformSetPositionEntity(vfx_checkbox_pos.x, vfx_checkbox_pos.y, vfx_checkbox_entity);
            InternalCalls.TransformSetPositionEntity(master_slider_bar_pos.x, master_slider_bar_pos.y, master_slider_bar_entity);
            InternalCalls.TransformSetPositionEntity(bgm_slider_bar_pos.x, bgm_slider_bar_pos.y, bgm_slider_bar_entity);
            InternalCalls.TransformSetPositionEntity(vfx_slider_bar_pos.x, vfx_slider_bar_pos.y, vfx_slider_bar_entity);
            InternalCalls.TransformSetPositionEntity(master_slider_knob_pos.x, master_slider_knob_pos.y, master_slider_knob_entity);
            InternalCalls.TransformSetPositionEntity(bgm_slider_knob_pos.x, bgm_slider_knob_pos.y, bgm_slider_knob_entity);
            InternalCalls.TransformSetPositionEntity(vfx_slider_knob_pos.x, vfx_slider_knob_pos.y, vfx_slider_knob_entity);
            InternalCalls.TransformSetPositionEntity(back_button_pos.x, back_button_pos.y, back_button_entity);
            /*   InternalCalls.RenderTextFont("SETTINGS", "MedusaGothic_D", origin.x + (0.5002604167f * win_dimension.x), origin.y + (0.5006548673f * win_dimension.y), 12.5f, (1f, 1f, 1f));
               InternalCalls.RenderTextFont("Master Volume", "Semplicita_Light", origin.x + (0.5002630208f * win_dimension.x), origin.y + (0.5005703048f * win_dimension.y), 10f, (1f, 1f, 1f));
               InternalCalls.RenderTextFont("BGM Volume", "Semplicita_Light", origin.x + (0.5002604167f * win_dimension.x), origin.y + (0.5004572271f * win_dimension.y), 10f, (1f, 1f, 1f));
               InternalCalls.RenderTextFont("VFX Volume", "Semplicita_Light", origin.x + (0.5002578125f * win_dimension.x), origin.y + (0.5003539823f * win_dimension.y), 10f, (1f, 1f, 1f));*/

            InternalCalls.RenderTextFont("SETTINGS", "MedusaGothic_D", 0.5f, 0.666f, 12.5f, (1f, 1f, 1f));
            InternalCalls.RenderTextFont("Master Volume", "Semplicita_Light", 0.505f, 0.58f, 10f, (1f, 1f, 1f));
            InternalCalls.RenderTextFont("BGM Volume", "Semplicita_Light", 0.5f, 0.465f, 10f, (1f, 1f, 1f));
            InternalCalls.RenderTextFont("VFX Volume", "Semplicita_Light", 0.495f, 0.36f, 10f, (1f, 1f, 1f));
        }

        static private void HideSettings()
        {
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, background_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, settings_overlay_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, master_checkbox_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, bgm_checkbox_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, vfx_checkbox_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, master_slider_bar_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, bgm_slider_bar_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, vfx_slider_bar_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, master_slider_knob_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, bgm_slider_knob_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, vfx_slider_knob_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, back_button_entity); 
        }


    }
}

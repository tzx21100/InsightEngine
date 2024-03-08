using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System;
namespace IS
{
    class ResolutionDropdownScript
    {
        // Windows
        static public Vector2D win_dimension = new Vector2D(0, 0);
        static public Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static public float x_pos;
        static public float y_pos;
        static private int id;
        static public bool first_hover = false;

        static public bool dropdown_open = false;

        //static private int dropdown_entity;
        //static private int dropdown_option_entity;
        static private int dropdown_option_highlight_entity;
        static private int dropdown_option_end_entity;

        static SimpleImage dropdown_minimized_image;
        static SimpleImage dropdown_open_image;
        static SimpleImage dropdown_option_image;
        static SimpleImage dropdown_option_highlight_image;
        static SimpleImage dropdown_option_end_image;

        static private List<string> resolution_options = new List<string>();
        static private List<int> option_entities = new List<int>();
        static private List<Vector2D> position_of_entities = new List<Vector2D>();

        static public void Init()
        {
            dropdown_minimized_image = InternalCalls.GetSpriteImage("dropdown_minimized.png");
            dropdown_open_image = InternalCalls.GetSpriteImage("dropdown_open.png");
            dropdown_option_image = InternalCalls.GetSpriteImage("dropdown_option.png");
            dropdown_option_highlight_image = InternalCalls.GetSpriteImage("dropdown_option_highlight.png");
            dropdown_option_end_image = InternalCalls.GetSpriteImage("dropdown_option_end.png");

            //dropdown_option_entity = InternalCalls.CreateEntityUI("Option", dropdown_option_image);
            dropdown_option_highlight_entity = InternalCalls.CreateEntityUI("Option Highlight", dropdown_option_highlight_image);
            dropdown_option_end_entity = InternalCalls.CreateEntityUI("Option End", dropdown_option_end_image);

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

            resolution_options.Add("800 x 600");
            resolution_options.Add("1024 x 768");
            resolution_options.Add("1280 x 720");
            resolution_options.Add("1600 x 900");
            resolution_options.Add("1920 x 1080");

            InternalCalls.SetButtonIdleAlpha(id, 1f);
            InternalCalls.SetButtonHoverScale(id, 0.9f);

            dropdown_open = false;
            CreateOptionEntities();
            for (int i = 0; i < option_entities.Count; i++)
            {
                position_of_entities.Add(new Vector2D(0, 0));
            }
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
            //hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hover)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
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
            if (InternalCalls.GetButtonState() == 2)
            {
                dropdown_open = !dropdown_open;
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
            }

            if (!dropdown_open)
            {
                x_pos = origin.x + (0.5f * win_dimension.x);
            }
            else
            {
                x_pos = origin.x + (0.5005f * win_dimension.x);
            }

            y_pos = origin.y + (0.215f * win_dimension.y) - ScrollBarTrackerScript.virtual_y;

            //sizes
            Vector2D option = new Vector2D(0.1591f * win_dimension.x, 0.015f * win_dimension.x); //651 x 57
            Vector2D option_highlight = new Vector2D(0.1591f * win_dimension.x, 0.031f * win_dimension.x); //651 x 140 
            Vector2D option_end = new Vector2D(0.1591f * win_dimension.x, 0.018f * win_dimension.x); //651 x 63
            //scale
            //InternalCalls.TransformSetScaleEntity(option.x, option.y, dropdown_option_entity);
            for (int i = 0; i < option_entities.Count - 1; i++)
            {
                InternalCalls.TransformSetScaleEntity(option.x, option.y, option_entities[i]);
            }
            InternalCalls.TransformSetScaleEntity(option_highlight.x, option_highlight.y, dropdown_option_highlight_entity);
            InternalCalls.TransformSetScaleEntity(option_end.x, option_end.y, dropdown_option_end_entity);

            for (int i = 0; i < option_entities.Count - 1; i++)
            {
                position_of_entities[i].x = x_pos - (0.0001f * win_dimension.x);
                position_of_entities[i].y = y_pos - (0.033f * win_dimension.y) - (i * 0.024f * win_dimension.y);
                position_of_entities[i].Set(position_of_entities[i].x, position_of_entities[i].y);
            }

            if (dropdown_open)
            {
                InternalCalls.SetSpriteImage(dropdown_open_image);
                Draw();
            }
            else
            {
                InternalCalls.SetSpriteImage(dropdown_minimized_image);
                Hide();
            }

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

        static private void Draw()
        {
            for (int i = 0; i < option_entities.Count; i++)
            {
                if (i < position_of_entities.Count)
                {
                    InternalCalls.TransformSetPositionEntity(position_of_entities[i].x, position_of_entities[i].y, option_entities[i]);
                }
            }

            //InternalCalls.TransformSetPositionEntity(option_highlight_pos.x, option_highlight_pos.y, dropdown_option_highlight_entity);
            //InternalCalls.TransformSetPositionEntity(option_end_pos.x, option_end_pos.y, dropdown_option_end_entity);
        }

        static private void Hide()
        {
            //InternalCalls.TransformSetPositionEntity(9999f, 9999f, dropdown_option_entity);
            for (int i = 0; i < option_entities.Count; i++)
            {
                if (i < position_of_entities.Count)
                {
                    InternalCalls.TransformSetPositionEntity(9999f, 9999f, option_entities[i]);
                }
            }
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, dropdown_option_highlight_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, dropdown_option_end_entity);
        }

        static private void CreateOptionEntities()
        {

            for (int i = 0; i < resolution_options.Count; i++)
            {
                string option = resolution_options[i];
                int entity = InternalCalls.CreateEntityUI(option, dropdown_option_image);
                option_entities.Add(entity);


            }
        }
    }
}

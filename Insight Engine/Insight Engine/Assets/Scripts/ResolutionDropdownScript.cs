using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System;
using System.Linq;

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
        static public bool options_created = false;
        static public List<string> resolution_options = new List<string>();
        static public Dictionary<int, string> option_entities = new Dictionary<int, string>();
        static public List<int> option_entities_list = new List<int>();
        static public List<Vector2D> position_of_entities = new List<Vector2D>();
        static public List<int> key_list = new List<int>();
        static public int last_key;
        static private int last_pos_index;
        static private string current_resolution;

        static private int highlight_entity;
        static public void Init()
        {
            id = InternalCalls.GetCurrentEntityID();

            InternalCalls.SetButtonIdleAlpha(id, 1f);
            InternalCalls.SetButtonHoverScale(id, 0.9f);

            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            int monitor_width = InternalCalls.GetMonitorWidth();
            int monitor_height = InternalCalls.GetMonitorHeight();
            string fullscreen_option = monitor_width.ToString() + " x " + monitor_height.ToString();
            current_resolution = fullscreen_option + " (Windowed)";
            if (!options_created)
            {
                resolution_options.Add(fullscreen_option + " (Fullscreen)");
                resolution_options.Add(fullscreen_option + " (Windowed)");
                resolution_options.Add("1600 x 900 (Windowed)");
                resolution_options.Add("1280 x 720 (Windowed)");
                resolution_options.Add("1024 x 768 (Windowed)");
                resolution_options.Add("  800 x 600 (Windowed)");
            }

            dropdown_open = false;
            CreateOptionEntities();

            if (!options_created)
            {
                for (int i = 0; i < option_entities_list.Count; i++)
                {
                    option_entities.Add(option_entities_list[i], resolution_options[i]);
                }
                options_created = true;
            }

            highlight_entity = InternalCalls.CreateEntityUI("Option Highlight", SettingsScript.DROPDOWN_OPTION_HIGHLIGHT);

            DeleteExtraOptionEntities();
            key_list = option_entities.Keys.ToList();
            last_key = key_list[key_list.Count - 1];
            for (int i = 0; i < option_entities.Count; i++)
            {
                position_of_entities.Add(new Vector2D(0, 0));
            }
            last_pos_index = position_of_entities.Count - 1;
            InternalCalls.SetSpriteImageEntity(SettingsScript.DROPDOWN_OPTION_END, last_key);
            Hide();
        }

        static public void Update()
        {
            if (ScrollBarTrackerScript.is_adjusting_scroll)
                dropdown_open = false;

            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);
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
                dropdown_open = !dropdown_open;
                FPSDropdownScript.dropdown_open = false;
                SettingsScript.PlayClickSound();
            }

            x_pos = origin.x + 0.5f * win_dimension.x;
            y_pos = origin.y + (0.1f * win_dimension.y) - ScrollBarTrackerScript.virtual_y;

            //sizes
            const float OPTION_SCALE = 0.177f;
            const float OPTION_AR = 56f / 610f;

            float option_scaled_width = OPTION_SCALE * win_dimension.x;
            float option_scaled_height = OPTION_AR * option_scaled_width;
            float height = InternalCalls.GetTransformScaling().y;

            for (int i = 0; i < key_list.Count; i++)
            {
                int key = key_list[i];
                InternalCalls.TransformSetScaleEntity(option_scaled_width, option_scaled_height, key);
            }
            for (int i = 0; i < key_list.Count; i++)
            {
                position_of_entities[i].x = x_pos;
                position_of_entities[i].y = y_pos - height * .8f - (i * option_scaled_height * .8f);
            }

            Vector2D current_resolution_text_pos = new Vector2D(origin.x + (0.4375f * win_dimension.x),
                origin.y + (0.093f * win_dimension.y) - ScrollBarTrackerScript.virtual_y);
            Vector2D normalized_pos = ConvertAbsoluteToNormalizedPosition(current_resolution_text_pos.x, current_resolution_text_pos.y);
            if (SettingsScript.show_settings && !(current_resolution_text_pos.y > (origin.y + (0.7f * win_dimension.y))
                || current_resolution_text_pos.y < (origin.y + (0.25f * win_dimension.y))))
            {
                InternalCalls.RenderLeftAlignTextFont(current_resolution, SettingsScript.FONT_NAME,
                normalized_pos.x, normalized_pos.y, 7f, (1f, 1f, 1f, 1f));
            }

            if (dropdown_open)
            {
                float yoffset = 0.06f;
                foreach (KeyValuePair<int, string> entry in option_entities)
                {
                    InternalCalls.RenderLeftAlignTextFont(entry.Value, SettingsScript.FONT_NAME,
                        ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x,
                        ConvertAbsoluteToNormalizedPosition(0f, origin.y + (yoffset * win_dimension.y) - ScrollBarTrackerScript.virtual_y).y,
                        7f, (1f, 1f, 1f, 1f));
                    yoffset -= 0.025f;
                }

                foreach (int entity in option_entities_list)
                {
                    if (InternalCalls.CheckMouseIntersectEntity(entity))
                    {
                        SimpleVector2D pos = InternalCalls.GetTransformPositionEntity(entity);
                        SimpleVector2D size = InternalCalls.GetTransformScalingEntity(entity);
                        InternalCalls.TransformSetPositionEntity(pos.x, pos.y, highlight_entity);
                        InternalCalls.TransformSetScaleEntity(.9f * size.x, .9f * size.y, highlight_entity);
                        if (InternalCalls.MousePressed((int)MouseButton.Left))
                        {
                            current_resolution = option_entities[entity];
                            dropdown_open = false;
                        }
                    }
                }

                InternalCalls.SetSpriteImage(SettingsScript.DROPDOWN_EXPANDED);
                Draw();
            }
            else
            {
                InternalCalls.SetSpriteImage(SettingsScript.DROPDOWN_MINIMIZED);
                Hide();
            }

            if (SettingsScript.show_settings)
                InternalCalls.TransformSetPosition(x_pos, y_pos);
            if (!SettingsScript.show_settings || y_pos > (origin.y + (0.7f * win_dimension.y)) || y_pos < (origin.y + (0.25f * win_dimension.y)))
            {
                dropdown_open = false;
                InternalCalls.TransformSetPosition(9999f, 9999f);
            }

        }

        static public void CleanUp()
        {
        }

        static private void Draw()
        {
            for (int i = 0; i < key_list.Count; i++)
            {
                if (i < position_of_entities.Count)
                {
                    int key = key_list[i];
                    InternalCalls.TransformSetPositionEntity(position_of_entities[i].x, position_of_entities[i].y, key);
                }
            }

            //InternalCalls.TransformSetPositionEntity(option_highlight_pos.x, option_highlight_pos.y, dropdown_option_highlight_entity);
            //InternalCalls.TransformSetPositionEntity(option_end_pos.x, option_end_pos.y, dropdown_option_end_entity);
        }

        static private void Hide()
        {
            InternalCalls.TransformSetPosition(9999f, 9999f);
            for (int i = 0; i < key_list.Count; i++)
            {
                if (i < position_of_entities.Count)
                {
                    int key = key_list[i];
                    InternalCalls.TransformSetPositionEntity(9999f, 9999f, key);
                }
            }
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, highlight_entity);
        }

        static private void CreateOptionEntities()
        {
            for (int i = 0; i < resolution_options.Count - 1; i++)
            {
                string option = resolution_options[i];
                int entity = InternalCalls.CreateEntityUIScript(option, SettingsScript.DROPDOWN_OPTION, "DropdownOptionScript");

                option_entities_list.Add(entity);
            }
            int last_index = resolution_options.Count - 1;
            string last_option = resolution_options[last_index];
            int last_entity = InternalCalls.CreateEntityUIScript(last_option, SettingsScript.DROPDOWN_OPTION_END, "DropdownOptionScript");
            option_entities_list.Add(last_entity);
        }
        static private void DeleteExtraOptionEntities()
        {
            for (int i = 0; i < option_entities_list.Count; i++)
            {
                if (!option_entities.ContainsKey(option_entities_list[i]))
                {
                    InternalCalls.DestroyEntity(option_entities_list[i]);
                }
            }
        }

        static public Vector2D ConvertAbsoluteToNormalizedPosition(float x, float y)
        {
            float normalized_x = (x - origin.x) / win_dimension.x;
            float normalized_y = (y - origin.y) / win_dimension.y;

            // Ensure the normalized values are clamped between 0 and 1 to avoid going out of bounds
            normalized_x = Math.Clamp(normalized_x, 0f, 1f);
            normalized_y = Math.Clamp(normalized_y, 0f, 1f);

            return new Vector2D(normalized_x, normalized_y);
        }
    }
}

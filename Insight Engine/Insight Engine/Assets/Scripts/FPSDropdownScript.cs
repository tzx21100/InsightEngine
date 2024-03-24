using System;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace IS
{
    class FPSDropdownScript
    {
        static public bool dropdown_open = false;
        static public float x_pos;
        static public float y_pos;

        // Windows
        static private Vector2D win_dimension = new Vector2D(0, 0);
        static private Vector2D origin = new Vector2D(0, 0);

        // Camera
        static private Vector2D camera_pos = new Vector2D(0, 0);
        static private float camera_zoom = 0f;
        static private int id;
        static private int current_fps;
        static private bool first_hover = false;
        static private bool options_created = false;

        static private List<int> options = new List<int>();
        static private List<int> option_entities_list = new List<int>();
        static private Dictionary<int, int> option_entities = new Dictionary<int, int>();
        static private List<Vector2D> position_of_entities = new List<Vector2D>();
        static private int highlighted_entity;
        static private List<int> key_list = new List<int>();
        static private int last_key;
        static private int last_pos_index;

        static public void Init()
        {
            id = InternalCalls.GetCurrentEntityID();
            current_fps = InternalCalls.GetTargetFPS();

            InternalCalls.SetButtonIdleAlpha(id, 1f);
            InternalCalls.SetButtonHoverScale(id, 0.9f);

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            // Windows
            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);

            if (!options_created)
            {
                options.Add(30);
                options.Add(45);
                options.Add(60);
            }

            dropdown_open = false;
            CreateOptionEntities();

            if (!options_created)
            {
                for (int i = 0; i < option_entities_list.Count; i++)
                {
                    option_entities.Add(option_entities_list[i], options[i]);
                }
                options_created = true;
            }

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
                ResolutionDropdownScript.dropdown_open = false;
                SettingsScript.PlayClickSound();
            }

            x_pos = origin.x + 0.5f * win_dimension.x;
            y_pos = origin.y + (0.215f * win_dimension.y) - ScrollBarTrackerScript.virtual_y;
            //y_pos = origin.y + (0.165f * win_dimension.y) - ScrollBarTrackerScript.virtual_y;

            //sizes
            const float OPTION_SCALE = 0.177f;
            const float OPTION_AR = 56f / 610f;

            float option_scaled_width = OPTION_SCALE * win_dimension.x;
            float option_scaled_height = OPTION_AR * option_scaled_width;

            foreach (int entity in option_entities_list)
            {
                InternalCalls.TransformSetScaleEntity(option_scaled_width, option_scaled_height, entity);
            }
            for (int i = 0; i < key_list.Count - 1; i++)
            {
                position_of_entities[i].x = x_pos;
                position_of_entities[i].y = y_pos - (0.036f * win_dimension.y) - (i * 0.024f * win_dimension.y);
            }

            if (key_list.Count > 0)
            {
                position_of_entities[last_pos_index].x = x_pos;
                position_of_entities[last_pos_index].y = y_pos - (0.033f * win_dimension.y) - (last_pos_index * 0.024f * win_dimension.y);
            }

            if (SettingsScript.show_settings && !(y_pos > (origin.y + (0.7f * win_dimension.y)) || y_pos < (origin.y + (0.25f * win_dimension.y))))
            {
                InternalCalls.RenderLeftAlignTextFont(current_fps.ToString(), SettingsScript.FONT_NAME,
                        ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x,
                        ConvertAbsoluteToNormalizedPosition(0f, origin.y + (0.208f * win_dimension.y) - ScrollBarTrackerScript.virtual_y).y,
                        7f, (1f, 1f, 1f, 1f));
            }

            if (dropdown_open)
            {
                float yoffset = 0.177f;
                foreach (int option in options)
                {
                    InternalCalls.RenderLeftAlignTextFont(option.ToString(), SettingsScript.FONT_NAME,
                        ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x,
                        ConvertAbsoluteToNormalizedPosition(0f, origin.y + (yoffset * win_dimension.y) - ScrollBarTrackerScript.virtual_y).y,
                        7f, (1f, 1f, 1f, 1f));
                    yoffset -= 0.023f;
                }
                foreach (int entity in option_entities_list)
                {
                    if (InternalCalls.CheckMouseIntersectEntity(entity))
                    {
                        if (InternalCalls.MousePressed((int)MouseButton.Left))
                        {
                        }
                        Console.WriteLine("Hovered " + entity);
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
            for (int i = 0; i < option_entities.Keys.Count; i++)
            {
                if (i < position_of_entities.Count)
                {
                    int key = option_entities.Keys.ElementAt(i);
                    InternalCalls.TransformSetPositionEntity(position_of_entities[i].x, position_of_entities[i].y, key);
                }
            }

            //InternalCalls.TransformSetPositionEntity(option_highlight_pos.x, option_highlight_pos.y, dropdown_option_highlight_entity);
            //InternalCalls.TransformSetPositionEntity(option_end_pos.x, option_end_pos.y, dropdown_option_end_entity);
        }

        static private void Hide()
        {
            InternalCalls.TransformSetPosition(9999f, 9999f);
            for (int i = 0; i < option_entities.Keys.Count; i++)
            {
                if (i < position_of_entities.Count)
                {
                    int key = option_entities.Keys.ElementAt(i);
                    InternalCalls.TransformSetPositionEntity(9999f, 9999f, key);
                }
            }
        }
        static private void CreateOptionEntities()
        {
            for (int i = 0; i < options.Count - 1; i++)
            {
                int option = options[i];
                int entity = InternalCalls.CreateEntityUIScript(option.ToString(), SettingsScript.DROPDOWN_OPTION, "DropdownOptionScript");

                option_entities_list.Add(entity);
                Console.WriteLine("Entity " + entity + " created");
            }
            int last_option = options.Last();
            int last_entity = InternalCalls.CreateEntityUIScript(last_option.ToString(), SettingsScript.DROPDOWN_OPTION_END, "DropdownOptionScript");
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

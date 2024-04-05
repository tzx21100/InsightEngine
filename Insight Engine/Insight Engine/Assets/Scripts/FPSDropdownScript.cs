using System;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace IS
{
    class FPSDropdownScript
    {
        // Public access
        static public bool dropdown_open = false;
        static public float x_pos;
        static public float y_pos;

        // Windows
        static private Vector2D win_dimension   = new Vector2D(0, 0);
        static private Vector2D origin          = new Vector2D(0, 0);

        // Camera
        static private Vector2D camera_pos  = new Vector2D(0, 0);
        static private float camera_zoom    = 0f;

        // Flags
        static private bool first_hover     = false;
        static private bool options_created = false;

        // Identification
        static private int current_fps;
        static private int highlight_entity;
        static private int selected_entity;

        // Containers
        static private List<int> options                    = new List<int>();
        static private List<int> option_entities_list       = new List<int>();
        static private Dictionary<int, int> option_entities = new Dictionary<int, int>();
        static private List<Vector2D> position_of_entities  = new List<Vector2D>();

        static public void Init()
        {
            current_fps = InternalCalls.GetTargetFPS();

            int dropdown_list_entity = InternalCalls.GetCurrentEntityID();
            InternalCalls.SetButtonIdleAlpha(dropdown_list_entity, 1f);
            InternalCalls.SetButtonHoverScale(dropdown_list_entity, 0.9f);

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

            //highlight_entity = InternalCalls.CreateEntityUI("Option Highlight", SettingsScript.DROPDOWN_OPTION_HIGHLIGHT);
            //selected_entity = InternalCalls.CreateEntityUI("Option Selected", SettingsScript.DROPDOWN_OPTION_HIGHLIGHT);
            InternalCalls.SetSpriteAlphaEntity(.5f, selected_entity);

            DeleteExtraOptionEntities();
            for (int i = 0; i < option_entities.Count; i++)
            {
                position_of_entities.Add(new Vector2D(0, 0));
            }
            //InternalCalls.SetSpriteImageEntity(SettingsScript.DROPDOWN_OPTION_END, option_entities_list.Last());
            Hide();

            //sizes
            const float OPTION_AR = 56f / 610f;

            float width = InternalCalls.GetTransformScaling().x;
            float option_scaled_height = OPTION_AR * width;

            foreach (int entity in option_entities_list)
            {
                InternalCalls.TransformSetScaleEntity(width, option_scaled_height, entity);
            }
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

            UpdateOptions();
            RenderOptions();
            RenderCurrentOptionText();

            if (SettingsScript.show_settings)
                InternalCalls.TransformSetPosition(x_pos, y_pos);
            if (!SettingsScript.show_settings || y_pos > (origin.y + (0.7f * win_dimension.y)) || y_pos < (origin.y + (0.25f * win_dimension.y)))
            {
                dropdown_open = false;
                InternalCalls.TransformSetPosition(9999f, 9999f);
            }
        }

        static public void CleanUp()
        {}

        static private void RenderCurrentOptionText()
        {
            if (SettingsScript.show_settings && !(y_pos > (origin.y + (0.7f * win_dimension.y)) || y_pos < (origin.y + (0.25f * win_dimension.y))))
            {
                InternalCalls.RenderLeftAlignTextFont(current_fps.ToString(), SettingsScript.BODY_FONT_NAME,
                        ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x,
                        ConvertAbsoluteToNormalizedPosition(0f, origin.y + (0.208f * win_dimension.y) - ScrollBarTrackerScript.virtual_y).y,
                        7f, (1f, 1f, 1f, 1f));
            }
        }

        static private void UpdateOptions()
        {
            const float OPTION_AR = 56f / 610f;

            SimpleVector2D dropdown_list_size = InternalCalls.GetTransformScaling();
            float option_scaled_height = OPTION_AR * dropdown_list_size.x;

            for (int i = 0; i < option_entities_list.Count; i++)
            {
                position_of_entities[i].Set(x_pos, y_pos - dropdown_list_size.y * .8f - (i * option_scaled_height));
                InternalCalls.TransformSetScaleEntity(dropdown_list_size.x, option_scaled_height, option_entities_list[i]);
            }
        }

        static private void RenderOptions()
        {
            if (dropdown_open)
            {
                const float INITIAL_YOFFSET = 0.174f;
                const float YOFFSET = 0.028f;
                for (int i = 0; i < option_entities.Count; ++i)
                {
                    Vector2D position = ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x),
                        origin.y + (INITIAL_YOFFSET * win_dimension.y) - ScrollBarTrackerScript.virtual_y - i * YOFFSET * win_dimension.y);

                    InternalCalls.RenderLeftAlignTextFont(options[i].ToString(), SettingsScript.BODY_FONT_NAME,
                        position.x, position.y, 7f, (1f, 1f, 1f, 1f));
                }

                foreach (int entity in option_entities_list)
                {
                    SimpleVector2D pos = InternalCalls.GetTransformPositionEntity(entity);
                    SimpleVector2D size = InternalCalls.GetTransformScalingEntity(entity);

                    if (current_fps == option_entities[entity])
                    {
                        InternalCalls.TransformSetPositionEntity(pos.x, pos.y, selected_entity);
                        InternalCalls.TransformSetScaleEntity(size.x, .9f * size.y, selected_entity);
                    }

                    if (InternalCalls.CheckMouseIntersectEntity(entity))
                    {
                        InternalCalls.TransformSetPositionEntity(pos.x, pos.y, highlight_entity);
                        InternalCalls.TransformSetScaleEntity(size.x, .9f * size.y, highlight_entity);
                        if (InternalCalls.MousePressed((int)MouseButton.Left))
                        {
                            int fps = option_entities[entity];
                            InternalCalls.SetTargetFPS(fps);
                            current_fps = fps;
                            dropdown_open = false;
                            SettingsScript.PlayClickSound();
                        }
                    }
                }
                //InternalCalls.SetSpriteImage(SettingsScript.DROPDOWN_EXPANDED);
                Draw();
            }
            else
            {
                //InternalCalls.SetSpriteImage(SettingsScript.DROPDOWN_MINIMIZED);
                Hide();
            }
        }

        static private void Draw()
        {
            for (int i = 0; i < option_entities_list.Count; i++)
                InternalCalls.TransformSetPositionEntity(position_of_entities[i].x, position_of_entities[i].y, option_entities_list[i]);
        }

        static private void Hide()
        {
            for (int i = 0; i < option_entities_list.Count; i++)
                InternalCalls.TransformSetPositionEntity(9999f, 9999f, option_entities_list[i]);

            InternalCalls.TransformSetPositionEntity(9999f, 9999f, highlight_entity);
            InternalCalls.TransformSetPositionEntity(9999f, 9999f, selected_entity);
        }
        static private void CreateOptionEntities()
        {
            for (int i = 0; i < options.Count - 1; i++)
            {
                int option = options[i];
                //int entity = InternalCalls.CreateEntityUI(option.ToString(), SettingsScript.DROPDOWN_OPTION);

                //option_entities_list.Add(entity);
            }

            //int last_entity = InternalCalls.CreateEntityUI(options.Last().ToString(), SettingsScript.DROPDOWN_OPTION_END);
            //option_entities_list.Add(last_entity);
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

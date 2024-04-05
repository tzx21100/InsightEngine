using System;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace IS
{
    class ResolutionDropdownScript
    {
        // Public access
        static public bool dropdown_open = false;
        static public float x_pos;
        static public float y_pos;

        // Windows
        static private Vector2D win_dimension    = new Vector2D(0, 0);
        static private Vector2D origin           = new Vector2D(0, 0);

        // Camera
        static private Vector2D camera_pos  = new Vector2D(0, 0);
        static private float camera_zoom    = 0f;

        // Flags
        static private bool first_hover     = false;
        static private bool options_created = false;

        // Identification
        static private Resolution current_resolution;
        static private int highlight_entity;
        static private int selected_entity;

        // Containers
        static private List<Resolution> options                     = new List<Resolution>();
        static private List<int> option_entities_list               = new List<int>();
        static private Dictionary<int, Resolution> option_entities  = new Dictionary<int, Resolution>();
        static private List<Vector2D> position_of_entities          = new List<Vector2D>();

        static public void Init()
        {
            int monitor_width = InternalCalls.GetMonitorWidth();
            int monitor_height = InternalCalls.GetMonitorHeight();
            current_resolution = new Resolution(monitor_width, monitor_height, "Windowed");

            int dropdown_list_entity = InternalCalls.GetCurrentEntityID();
            InternalCalls.SetButtonIdleAlpha(dropdown_list_entity, 1f);
            InternalCalls.SetButtonHoverScale(dropdown_list_entity, 0.9f);

            camera_zoom = InternalCalls.CameraGetZoom();

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
                options.Add(new Resolution(monitor_width, monitor_height, "Fullscreen"));
                options.Add(new Resolution(monitor_width, monitor_height, "Borderless"));
                options.Add(new Resolution(monitor_width, monitor_height, "Windowed"));
                options.Add(new Resolution(1600, 900, "Windowed"));
                options.Add(new Resolution(1280, 720, "Windowed"));
                options.Add(new Resolution(1024, 768, "Windowed"));
                options.Add(new Resolution(800, 600, "Windowed"));
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

        public struct Resolution
        {
            public Resolution(int width, int height, string postfix = "")
            {
                mWidth = width; mHeight = height; mPostfix = postfix;
            }

            public override string ToString()
            {
                return $"{mWidth} x {mHeight} {mPostfix}";
            }

            public override bool Equals(object obj)
            {
                if (obj is Resolution other)
                {
                    return Equals(other);
                }
                return false;
            }

            public bool Equals(Resolution other)
            {
                return mWidth == other.mWidth && mHeight == other.mHeight && mPostfix == other.mPostfix;
            }

            public override int GetHashCode()
            {
                return HashCode.Combine(mWidth, mHeight, mPostfix);
            }

            public static bool operator ==(Resolution left, Resolution right)
            {
                return left.Equals(right);
            }

            public static bool operator !=(Resolution left, Resolution right)
            {
                return !left.Equals(right);
            }

            private int mWidth;
            private int mHeight;
            private string mPostfix;
        }

        static private void RenderCurrentOptionText()
        {
            Vector2D current_resolution_text_pos = new Vector2D(origin.x + (0.4375f * win_dimension.x),
                origin.y + (0.093f * win_dimension.y) - ScrollBarTrackerScript.virtual_y);
            Vector2D normalized_pos = ConvertAbsoluteToNormalizedPosition(current_resolution_text_pos.x, current_resolution_text_pos.y);
            if (SettingsScript.show_settings && !(current_resolution_text_pos.y + (0.028f * win_dimension.y) > (origin.y + (0.7f * win_dimension.y))
                || current_resolution_text_pos.y - (0.028f * win_dimension.y) < (origin.y + (0.25f * win_dimension.y))))
            {
                InternalCalls.RenderLeftAlignTextFont(current_resolution.ToString(), SettingsScript.BODY_FONT_NAME,
                normalized_pos.x, normalized_pos.y, 7f, (1f, 1f, 1f, 1f));
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
                const float INITIAL_YOFFSET = 0.06f;
                const float YOFFSET         = 0.028f;
                for (int i = 0; i < options.Count; ++i)
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

                    // Set current selection
                    if (current_resolution == option_entities[entity])
                    {
                        InternalCalls.TransformSetPositionEntity(pos.x, pos.y, selected_entity);
                        InternalCalls.TransformSetScaleEntity(size.x, .9f * size.y, selected_entity);
                    }

                    // Hover highlight
                    if (InternalCalls.CheckMouseIntersectEntity(entity))
                    {
                        InternalCalls.TransformSetPositionEntity(pos.x, pos.y, highlight_entity);
                        InternalCalls.TransformSetScaleEntity(size.x, .9f * size.y, highlight_entity);
                        if (InternalCalls.MousePressed((int)MouseButton.Left))
                        {
                            current_resolution = option_entities[entity];
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
                string option = options[i].ToString();
                //int entity = InternalCalls.CreateEntityUI(option, SettingsScript.DROPDOWN_OPTION);

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

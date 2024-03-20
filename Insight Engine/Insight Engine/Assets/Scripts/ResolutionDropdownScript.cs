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
        static public bool option_highlights_created = false;

        static SimpleImage dropdown_minimized_image;
        static SimpleImage dropdown_open_image;
        static SimpleImage dropdown_option_image;
        static SimpleImage dropdown_option_highlight_image;
        static SimpleImage dropdown_option_end_image;

        static public List<string> resolution_options = new List<string>();
        static public Dictionary<int, string> option_entities = new Dictionary<int, string>();
        static public List<int> option_entities_list = new List<int>();
        static public List<Vector2D> position_of_entities = new List<Vector2D>();
        static public List<int> key_list = new List<int>();
        static public int last_key;
        static private int last_pos_index;

        static public Dictionary<int, int> option_highlight_entities = new Dictionary<int, int>();
        static public List<int> option_highlight_entities_list = new List<int>();
        static public void Init()
        {
            id = InternalCalls.GetCurrentEntityID();

            dropdown_minimized_image = InternalCalls.GetSpriteImage("dropdown_minimized.png");
            dropdown_open_image = InternalCalls.GetSpriteImage("dropdown_open.png");
            dropdown_option_image = InternalCalls.GetSpriteImage("dropdown_option.png");
            dropdown_option_highlight_image = InternalCalls.GetSpriteImage("dropdown_option_highlight.png");
            dropdown_option_end_image = InternalCalls.GetSpriteImage("dropdown_option_end.png");

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

            resolution_options.Add("800 x 600");
            resolution_options.Add("1024 x 768");
            resolution_options.Add("1280 x 720");
            resolution_options.Add("1600 x 900");
            resolution_options.Add("1920 x 1080");

            dropdown_open = false;
            CreateOptionEntities();
            CreateOptionHighlightEntities();

            if (!options_created)
            {
                for (int i = 0; i < option_entities_list.Count; i++)
                {
                    option_entities.Add(option_entities_list[i], resolution_options[i]);
                }
                options_created = true;
            }

            if (!option_highlights_created)
            {
                for (int i = 0; i < option_highlight_entities_list.Count; i++)
                {
                    option_highlight_entities.Add(option_highlight_entities_list[i], option_entities_list[i]);
                }
                option_highlights_created = true;
            }

            DeleteExtraOptionEntities();
            DeleteExtraOptionHighlightEntities();
            key_list = option_entities.Keys.ToList();
            last_key = key_list[key_list.Count - 1];
            for (int i = 0; i < option_entities.Count; i++)
            {
                position_of_entities.Add(new Vector2D(0, 0));
            }
            last_pos_index = position_of_entities.Count - 1;
            InternalCalls.SetSpriteImageEntity(dropdown_option_end_image, last_key);

            /*            dropdown_minimized_image = InternalCalls.GetSpriteImage("dropdown_minimized.png");
                        dropdown_open_image = InternalCalls.GetSpriteImage("dropdown_open.png");
                        dropdown_option_image = InternalCalls.GetSpriteImage("dropdown_option.png");
                        dropdown_option_highlight_image = InternalCalls.GetSpriteImage("dropdown_option_highlight.png");
                        dropdown_option_end_image = InternalCalls.GetSpriteImage("dropdown_option_end.png");

                        //dropdown_option_entity = InternalCalls.CreateEntityUI("Option", dropdown_option_image);

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
                        CreateOptionHighlightEntities();

                        if (!options_created)
                        {
                            for (int i = 0; i < option_entities_list.Count; i++)
                            {
                                option_entities.Add(option_entities_list[i], resolution_options[i]);
                            }
                            options_created = true;
                        }

                        if (!option_highlights_created)
                        {
                            for (int i = 0; i < option_highlight_entities_list.Count; i++)
                            {
                                option_highlight_entities.Add(option_highlight_entities_list[i], option_entities_list[i]);
                            }
                            option_highlights_created = true;
                        }

                        DeleteExtraOptionEntities();
                        DeleteExtraOptionHighlightEntities();
                        key_list = option_entities.Keys.ToList();
                        last_key = key_list[key_list.Count - 1];
                        for (int i = 0; i < option_entities.Count; i++)
                        {
                            position_of_entities.Add(new Vector2D(0, 0));
                        }
                        last_pos_index = position_of_entities.Count - 1;
                        InternalCalls.SetSpriteImageEntity(dropdown_option_end_image, last_key);*/

        }

        static public void Update()
        {
            /*  camera_zoom = InternalCalls.CameraGetZoom();

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
              Vector2D option_end = new Vector2D(0.1591f * win_dimension.x, 0.018f * win_dimension.x); //651 x 63
              //scale
              for (int i = 0; i < key_list.Count - 1; i++)
              {
                  int key = key_list[i];
                  InternalCalls.TransformSetScaleEntity(option.x, option.y, key);
              }
              if (key_list.Count > 0)
              {
                  InternalCalls.TransformSetScaleEntity(option_end.x, option_end.y, last_key);
              }
              for (int i = 0; i < key_list.Count - 1; i++)
              {
                  position_of_entities[i].x = x_pos - (0.0001f * win_dimension.x);
                  position_of_entities[i].y = y_pos - (0.033f * win_dimension.y) - (i * 0.024f * win_dimension.y);
                  position_of_entities[i].Set(position_of_entities[i].x, position_of_entities[i].y);
              }

              if (key_list.Count > 0)
              {
                  position_of_entities[last_pos_index].x = x_pos - (0.0001f * win_dimension.x);
                  position_of_entities[last_pos_index].y = y_pos - (0.033f * win_dimension.y) - (last_pos_index * 0.024f * win_dimension.y);
                  position_of_entities[last_pos_index].Set(position_of_entities[last_pos_index].x, position_of_entities[last_pos_index].y);
              }
              if (dropdown_open)
              {
                  //foreach (KeyValuePair<int, string> entry in option_entities)
                  {
                      InternalCalls.RenderLeftAlignTextFont("800x600", "Semplicita_Light", ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x, ConvertAbsoluteToNormalizedPosition(0f, origin.y + (0.177f *win_dimension.y) - ScrollBarTrackerScript.virtual_y).y, 7f, (1f, 1f, 1f, 1f));
                      InternalCalls.RenderLeftAlignTextFont("800x600", "Semplicita_Light", ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x, ConvertAbsoluteToNormalizedPosition(0f, origin.y + (0.153f * win_dimension.y) - ScrollBarTrackerScript.virtual_y).y, 7f, (1f, 1f, 1f, 1f));
                      InternalCalls.RenderLeftAlignTextFont("800x600", "Semplicita_Light", ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x, ConvertAbsoluteToNormalizedPosition(0f, origin.y + (0.129f * win_dimension.y) - ScrollBarTrackerScript.virtual_y).y, 7f, (1f, 1f, 1f, 1f));
                      InternalCalls.RenderLeftAlignTextFont("800x600", "Semplicita_Light", ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x, ConvertAbsoluteToNormalizedPosition(0f, origin.y + (0.105f * win_dimension.y) - ScrollBarTrackerScript.virtual_y).y, 7f, (1f, 1f, 1f, 1f));
                      InternalCalls.RenderLeftAlignTextFont("800x600", "Semplicita_Light", ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x, ConvertAbsoluteToNormalizedPosition(0f, origin.y + (0.081f * win_dimension.y) - ScrollBarTrackerScript.virtual_y).y, 7f, (1f, 1f, 1f, 1f));
                  }
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
  */
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
                SettingsScript.PlayClickSound();
            }

            x_pos = origin.x + (dropdown_open ? 0.5005f : 0.5f) * win_dimension.x;
            y_pos = origin.y + (0.215f * win_dimension.y) - ScrollBarTrackerScript.virtual_y;

            //sizes
            Vector2D option = new Vector2D(0.1591f * win_dimension.x, 0.015f * win_dimension.x); //651 x 57
            Vector2D option_end = new Vector2D(0.1591f * win_dimension.x, 0.018f * win_dimension.x); //651 x 63
                                                                                                     //scale
            for (int i = 0; i < key_list.Count - 1; i++)
            {
                int key = key_list[i];
                InternalCalls.TransformSetScaleEntity(option.x, option.y, key);
            }
            if (key_list.Count > 0)
            {
                InternalCalls.TransformSetScaleEntity(option_end.x, option_end.y, last_key);
            }
            for (int i = 0; i < key_list.Count - 1; i++)
            {
                position_of_entities[i].x = x_pos - (0.0001f * win_dimension.x);
                position_of_entities[i].y = y_pos - (0.033f * win_dimension.y) - (i * 0.024f * win_dimension.y);
                position_of_entities[i].Set(position_of_entities[i].x, position_of_entities[i].y);
            }

            if (key_list.Count > 0)
            {
                position_of_entities[last_pos_index].x = x_pos - (0.0001f * win_dimension.x);
                position_of_entities[last_pos_index].y = y_pos - (0.033f * win_dimension.y) - (last_pos_index * 0.024f * win_dimension.y);
                position_of_entities[last_pos_index].Set(position_of_entities[last_pos_index].x, position_of_entities[last_pos_index].y);
            }

            if (dropdown_open)
            {
                float yoffset = 0.177f;
                foreach (KeyValuePair<int, string> entry in option_entities)
                {
                    InternalCalls.RenderLeftAlignTextFont(entry.Value, SettingsScript.FONT_NAME,
                        ConvertAbsoluteToNormalizedPosition(origin.x + (0.4375f * win_dimension.x), 0f).x,
                        ConvertAbsoluteToNormalizedPosition(0f, origin.y + (yoffset * win_dimension.y) - ScrollBarTrackerScript.virtual_y).y,
                        7f, (1f, 1f, 1f, 1f));
                    yoffset -= 0.023f;
                }
                InternalCalls.SetSpriteImage(dropdown_open_image);
                Draw();
            }
            else
            {
                InternalCalls.SetSpriteImage(dropdown_minimized_image);
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
            //InternalCalls.TransformSetPositionEntity(9999f, 9999f, dropdown_option_entity);
            for (int i = 0; i < key_list.Count; i++)
            {
                if (i < position_of_entities.Count)
                {
                    int key = key_list[i];
                    InternalCalls.TransformSetPositionEntity(9999f, 9999f, key);
                }
            }
        }

        static private void CreateOptionEntities()
        {
            for (int i = 0; i < resolution_options.Count - 1; i++)
            {
                string option = resolution_options[i];
                int entity = InternalCalls.CreateEntityUIScript(option, dropdown_option_image, "DropdownOptionScript");

                option_entities_list.Add(entity);
            }
            int last_index = resolution_options.Count - 1;
            string last_option = resolution_options[last_index];
            int last_entity = InternalCalls.CreateEntityUIScript(last_option, dropdown_option_end_image, "DropdownOptionScript");
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

        static private void CreateOptionHighlightEntities()
        {
            for (int i = 0; i < resolution_options.Count; i++)
            {
                int entity = InternalCalls.CreateEntityUI("Option Highlight", dropdown_option_highlight_image);
                option_highlight_entities_list.Add(entity);
            }
        }

        static private void DeleteExtraOptionHighlightEntities()
        {
            foreach (var entity in option_highlight_entities_list)
            {
                if (!option_highlight_entities.ContainsKey(entity))
                {
                    InternalCalls.DestroyEntity(entity);
                    //Console.WriteLine("deleted {0}",entity);
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

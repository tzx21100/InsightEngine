using System;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace IS
{
    class FPSDropdownScript
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
        static private int current_fps;
        static public bool first_hover = false;

        static public bool dropdown_open = false;
        static public bool options_created = false;

        static public SimpleImage dropdown_minimized_image;
        static public SimpleImage dropdown_open_image;
        static public SimpleImage dropdown_option_image;
        static public SimpleImage dropdown_option_highlight_image;
        static public SimpleImage dropdown_option_end_image;

        static public List<int> options = new List<int>();
        static public List<int> option_entities_list = new List<int>();
        static public Dictionary<int, int> option_entities = new Dictionary<int, int>();
        static public List<Vector2D> position_of_entities = new List<Vector2D>();
        static public int highlighted_entity;

        static public void Init()
        {
            id = InternalCalls.GetCurrentEntityID();
            current_fps = InternalCalls.GetTargetFPS();

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

            options.Add(30);
            options.Add(60);
            options.Add(90);
            options.Add(120);
            options.Add(144);

            dropdown_open = false;
            CreateOptionEntities();

            if (!options_created)
            {
                for (int i = 0; i < option_entities.Count; i++)
                {
                    option_entities.Add(option_entities_list[i], options[i]);
                    if (options[i] == current_fps)
                        highlighted_entity = option_entities_list[i];
                }
                options_created = true;
            }

            DeleteExtraOptionEntities();
            //key_list = option_entities.Keys.ToList();
            //last_key = key_list[key_list.Count - 1];
            //for (int i = 0; i < option_entities.Count; i++)
            //{
            //    position_of_entities.Add(new Vector2D(0, 0));
            //}
            //last_pos_index = position_of_entities.Count - 1;
            //InternalCalls.SetSpriteImageEntity(dropdown_option_end_image, last_key);
        }

        static public void Update()
        {
            
        }
        
        static public void CleanUp()
        {

        }
        static private void CreateOptionEntities()
        {
            for (int i = 0; i < options.Count - 1; i++)
            {
                int option = options[i];
                int entity = InternalCalls.CreateEntityUIScript(option.ToString(), dropdown_option_image, "DropdownOptionScript");

                option_entities_list.Add(entity);
            }
            int last_option = options.Last();
            int last_entity = InternalCalls.CreateEntityUIScript(last_option.ToString(), dropdown_option_end_image, "DropdownOptionScript");
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

    }
}

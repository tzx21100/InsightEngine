using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System;
using System.Linq;

namespace IS
{
    class DropdownOptionScript
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
        static public bool option_highlights_created = false;

        //static private int dropdown_entity;
        //static private int dropdown_option_entity;
        //static private int dropdown_option_highlight_entity;

        static public Dictionary<int, int> option_highlight_entities = new Dictionary<int, int>();
        static public List<int> option_highlight_entities_list = new List<int>();
        static public List<Vector2D> position_of_entities = new List<Vector2D>();
        static public List<int> key_list = new List<int>();
        
        static public void Init()
        {
            //dropdown_option_highlight_image = InternalCalls.GetSpriteImage("dropdown_option_highlight.png");

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


            InternalCalls.ButtonComponentAdd(id, 1f, 1f, 1f, 1f, 1f);

            //preservation of option button sizes scale
            //sizes
            Vector2D option = new Vector2D(0.1591f * win_dimension.x, 0.015f * win_dimension.x); //651 x 57
            Vector2D option_end = new Vector2D(0.1591f * win_dimension.x, 0.018f * win_dimension.x); //651 x 63
            //scale
            for (int i = 0; i < ResolutionDropdownScript.key_list.Count - 1; i++)
            {
                int key = ResolutionDropdownScript.key_list[i];
                InternalCalls.TransformSetScaleEntity(option.x, option.y, key);
            }
            if (ResolutionDropdownScript.key_list.Count > 0)
            {
                InternalCalls.TransformSetScaleEntity(option_end.x, option_end.y, ResolutionDropdownScript.last_key);
            }

            DeleteExtraOptionEntities();
            //CreateOptionHighlightEntities();
            //if (!option_highlights_created)
            //{
            //    for (int i = 0; i < option_highlight_entities_list.Count; i++)
            //    {
            //        option_highlight_entities.Add(id, option_highlight_entities_list[i]);
            //    }
            //    option_highlights_created = true;
            //}

            //DeleteExtraOptionHighlightEntities();
            //Console.WriteLine(option_highlight_entities[id]);

            //foreach (KeyValuePair<int, string> entry in ResolutionDropdownScript.option_entities)
            //{
            //    Console.WriteLine("Key: {0}, Value: {1}", entry.Key, entry.Value);
            //}

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
            //Vector2D option_highlight = new Vector2D(0.1591f * win_dimension.x, 0.031f * win_dimension.x); //651 x 140 
            //InternalCalls.TransformSetScaleEntity(option_highlight.x, option_highlight.y, dropdown_option_highlight_entity);
            //dropdown_option_highlight_position.x = origin.x + (0.5f * win_dimension.x);
            //dropdown_option_highlight_position.y = origin.y + (0.5f * win_dimension.y);
            //dropdown_option_highlight_position.Set(dropdown_option_highlight_position.x, dropdown_option_highlight_position.y);
            /*//hovered
            if (InternalCalls.GetButtonState() == 1)
            {
                //hovering
                if (!first_hover)
                {
                    InternalCalls.AudioPlaySound("Footsteps_Dirt-Gravel-Far-Small_1.wav", false, 0.15f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
                    first_hover = true;
                }
                InternalCalls.TransformSetPositionEntity(dropdown_option_highlight_position.x, dropdown_option_highlight_position.y, dropdown_option_highlight_entity); //why this only gets last??
            }
            else
            {
                InternalCalls.TransformSetPositionEntity(9999f, 9999f, dropdown_option_highlight_entity);

                first_hover = false;
            }*/
            if (!InternalCalls.IsWindowFocused())
            {
                first_hover = true;
            }
            // clicking
            if (InternalCalls.GetButtonState() == 2)
            {
                InternalCalls.AudioPlaySound("QubieSFX3.wav", false, 0.4f * SettingsScript.master_multiplier * SettingsScript.vfx_multiplier);
            }


        }

        static public void CleanUp()
        {
        }

        static private void DeleteExtraOptionEntities()
        {
            for (int i = 0; i < ResolutionDropdownScript.option_entities_list.Count; i++)
            {
                if (!ResolutionDropdownScript.option_entities.ContainsKey(ResolutionDropdownScript.option_entities_list[i]))
                {
                    InternalCalls.DestroyEntity(ResolutionDropdownScript.option_entities_list[i]);
                }
            }
        }

        //static private void CreateOptionHighlightEntities()
        //{
        //    //for (int i =0; i < ResolutionDropdownScript.resolution_options.Count; i++)
        //    //{
        //        int entity = InternalCalls.CreateEntityUI("Option Highlight", dropdown_option_highlight_image);
        //        option_highlight_entities_list.Add(entity);
        //    //}
        //}

        //static private void DeleteExtraOptionHighlightEntities()
        //{
        //    for (int i = 0; i < option_highlight_entities_list.Count; i++)
        //    {
        //        if (!option_highlight_entities.ContainsKey(id))
        //        {
        //            InternalCalls.DestroyEntity(option_highlight_entities_list[i]);
        //        }
        //    }
        //}
    }
}

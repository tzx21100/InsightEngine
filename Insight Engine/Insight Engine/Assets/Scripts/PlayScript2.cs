/*!
 * \file PlayerScript.cs
 * \author  Wu Zekai, zekai.wu@digipen.edu
 * \par Course: CSD2401
 * \date 26-11-2023
 * \brief
 * This source file is meant for the player movement and camera.
 *
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

using System.Numerics;
using System;
using System.Runtime.CompilerServices;
using System.ComponentModel;
using System.Diagnostics;
using System.Reflection.Metadata;

namespace IS
{
    public class PlayerScript2
    {
        //private entity
        static SimpleImage player_walk;
        static SimpleImage player_idle;
        /*static SimpleImage player_climb;
        static SimpleImage player_transparent;
        static SimpleImage player_land;
        static SimpleImage player_jump;
        static SimpleImage player_jump_vfx;
        static SimpleImage player_death_vfx;
        static SimpleImage player_powerup_vfx;
        static SimpleImage player_reward_dash;
        static SimpleImage player_reward_doublejump;
        static SimpleImage player_reward_wallclimb;*/

        //psuedo animations for images
        static private float animation_speed = 0.07f;
        static private float animation_speed_set = 0.07f;
        static private int animation_current_frame = 0;

        static private int land_entity;
        static private int jump_entity;
        static private int death_entity;
        static private int powerup_entity;

        static public int PLAYER_ID;

        //movement
        static private float acceleration = 0f;
        static private float acceleration_base = 50f;
        static private float acceleration_increment = 10f;
        static private float max_acceleration = 100;
        static private float max_speed = 700f;
        static private float move_speed = 0f;

        static private int hori_movement;

        // player pos and dimension
        static public Vector2D player_pos = new Vector2D(0, 0);
        static private Vector2D trans_scaling = new Vector2D(0, 0);

        //camera pos
        static public Vector2D camera_pos = new Vector2D(0, 0);
        static private Vector2D target_pos = new Vector2D(0, 0);
        //static float camera_zoom = 0f;

        // Windows
        static Vector2D windows = new Vector2D(0, 0);
        static Vector2D orgin = new Vector2D(0, 0);

        static public void Init()
        {

            // Init Image
            player_walk = InternalCalls.GetSpriteImage("run_strip.png");
            player_idle = InternalCalls.GetSpriteImage("idle_strip.png");
            /*player_jump = InternalCalls.GetSpriteImage("jump_strip.png");
            player_climb = InternalCalls.GetSpriteImage("wall_climb_anim 4R3C.png");
            player_transparent = InternalCalls.GetSpriteImage("transparent.png");
            player_land = InternalCalls.GetSpriteImage("land_vfx 2R7C.png");
            player_jump_vfx = InternalCalls.GetSpriteImage("woosh_vfx 3R4C.png");
            player_death_vfx = InternalCalls.GetSpriteImage("Death Vfx R1 C11.png");
            player_powerup_vfx = InternalCalls.GetSpriteImage("collect_fragment_vfx 2R6C.png");

            player_reward_dash = InternalCalls.GetSpriteImage("Dash UI.png");
            player_reward_doublejump = InternalCalls.GetSpriteImage("Double Jump UI.png");
            player_reward_wallclimb = InternalCalls.GetSpriteImage("Wall Climb UI.png");*/


        }

        static public void Update()
        {

            //float horizontalInput = Input.GetAxis("Horizontal");
            hori_movement = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.D)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.A));
            //float horizontalInput = Input.GetAxis("Horizontal");
            Console.WriteLine("testing");
            Vector2D movement = new Vector2D(hori_movement, 0f);
            //movement.Normalize(); // Ensure diagonal movement isn't faster

            //transform.Translate(movement * 5 * Time.deltaTime);
            InternalCalls.RigidBodySetForce(movement.x * 5, movement.y);
        }

        static public void CleanUp()
        {
            // Empty for now
        }

        public static int BoolToInt(bool boolValue)
        {
            return boolValue ? 1 : 0;
        }
    } //player script

}


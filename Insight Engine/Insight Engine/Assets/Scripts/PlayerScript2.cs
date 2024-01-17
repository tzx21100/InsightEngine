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
using System.Collections.Generic;

namespace IS
{
    public class PlayerScript2
    {
        // death timer
        static private float respawn_timer = 0.6f;
        static private float respawn_timer_set = 0.6f;
        static public bool isDead = false;
        static public bool initialDeath = false;

        static private float camera_shake_duration = 0.2f;
        static private float camera_shake_duration_set = 0.2f;
        static private float camera_shake_timer = 0.02f;
        static private float camera_shake_set = 0.02f;
        static private Vector2D camera_shake_dir = new Vector2D(0, 0);
        static private float camera_shake_angle = 0f;

        // save point information
        static public int health = 3;
        static public int max_health = 3;

        static public float respawn_x = 0f;
        static public float respawn_y = 0f;

        static public int SAVE_POINT_ID = 0;

        //private entity image
        static SimpleImage player_walk;
        static SimpleImage player_idle;
        static SimpleImage player_climb;
        static SimpleImage player_transparent;
        static SimpleImage player_land;
        static SimpleImage player_jump;
        static SimpleImage player_jump_vfx;
        static SimpleImage player_death_vfx;
        static SimpleImage player_powerup_vfx;
        static SimpleImage player_reward_dash;
        static SimpleImage player_reward_doublejump;
        static SimpleImage player_reward_wallclimb;

        //psuedo animations for images
        static private float animation_speed = 0.07f;
        static private float animation_speed_set = 0.07f;
        static private int animation_current_frame = 0;

        static private int land_entity;
        static private int jump_entity;
        static private int death_entity;
        static private int powerup_entity;

        static public int player_id;
        static private float player_width;
        static private float player_height;

        static public bool isGrounded = false;

        //double jump and above
        static private int jump_amount;
        static private int jump_amount_set = 1;
        static private float jump_timer = 0.2f;
        static private float jump_timer_set = 0.2f;
        static private bool isJumping = false;
        static private bool initial_land = false;
        static private float jump_height = 1000f;
        static private float fall_multiplier = 2f;
        static private bool DoubleJumpEnable = false;
        static private int max_jumps = 2;
        static private int jumps_left;

        //dashing 
        static public float bullet_time_timer = 1f;
        static private float bullet_time_duration = 1f;
        static private float dash_timer = 0.2f;
        static private float dash_duration = 0.2f;
        static private bool canDash = true;
        static private bool initialDash = true;
        static public bool isDashing = false;

        static private float dashSpeed = 2000f;

        static private Vector2D dash_dir = new Vector2D(0, 0);//dash dir

        static public int entity_feet;

        //movement
        static private float acceleration = 0f;
        static private float acceleration_base = 50f;
        static private float acceleration_increment = 10f;
        static private float max_acceleration = 100;
        static private float max_speed = 800f;
        static private float move_speed = 100f;

        static private int move_input;
        static private float gravity_scale;

        // player pos and dimension
        static public Vector2D player_pos = new Vector2D(0, 0);
        static public Vector2D player_vel = new Vector2D(0, 0);
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
            player_id = InternalCalls.GetCurrentEntityID();

            // Init Image
            player_walk = InternalCalls.GetSpriteImage("run_strip.png");
            player_idle = InternalCalls.GetSpriteImage("idle_strip.png");
            player_jump = InternalCalls.GetSpriteImage("jump_strip.png");
            player_climb = InternalCalls.GetSpriteImage("wall_climb_anim 4R3C.png");
            player_transparent = InternalCalls.GetSpriteImage("transparent.png");
            player_land = InternalCalls.GetSpriteImage("land_vfx 2R7C.png");
            player_jump_vfx = InternalCalls.GetSpriteImage("woosh_vfx 3R4C.png");
            player_death_vfx = InternalCalls.GetSpriteImage("Death Vfx R1 C11.png");
            player_powerup_vfx = InternalCalls.GetSpriteImage("collect_fragment_vfx 2R6C.png");

            player_reward_dash = InternalCalls.GetSpriteImage("Dash UI.png");
            player_reward_doublejump = InternalCalls.GetSpriteImage("Double Jump UI.png");
            player_reward_wallclimb = InternalCalls.GetSpriteImage("Wall Climb UI.png");

            jump_entity = InternalCalls.CreateEntityVFX("jump", player_jump_vfx);
            InternalCalls.CreateAnimationFromSpriteEntity(3, 4, 0.3f, jump_entity);

            if (InternalCalls.GetCurrentAnimationEntity(jump_entity) >= 11)
            {
                InternalCalls.TransformSetScaleEntity(0, 0, jump_entity);
                InternalCalls.TransformSetPositionEntity(-999, -9999, jump_entity);
            }

            //isGrounded = false;

            gravity_scale = InternalCalls.GetGravityScale();

            // helper entity
            entity_feet = InternalCalls.CreateEntity("FeetCollider");
            InternalCalls.AddCollider(entity_feet);

            player_width = InternalCalls.GetTransformScaling().x / 3f;
            player_height = InternalCalls.GetTransformScaling().y;

            // camera
            InternalCalls.CameraSetZoom(1f);

            death_entity = InternalCalls.CreateEntityVFX("Death", player_death_vfx);
            InternalCalls.CreateAnimationFromSpriteEntity(1, 11, 0.9f, death_entity);

        }

        static public void Update()
        {
            // animation sets
            if (InternalCalls.GetCurrentAnimationEntity(death_entity) >= 10)
            {
                InternalCalls.TransformSetScaleEntity(0, 0, death_entity);
                InternalCalls.TransformSetPositionEntity(-999, -9999, death_entity);
            }

            if (health <= 0)
            {
                isDead = true;
            }

            if (isDead)
            {
                PlayDeadEffect();
                return;
            }
            initialDeath = true;

            //InternalCalls.TransformSetRotation(0f,0f); // for now only
            player_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformPosition());

            trans_scaling = Vector2D.FromSimpleVector2D(InternalCalls.GetTransformScaling());
            if (InternalCalls.KeyHeld((int)KeyCodes.A)) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } }
            else if (InternalCalls.KeyHeld((int)KeyCodes.D)) { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }
            if(isGrounded && !InternalCalls.KeyHeld((int)KeyCodes.A) && !InternalCalls.KeyHeld((int)KeyCodes.D)) { InternalCalls.RigidBodySetForceX(player_vel.x/1.5f); } // let player slide for abit
            move_input = BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.D)) - BoolToInt(InternalCalls.KeyHeld((int)KeyCodes.A));
            
            if (move_input != 0)
            {
                InternalCalls.SetSpriteAnimationIndex(0);
                InternalCalls.SetSpriteImage(player_walk);
            }
            else
            {
                InternalCalls.SetSpriteAnimationIndex(1);
                InternalCalls.SetSpriteImage(player_idle);
            }

            player_vel = Vector2D.FromSimpleVector2D(InternalCalls.RigidBodyGetVelocity());
            //transform.Translate(movement * 5 * Time.deltaTime);
            // moving left or right when A/D being pressed
            InternalCalls.RigidBodyAddForce(move_input * move_speed, 0);
            if (MathF.Abs(player_vel.x) > max_speed) { InternalCalls.RigidBodySetForce(MathF.Sign(player_vel.x) * max_speed, player_vel.y);}
            if (MathF.Abs(player_vel.y) > jump_height) { InternalCalls.RigidBodySetForce(player_vel.x, MathF.Sign(player_vel.y) * jump_height);}

            CollisionEnterCheck();

            if (isGrounded && !(InternalCalls.KeyHeld((int)KeyCodes.Space))) // if player on the ground
            {
                jumps_left = max_jumps;
                //DoubleJumpEnable = false;
                /*if (InternalCalls.KeyPressed((int)KeyCodes.Space))
                {
                    //Jump();
                    *//*InternalCalls.ResetSpriteAnimationFrameEntity(jump_entity);
                    InternalCalls.TransformSetScaleEntity(100 * -move_input, 100, jump_entity);
                    InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, jump_entity);
                    InternalCalls.SetSpriteImage(player_jump);*//*
                    InternalCalls.RigidBodyAddForce(0f, jump_height);
                    isGrounded = false;
                    DoubleJumpEnable = true;
                }*/
                //InternalCalls.SetGravityScale(gravity_scale * 0f);
            }

            if (InternalCalls.KeyPressed((int)KeyCodes.Space) && jumps_left > 0) // jump
            {
               // if (isGrounded || DoubleJumpEnable)
                {
                    InternalCalls.RigidBodySetForce(player_vel.x, 0f); //reset the vel otherwise double jump will be higher
                    InternalCalls.RigidBodyAddForce(0f, jump_height);
                    // move the feet collider away otherwise it will check with the ground again when jumping
                    InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, entity_feet);
                    isGrounded = false;
                    //isJumping = true;
                    jumps_left -= 1;
                    //DoubleJumpEnable = !DoubleJumpEnable; // it change from false to true during first jumping, true to false during second jumping
                }
            }

            // when jumping in the air
            if (!isGrounded)
            {
                InternalCalls.SetSpriteAnimationIndex(2);
                InternalCalls.SetSpriteImage(player_jump);
                InternalCalls.SetGravityScale(gravity_scale);
                if (player_vel.y < 10f)
                {
                    InternalCalls.SetGravityScale(gravity_scale * fall_multiplier * 1.1f); // higher jump
                }
                else if(jumps_left != 1 /*not apply to first jump only*/ || 
                    player_vel.y > 10f && !(InternalCalls.KeyHeld((int)KeyCodes.Space)))
                {
                    InternalCalls.SetGravityScale(gravity_scale * fall_multiplier / 1.1f); // lower jump
                }
            }

            // dash
            if (canDash == true && isDashing == false)
            {
                if (InternalCalls.KeyPressed((int)KeyCodes.LeftShift))
                {
                    isDashing = true;
                }
            }
            if (isDashing)
            {
                if (bullet_time_timer > 0)
                {
                    bullet_time_timer -= InternalCalls.GetDeltaTime();
                    InternalCalls.RigidBodySetForce(0, 0);
                    InternalCalls.SetGravityScale(0f);

                    //Get mouse pos
                    Vector2D mouse_pos = Vector2D.FromSimpleVector2D(InternalCalls.GetMousePosition());

                    float angle = CustomMath.AngleBetweenPoints(player_pos, mouse_pos);

                    move_input = 0;
                    InternalCalls.SetSpriteImage(player_idle);

                    if (mouse_pos.x > player_pos.x) { if (trans_scaling.x < 0) { trans_scaling.x *= -1; } } else { if (trans_scaling.x > 0) { trans_scaling.x *= -1; } }

                    dash_dir = Vector2D.DirectionFromAngle(angle);
                    dash_dir = dash_dir.Normalize();

                    var color = (1f, 1f, 1f);
                    InternalCalls.DrawLineBetweenPoints(player_pos.x, player_pos.y, mouse_pos.x, mouse_pos.y, color);
                    // Render Circles
                    for (int i = 1; i <= 5; i++)
                    {
                        float scale_multiplier = (i / (float)bullet_time_duration);
                        InternalCalls.DrawCircle(
                            player_pos.x, player_pos.y,
                            trans_scaling.x * 50f * (bullet_time_timer / scale_multiplier),
                            trans_scaling.y * 50f * (bullet_time_timer / scale_multiplier),
                            color
                        );
                    }
                }
                else
                {
                    InternalCalls.SetGravityScale(gravity_scale);
                    Dashing();
                }
                
                isGrounded = false; // must have otherwise will stuck in the floor
                
            }

            // Set the rotation to be the same as the detected one
            float collided_angle = InternalCalls.GetCollidedObjectAngle(entity_feet);
            if ((collided_angle > 0 && collided_angle < 45) || (collided_angle > 315 && collided_angle < 360))
            {
                InternalCalls.TransformSetRotation(collided_angle, 0);
            }
            else
            {
                InternalCalls.TransformSetRotation(0, 0);
            }

            // camera
            target_pos.x = player_pos.x + move_input * CustomMath.min(200f, CustomMath.Abs(InternalCalls.RigidBodyGetVelocity().x));
            target_pos.y = player_pos.y + CustomMath.min(100f, InternalCalls.RigidBodyGetVelocity().y / 20f);

            float interpolate_speed = 4f;
            //camera_pos = Vector2D.Lerp(camera_pos, target_pos, interpolate_speed * InternalCalls.GetDeltaTime());

            camera_pos = player_pos;
            InternalCalls.AttachCamera(camera_pos.x, camera_pos.y);
            InternalCalls.TransformSetScale(trans_scaling.x, trans_scaling.y);//setting image flips
            FloorCheckerUpdate();
            //Console.WriteLine(isGrounded);
        }

        static public void CleanUp()
        {
            // Empty for now
        }

        public static int BoolToInt(bool boolValue)
        {
            return boolValue ? 1 : 0;
        }

        public static void CollisionEnterCheck()
        {
            // when feet collider colliding
            /*if (InternalCalls.OnEntityCollisionEnter(entity_feet))
            {
                // check with ground
                if (InternalCalls.CompareEntityCategory(entity_feet, "Ground"))
                {
                    isGrounded = true;
                    canDash = true;
                    //isJumping = false;
                }
                else
                {
                    isGrounded = false;
                }
            }*/

            // when player itself colliding
            if (InternalCalls.OnCollisionEnter())
            {
                if (InternalCalls.CompareCategory("Ground"))
                {
                    isGrounded = true;
                    canDash = true;
                    //isJumping = false;
                }
                else
                {
                    isGrounded = false;
                }

                if (InternalCalls.CompareCategory("Spikes"))
                {
                    isDead = true;
                }
            }
        }

        public static void Dashing()
        {
            if (initialDash == true)
            {
                InternalCalls.AudioPlaySound("Sward-Whoosh_3.wav", false, 0.3f);
                initialDash = false;
            }

            for (int i = 0; i < 36; i++)
            {

                float direction = -1 ^ i * 10;
                float size = 20f * (bullet_time_timer / bullet_time_duration);
                float scale = 2f;
                float dash_particle_alpha = 1f;
                float growth = -0.05f;
                float speed = 10000f;
                InternalCalls.GameSpawnParticleExtra(player_pos.x, player_pos.y,
                    direction, size, scale, dash_particle_alpha,
                    growth, bullet_time_duration, speed, "Particle Test.txt");
            }

            float alpha = 1f - (dash_timer / dash_duration);
            InternalCalls.GameSpawnParticleExtraImage(player_pos.x, player_pos.y,
                                                        0.0f, trans_scaling.x, trans_scaling.y, 1, alpha, 0.0f, 0.2f,
                                                        0, "Particle Empty.txt", "Dash AfterImage.png");

            canDash = false;
            dash_timer -= InternalCalls.GetDeltaTime();
            if (dash_timer <= 0)
            {
                ResetDashStatus();
                //return; // uncomment this will instaniously set vel to 0 and fall right down after dashing
            }
            InternalCalls.RigidBodySetForce(dash_dir.x * dashSpeed * -1, dash_dir.y * dashSpeed * -1);
        }

        public static void ResetDashStatus()
        {
            isDashing = false;
            dash_timer = dash_duration;
            bullet_time_timer = bullet_time_duration;
            initialDash = true;
            InternalCalls.RigidBodySetForce(0, 0);
        }

        public static void PlayDeadEffect()
        {
            if (initialDeath)
            {
                InternalCalls.SetSpriteImage(player_transparent);
                InternalCalls.TransformSetPositionEntity(player_pos.x, player_pos.y, death_entity);
                InternalCalls.ResetSpriteAnimationFrameEntity(death_entity);
                InternalCalls.TransformSetPosition(respawn_x, respawn_y);
                InternalCalls.TransformSetScaleEntity(300, 300, death_entity);
                InternalCalls.AudioPlaySound("DieSound.wav", false, 0.2f);
                ResetDashStatus();

                for (int i = 0; i < 36; i++)
                {
                    InternalCalls.GameSpawnParticleExtra(
                        player_pos.x, player_pos.y, i * 10, 8, -2, 0.9f, -0.1f, 1f, 300f, "Particle Test"
                     );
                }

                initialDeath = false;
            }
            else
            {

                if (camera_shake_duration > 0)
                {
                    camera_shake_duration -= InternalCalls.GetDeltaTime();

                    camera_shake_timer -= InternalCalls.GetDeltaTime();

                    if (camera_shake_timer <= 0)
                    {
                        camera_shake_dir = Vector2D.DirectionFromAngle(camera_shake_angle);
                        camera_shake_timer = camera_shake_set;
                        camera_shake_angle += CustomMath.PI / 4;
                    }
                    InternalCalls.AttachCamera(camera_pos.x + 20 * camera_shake_dir.x, camera_pos.y + 20 * camera_shake_dir.y);
                }

                respawn_timer -= InternalCalls.GetDeltaTime();
                if (respawn_timer <= 0)
                {
                    camera_shake_duration = camera_shake_duration_set;
                    camera_shake_angle = 0;

                    respawn_timer = respawn_timer_set;
                    isDead = false;
                    InternalCalls.CameraSetZoom(1.2f);

                }

            }
        }

        static public void FloorCheckerUpdate()
        {
            //Player x y coord
            /*xCoord = InternalCalls.GetTransformPosition().x;
            yCoord = InternalCalls.GetTransformPosition().y;*/
            float rotationAngle = InternalCalls.GetTransformRotation();
            float angleRadians = rotationAngle * (CustomMath.PI / 180.0f);
            float distanceBelow = player_height / 2.1f;

            Vector2D relativePosition = new Vector2D(0, distanceBelow);

            // Apply rotation to the relative position
            // Rotation matrix in 2D: [cos(theta), -sin(theta); sin(theta), cos(theta)]
            Vector2D rotatedRelativePosition = new Vector2D(
                (float)(relativePosition.x * CustomMath.Cos(angleRadians) + relativePosition.y * CustomMath.Sin(angleRadians)),
                (float)(relativePosition.x * CustomMath.Sin(angleRadians) - relativePosition.y * CustomMath.Cos(angleRadians))
                );


            // Calculate the absolute position for the floor checker
            Vector2D checkerPosition = new Vector2D(
                player_pos.x + rotatedRelativePosition.x,
                player_pos.y + rotatedRelativePosition.y
            );

            // Set the floor checker's position
            InternalCalls.TransformSetPositionEntity(checkerPosition.x, checkerPosition.y, entity_feet);
            InternalCalls.TransformSetRotationEntity(rotationAngle, 0, entity_feet);
            InternalCalls.TransformSetScaleEntity(player_width / 1.8f, 2f, entity_feet);
            InternalCalls.DrawImageAt
                (
                    new SimpleVector2D(checkerPosition.x, checkerPosition.y), 0, new SimpleVector2D(player_width / 1.8f, 2f), player_reward_dash, 1f, 4
                );
        }

    } //player script

}


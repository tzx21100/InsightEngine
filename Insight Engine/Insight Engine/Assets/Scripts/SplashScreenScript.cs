/*!
 * \file SplashScreenScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This C# file contains the SplashScreenScript class, used for the splash screen (SplashScreen.insight)
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Numerics;
namespace IS
{
    class SplashScreenScript
    {
        static private float timer;
        static private float dt;
        //static public bool is_lighted = false;

        //static private float light_toggle_timer;
        //static private float light_toggle_interval;

        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        //static private int team_logo_entity;
        //static private int splash_entity;
        //static private int top_left_splash_entity;
        //static private int top_right_splash_entity;
        //static private int bottom_left_splash_entity;
        //static private int bottom_right_splash_entity;

        //static SimpleImage team_logo_image;
        //static SimpleImage splash_image;
        //static SimpleImage top_left_splash_image;
        //static SimpleImage top_right_splash_image;
        //static SimpleImage bottom_left_splash_image;
        //static SimpleImage bottom_right_splash_image;

        //static private float jigsaw_scale_x;
        //static private float jigsaw_scale_y;

        //static Vector2D team_logo_pos = new Vector2D(0, 0);
        //static Vector2D splash_pos = new Vector2D(0, 0);
        //static Vector2D top_left_splash_pos = new Vector2D(0, 0);
        //static Vector2D top_right_splash_pos = new Vector2D(0, 0);
        //static Vector2D bottom_left_splash_pos = new Vector2D(0, 0);
        //static Vector2D bottom_right_splash_pos = new Vector2D(0, 0);

        //static private Vector2D top_left_start_pos = new Vector2D(0, 0);
        //static private Vector2D top_right_start_pos = new Vector2D(0, 0);
        //static private Vector2D bottom_left_start_pos = new Vector2D(0, 0);
        //static private Vector2D bottom_right_start_pos = new Vector2D(0, 0);

        static public string splashScreenVideo = "Assets/Videos/Splashscreen.mov";

        // Animation
        //static private float jigsaw_animation_duration;
        //static private float jigsaw_animation_timer;
        //static private float logo_animation_duration;
        //static private float logo_animation_timer;

        static public void Init()
        {

            timer = 8.15f;
            InternalCalls.SetLightsToggle(false);
            InternalCalls.loadVideo(splashScreenVideo, 1.0f, 1.0f, 0.0f, 0.0f, true);
            InternalCalls.AudioPlayMusicSFX("splashscreen audio.wav", 0.2f);

            //dt = InternalCalls.GetFixedDeltaTime();
            //jigsaw_animation_duration = 1.5f;
            //jigsaw_animation_timer = jigsaw_animation_duration;
            //logo_animation_duration = 1f;
            //logo_animation_timer = logo_animation_duration;

            //top_left_splash_image = InternalCalls.GetSpriteImage("splash_top_left.png");
            //top_right_splash_image = InternalCalls.GetSpriteImage("splash_top_right.png");
            //bottom_left_splash_image = InternalCalls.GetSpriteImage("splash_bottom_left.png");
            //bottom_right_splash_image = InternalCalls.GetSpriteImage("splash_bottom_right.png");
            //splash_image = InternalCalls.GetSpriteImage("main_menu_bg.jpeg");
            //team_logo_image = InternalCalls.GetSpriteImage("team_logo.png");

            //top_left_splash_entity = InternalCalls.CreateEntityUI("Top Left", top_left_splash_image);
            //top_right_splash_entity = InternalCalls.CreateEntityUI("Top Right", top_right_splash_image);
            //bottom_left_splash_entity = InternalCalls.CreateEntityUI("Bottom Left", bottom_left_splash_image);
            //bottom_right_splash_entity = InternalCalls.CreateEntityUI("Bottom Right", bottom_right_splash_image);
            //splash_entity = InternalCalls.CreateEntityUI("Splash", splash_image);
            //team_logo_entity = InternalCalls.CreateEntityUI("Team Logo", team_logo_image);

            //InternalCalls.SetSpriteAlphaEntity(0f, team_logo_entity);
            //InternalCalls.AttachLightComponentToEntityWithOffset(team_logo_entity, 1.0f, 0f, 0f, 1.0f, 400.0f, new SimpleVector2D(5.0f, 70.0f));
            //InternalCalls.SetLightToggleEntity(team_logo_entity, false);

            //light_toggle_interval = 0.2f;
            //light_toggle_timer = 0f;
        }

        static public void Update()
        {
            dt = InternalCalls.GetDeltaTime();
            timer -= dt;
            if (timer <= 0.0f)
            {
                InternalCalls.SetLightsToggle(true);
                InternalCalls.unloadVideos();
                InternalCalls.AudioStopAllSounds();
                InternalCalls.LoadScene("Assets/Scenes/Fmod.insight");
            }


            //// get window size
            //win_dimension.x = (float)InternalCalls.GetWindowWidth();
            //win_dimension.y = (float)InternalCalls.GetWindowHeight();
            //InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);

            //// set origin
            //origin.x = -(win_dimension.x / 2f); //-960
            //origin.y = -(win_dimension.y / 2f); //-540

            //jigsaw_scale_x = (win_dimension.x / 2) * 1.09f;
            //jigsaw_scale_y = (win_dimension.y / 2) * 1.167f;

            //top_left_start_pos = new Vector2D(-jigsaw_scale_x, -jigsaw_scale_y);
            //top_right_start_pos = new Vector2D(win_dimension.x + jigsaw_scale_x, -jigsaw_scale_y);
            //bottom_left_start_pos = new Vector2D(-jigsaw_scale_x, win_dimension.y + jigsaw_scale_y);
            //bottom_right_start_pos = new Vector2D(win_dimension.x + jigsaw_scale_x, win_dimension.y + jigsaw_scale_y);

            //InternalCalls.TransformSetScaleEntity(jigsaw_scale_x, jigsaw_scale_y, top_left_splash_entity);
            //InternalCalls.TransformSetScaleEntity(jigsaw_scale_x, jigsaw_scale_y, top_right_splash_entity);
            //InternalCalls.TransformSetScaleEntity(jigsaw_scale_x, jigsaw_scale_y, bottom_left_splash_entity);
            //InternalCalls.TransformSetScaleEntity(jigsaw_scale_x, jigsaw_scale_y, bottom_right_splash_entity);
            //InternalCalls.TransformSetScaleEntity(win_dimension.x, win_dimension.y  , splash_entity);
            //InternalCalls.TransformSetScaleEntity(win_dimension.x/2, win_dimension.y/2, team_logo_entity);

            //top_left_splash_pos.Set(origin.x + (win_dimension.x * 0.262f), origin.y + (win_dimension.y * 0.698f)); //-455.14, 214.43
            //top_right_splash_pos.Set(origin.x + (win_dimension.x * 0.727f), origin.y + (win_dimension.y * 0.698f)); //437.59, 214.43
            //bottom_left_splash_pos.Set(origin.x + (win_dimension.x * 0.268f), origin.y + (win_dimension.y * 0.279f)); //-449.29, -237.83
            //bottom_right_splash_pos.Set(origin.x + (win_dimension.x * 0.731f), origin.y + (win_dimension.y * 0.294f)); //444.9. -221.74
            //splash_pos.Set(origin.x + (win_dimension.x * 0.5f), origin.y + (win_dimension.y * 0.5f));
            //team_logo_pos.Set(origin.x + (win_dimension.x * 0.5f), origin.y + (win_dimension.y * 0.5f));

            //HideJigsaw();
            //if (jigsaw_animation_timer > 0)
            //{
            //    float progress = (jigsaw_animation_duration - jigsaw_animation_timer) / jigsaw_animation_duration;

            //    // Interpolate positions
            //    top_left_splash_pos = Vector2D.Lerp(top_left_start_pos, top_left_splash_pos, progress);
            //    top_right_splash_pos = Vector2D.Lerp(top_right_start_pos, top_right_splash_pos, progress);
            //    bottom_left_splash_pos = Vector2D.Lerp(bottom_left_start_pos, bottom_left_splash_pos, progress);
            //    bottom_right_splash_pos = Vector2D.Lerp(bottom_right_start_pos, bottom_right_splash_pos, progress);

            //    // Update timer
            //    jigsaw_animation_timer -= dt;

            //}
            //DrawJigsawPieces();
            //DrawLogo();

            //timer += dt;
            //if(timer >= 2)
            //{
            //    HideJigsawPieces();
            //    DrawSplash();
            //}
            //if (timer >= 2 && logo_animation_timer > 0)
            //{
            //    float alpha;

            //    float progress = (logo_animation_duration - logo_animation_timer) / logo_animation_duration;
            //    alpha = Lerp(0.0f, 1.0f, progress);

            //    InternalCalls.SetSpriteAlphaEntity(alpha, team_logo_entity);
            //    logo_animation_timer -= dt;
            //}

            //light_toggle_timer += dt;
            //if (timer >= 3 && timer <= 4.2f && light_toggle_timer >= light_toggle_interval)
            //{
            //    is_lighted = !is_lighted;
            //    InternalCalls.SetLightToggleEntity(team_logo_entity, is_lighted);

            //    // Reset the light toggle timer
            //    light_toggle_timer = 0f;
            //}

            //if (timer >= 5)
            //{
            //    //add transition effect
            //    InternalCalls.LoadScene("Assets/Scenes/MainMenu.insight");
            //}
        }


        static public void CleanUp()
        {

        }

        //static private void DrawJigsawPieces() //ltr change to glass shattering effect
        //{
        //    InternalCalls.TransformSetPositionEntity(top_left_splash_pos.x, top_left_splash_pos.y, top_left_splash_entity);
        //    InternalCalls.TransformSetPositionEntity(top_right_splash_pos.x, top_right_splash_pos.y, top_right_splash_entity);
        //    InternalCalls.TransformSetPositionEntity(bottom_left_splash_pos.x, bottom_left_splash_pos.y, bottom_left_splash_entity);
        //    InternalCalls.TransformSetPositionEntity(bottom_right_splash_pos.x, bottom_right_splash_pos.y, bottom_right_splash_entity);
        //}
        //static private void HideJigsawPieces()
        //{
        //    InternalCalls.TransformSetPositionEntity(9999f, 9999f, top_left_splash_entity);
        //    InternalCalls.TransformSetPositionEntity(9999f,9999f, top_right_splash_entity);
        //    InternalCalls.TransformSetPositionEntity(9999f, 9999f, bottom_left_splash_entity);
        //    InternalCalls.TransformSetPositionEntity(9999f, 9999f, bottom_right_splash_entity);
        //}
        //static private void DrawSplash()
        //{
        //    InternalCalls.TransformSetPositionEntity(splash_pos.x, splash_pos.y, splash_entity);
        //}
        //static private void HideJigsaw()
        //{
        //    InternalCalls.TransformSetPositionEntity(9999f, 9999f, splash_entity);
        //}

        //static private void DrawLogo()
        //{
        //    InternalCalls.TransformSetPositionEntity(team_logo_pos.x, team_logo_pos.y, team_logo_entity);
        //}

        //static private float Lerp(float start, float end, float t)
        //{
        //    return start + (end - start) * t;
        //}

    }
}
/*!
 * \file CreditsScript.cs
 * \author Matthew Ng, matthewdeen.ng@digipen.edu 
 * \par Course: CSD2451
 * \brief
 * This c# file contains the CreditsScript class, used for Credits of the game
 * 
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
using System;
using System.Runtime.CompilerServices;
namespace IS
{
    class CreditsScript
    {
        // Windows
        static Vector2D win_dimension = new Vector2D(0, 0);
        static Vector2D origin = new Vector2D(0, 0);

        // Camera
        static Vector2D camera_pos = new Vector2D(0, 0);
        static float camera_zoom = 0f;

        static private float timer;
        static private float dt;
        static private float t;
        static private float bottomY;
        static private float topY;
        static private float duration;
        static private float normalizedYPos;
        static private float releasedYPos;
        static public void Init()
        {
            timer = 0f;
            dt = InternalCalls.GetDeltaTime();
            bottomY = 1.1f; // Start position for the text (below the bottom of the screen)
            topY = -0.1f; // End position for the text (above the top of the screen)
            duration = 10f;
        }

        static public void Update()
        {
            // Camera
            camera_zoom = InternalCalls.CameraGetZoom();

            //set camera pos
            camera_pos.x = InternalCalls.GetCameraPos().x;
            camera_pos.y = InternalCalls.GetCameraPos().y;

            win_dimension.x = (float)InternalCalls.GetWindowWidth() / camera_zoom;
            win_dimension.y = (float)InternalCalls.GetWindowHeight() / camera_zoom;

            origin.x = camera_pos.x - (win_dimension.x / 2f);
            origin.y = camera_pos.y - (win_dimension.y / 2f);
            InternalCalls.TransformSetScale(win_dimension.x, win_dimension.y);

            // Store the current position before checking mouse button state
            releasedYPos = normalizedYPos;

            //// Check for click event
            //if (InternalCalls.MouseHeld(0))
            //{
            //    duration = 5f;
            //}
            //else {
            //    duration = 10f;
            //    topY = releasedYPos;
            //}
        
            // Calculate the interpolation factor based on the timer
            t = Math.Clamp(timer / duration, 0f, 1f);

            // Calculate the normalized Y position by interpolating between startY and endY
            normalizedYPos = Lerp(topY, bottomY, t);

            InternalCalls.RenderTextFont("FRAGMENTS", "MedusaGothic_D", GetNormalizedX(0.5f), normalizedYPos, 12f, (1f, 1f, 1f, 1f));

            float secondLineOffset = 0.05f; // Adjust this value to change the spacing between lines
            InternalCalls.RenderTextFont("By Insight", "Semplicita_Light", GetNormalizedX(0.5f), normalizedYPos - secondLineOffset, 15f, (1f, 1f, 1f, 1f));

            timer += dt;
        }

        static public void CleanUp()
        {
        }

        //static private void DrawCredits()
        //{

        //}

        static private float GetNormalizedX(float x)
        {
            float normalized_x = (x - origin.x) / win_dimension.x;
            normalized_x = Math.Clamp(normalized_x, 0f, 1f);
            return normalized_x;
        }

        //static private float GetNormalizedY(float y)
        //{
        //    float normalized_y = (y - origin.y) / win_dimension.y;
        //    normalized_y = Math.Clamp(normalized_y, 0f, 1f);
        //    return normalized_y;
        //}

        static private float Lerp(float start, float end, float t)
        {
            return start + (end - start) * t;
        }
    }
}
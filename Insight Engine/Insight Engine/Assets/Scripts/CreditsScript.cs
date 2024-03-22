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
        static private float offset;
        static private float lineBreak;
        static private float releasedYPos;
        static public void Init()
        {
            timer = 0f;
            dt = InternalCalls.GetDeltaTime();
            bottomY = 3.1f; // End position for the text (above the top of the screen)
            topY = -0.1f; // Top position for the text (below the bottom of the screen)
            duration = 10f;
            normalizedYPos = 0f;
            t = 0f;

            //offsets
            offset = 0.05f;
            lineBreak = offset;
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

            InternalCalls.RenderTextFont("FRAGMENTS", "MedusaGothic_D", 0.5f, normalizedYPos, 15f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("By Insight", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak, 18f, (1f, 1f, 1f, 1f));
            
            InternalCalls.RenderTextFont("TEAM", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * 3f, 15f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Guo Yiming", "Semplicita_Light", 0.25f, normalizedYPos - lineBreak * 4f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Programmer", "Semplicita_Light", 0.75f, normalizedYPos - lineBreak * 4f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Koh Yan Khang", "Semplicita_Light", 0.25f, normalizedYPos - lineBreak * 5.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Product Manager", "Semplicita_Light", 0.75f, normalizedYPos - lineBreak * 5.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Ng De En Matthew", "Semplicita_Light", 0.25f, normalizedYPos - lineBreak * 7f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Programmer", "Semplicita_Light", 0.75f, normalizedYPos - lineBreak * 7f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Tan Zheng Xun", "Semplicita_Light", 0.25f, normalizedYPos - lineBreak * 8.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Technical Lead", "Semplicita_Light", 0.75f, normalizedYPos - lineBreak * 8.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Wu Zekai", "Semplicita_Light", 0.25f, normalizedYPos - lineBreak * 10f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Programmer", "Semplicita_Light", 0.75f, normalizedYPos - lineBreak * 10f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Ee Tze Rong Cheryl", "Semplicita_Light", 0.25f, normalizedYPos - lineBreak * 11.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Design Lead", "Semplicita_Light", 0.75f, normalizedYPos - lineBreak * 11.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Lim Wan Jin", "Semplicita_Light", 0.25f, normalizedYPos - lineBreak * 13f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Art Lead", "Semplicita_Light", 0.75f, normalizedYPos - lineBreak * 13f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Ang Qin Tian Rachel", "Semplicita_Light", 0.25f, normalizedYPos - lineBreak * 14.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Artist", "Semplicita_Light", 0.75f, normalizedYPos - lineBreak * 14.5f, 18f, (1f, 1f, 1f, 1f));
        
            InternalCalls.RenderTextFont("Instructors", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * 16.5f, 15f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Elie Hosry", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 18f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Goh Dian Yang", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 19.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Lee Chee Yuan Alwyn", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 21f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Goh Jing Ying", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 22.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Danielle Fong Sum Yee", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 24f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Holger Liebnitz", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 25.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Vuk Krakovic", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 27f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Gavin Parker", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 28.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Keh Choon Wee", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 30f, 18f, (1f, 1f, 1f, 1f));

            InternalCalls.RenderTextFont("Created At", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * 32f, 15f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Digipen Insitute of Technology Singapore", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 33.5f, 18f, (1f, 1f, 1f, 1f));

            InternalCalls.RenderTextFont("President", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * 35.5f, 15f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Claude Comair", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 37f, 18f, (1f, 1f, 1f, 1f));

            InternalCalls.RenderTextFont("Board of Directors", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * 39f, 15f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Chu Jason Yeu Tat", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 40.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Michele Comair", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 42f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Christopher Comair", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 43.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Alexander Comair", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 45f, 18f, (1f, 1f, 1f, 1f));

            InternalCalls.RenderTextFont("Executives", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * 47f, 15f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Michael Gats", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 48.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Tan Chek Ming", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 50f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Prasanna Kumar Ghali", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 51.5f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Mandy Wong", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 53f, 18f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Johnny Deek", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * 54.5f, 18f, (1f, 1f, 1f, 1f));

            //InternalCalls.RenderTextFont("Created At", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * 32f, 15f, (1f, 1f, 1f, 1f));


            timer += dt;
        }

        static public void CleanUp()
        {
        }

        //static private void DrawCredits()
        //{

        //}

        //static private float GetNormalizedX(float x)
        //{
        //    float normalized_x = (x - origin.x) / win_dimension.x;
        //    normalized_x = Math.Clamp(normalized_x, 0f, 1f);
        //    return normalized_x;
        //}

        static private float Lerp(float start, float end, float t)
        {
            return start + (end - start) * t;
        }
    }
}
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
using System.Collections.Generic;
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

        static private List<(string name, string role)> teamMembers = new List<(string name, string role)>();
        static private List<string> instructors = new List<string>();
        static private List<string> boardOfDirectors = new List<string>();
        static private List<string> executives = new List<string>();
        static public void Init()
        {
            timer = 0f;
            dt = InternalCalls.GetDeltaTime();
            bottomY = 5f; // End position for the text (above the top of the screen)
            topY = -0.1f; // Top position for the text (below the bottom of the screen)
            duration = 40f;
            normalizedYPos = 0f;
            t = 0f;

            //offsets
            offset = 0.05f;
            lineBreak = offset;

            teamMembers.Clear();
            instructors.Clear();
            boardOfDirectors.Clear();
            executives.Clear();

            teamMembers.Add(("Guo Yiming", "Programmer"));
            teamMembers.Add(("Koh Yan Khang", "Product Manager"));
            teamMembers.Add(("Ng De En Matthew", "Programmer"));
            teamMembers.Add(("Tan Zheng Xun", "Technical Lead"));
            teamMembers.Add(("Wu Zekai", "Programmer"));
            teamMembers.Add(("Ee Tze Rong Cheryl", "Design Lead"));
            teamMembers.Add(("Lim Wan Jin", "Art Lead"));
            teamMembers.Add(("Ang Qin Tian Rachel", "Artist"));

            instructors.Add("Elie Hosry");
            instructors.Add("Goh Dian Yang");
            instructors.Add("Lee Chee Yuan Alwyn");
            instructors.Add("Goh Jing Ying");
            instructors.Add("Danielle Fong Sum Yee");
            instructors.Add("Holger Liebnitz");
            instructors.Add("Vuk Krakovic");
            instructors.Add("Gavin Parker");
            instructors.Add("Keh Choon Wee");

            boardOfDirectors.Add("Chu Jason Yeu Tat");
            boardOfDirectors.Add("Michele Comair");
            boardOfDirectors.Add("Christopher Comair");
            boardOfDirectors.Add("Alexander Comair");

            executives.Add("Michael Gats");
            executives.Add("Tan Chek Ming");
            executives.Add("Prasanna Kumar Ghali");
            executives.Add("Mandy Wong");
            executives.Add("Johnny Deek");
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
            
            InternalCalls.RenderTextFont("TEAM", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * 4f, 15f, (1f, 1f, 1f, 1f));
            float teamMemberLineBreak = 5.5f;
            foreach (var member in teamMembers)
            {
                InternalCalls.RenderTextFont(member.name, "Semplicita_Light", 0.35f, normalizedYPos - lineBreak * teamMemberLineBreak, 18f, (1f, 1f, 1f, 1f));
                InternalCalls.RenderTextFont(member.role, "Semplicita_Light", 0.65f, normalizedYPos - lineBreak * teamMemberLineBreak, 18f, (1f, 1f, 1f, 1f));
                teamMemberLineBreak += 1.5f;
            }

            InternalCalls.RenderTextFont("Instructors", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (teamMemberLineBreak + 2f), 15f, (1f, 1f, 1f, 1f));

            float instructorLineBreak = teamMemberLineBreak + 3.5f;
            foreach (string instructor in instructors)
            {
                InternalCalls.RenderTextFont(instructor, "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * instructorLineBreak, 18f, (1f, 1f, 1f, 1f));
                instructorLineBreak += 1.5f;
            }

            InternalCalls.RenderTextFont("Created At", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (instructorLineBreak + 1.5f), 15f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Digipen Insitute of Technology Singapore", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * (instructorLineBreak + 3f), 18f, (1f, 1f, 1f, 1f));

            InternalCalls.RenderTextFont("President", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (instructorLineBreak + 6f), 15f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("Claude Comair", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * (instructorLineBreak + 7.5f), 18f, (1f, 1f, 1f, 1f));


            InternalCalls.RenderTextFont("Board of Directors", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (instructorLineBreak + 10.5f), 15f, (1f, 1f, 1f, 1f));
            float bodLineBreak = instructorLineBreak + 12f;
            foreach (string bod in boardOfDirectors)
            {
                InternalCalls.RenderTextFont(bod, "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * bodLineBreak, 18f, (1f, 1f, 1f, 1f));
                bodLineBreak += 1.5f;
            }

            InternalCalls.RenderTextFont("Executives", "MedusaGothic_D", 0.5f, normalizedYPos - lineBreak * (bodLineBreak + 1.5f), 15f, (1f, 1f, 1f, 1f));
            
            float execLineBreak = bodLineBreak + 3f;
            foreach (string exec in executives)
            {
                InternalCalls.RenderTextFont(exec, "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * execLineBreak, 18f, (1f, 1f, 1f, 1f));
                execLineBreak += 1.5f;
            }

            InternalCalls.RenderTextFont("www.digipen.edu", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * (execLineBreak + .5f), 9f, (1f, 1f, 1f, 1f));
            InternalCalls.RenderTextFont("All content 2024 DigiPen Institute of Technology Singapore. All Rights Reserved.", "Semplicita_Light", 0.5f, normalizedYPos - lineBreak * (execLineBreak + 1.25f), 9f, (1f, 1f, 1f, 1f));


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
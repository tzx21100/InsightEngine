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

        static private float total_timer;
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

        static private float fadeDuration;
        static private float alpha;
        static private bool isFadingIn;


        static private float gameByGroupStart;
        static private float gameByGroupDuration;
        static private float teamGroupStart; 
        static private float teamGroupDuration;
        static private float memberGroupStart; 
        static private float memberGroupEnd;
        static private float postMemberGroupWaitTime;
        static private float nextSectionStartTime;
        static private float currentTimeAfterMembers;
        static private float memberDisplayDuration;
        static private float totalMemberDisplayTime;
        static private float durationBetweenGroups;
        static private float postDisplayWaitTime;
        static private float endingGroupStartTime;

        static private List<(string name, string role)> teamMembers = new List<(string name, string role)>();
        static private List<string> instructors = new List<string>();
        static private List<string> boardOfDirectors = new List<string>();
        static private List<string> executives = new List<string>();
        static public void Init()
        {
            total_timer = 0f;
            timer = 0f;
            dt = InternalCalls.GetDeltaTime();
            bottomY = 3.3f; // End position for the text (above the top of the screen)
            topY = 0f; // Top position for the text (below the bottom of the screen)
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

            teamMembers.Add(("Koh Yan Khang", "Product Manager"));
            teamMembers.Add(("Tan Zheng Xun", "Technical Lead"));
            teamMembers.Add(("Guo Yiming", "Programmer"));
            teamMembers.Add(("Ng De En Matthew", "Programmer"));
            teamMembers.Add(("Wu Zekai", "Programmer"));
            teamMembers.Add(("Ee Tze Rong Cheryl", "Design Lead"));
            teamMembers.Add(("Lim Wan Jin", "Art Lead"));
            teamMembers.Add(("Ang Qin Tian Rachel", "Artist"));

            isFadingIn = true;
            alpha = 0f;

            fadeDuration = 3f;
            durationBetweenGroups = 1f;
            gameByGroupStart = 0f;
            gameByGroupDuration = 3f;
            teamGroupStart = gameByGroupStart + gameByGroupDuration + durationBetweenGroups;
            teamGroupDuration = 3f;
            memberGroupStart = teamGroupStart + teamGroupDuration + durationBetweenGroups;
            memberDisplayDuration = 3f;
            totalMemberDisplayTime = teamMembers.Count * (memberDisplayDuration + durationBetweenGroups);

            postMemberGroupWaitTime = 3f;
            memberGroupEnd = memberGroupStart + totalMemberDisplayTime + postMemberGroupWaitTime;
            nextSectionStartTime = memberGroupEnd;

            postDisplayWaitTime = 3f;
            endingGroupStartTime = 0f;
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

            InternalCalls.SetSpriteAlpha(0.95f);
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
            //releasedYPos = normalizedYPos;

            //// Check for click event
            //if (InternalCalls.MouseHeld(0))
            //{
            //    duration = 5f;
            //}
            //else {
            //    duration = 10f;
            //    topY = releasedYPos;
            //}



            if (total_timer > gameByGroupStart && total_timer <= gameByGroupStart + gameByGroupDuration)
            {
                alpha = CalculateFadeAlpha();
                InternalCalls.RenderTextFont("FRAGMENTS", "MedusaGothic_D", 0.5f, 0.55f, 15f, (1f, 1f, 1f, alpha));
                InternalCalls.RenderTextFont("By Insight", "Semplicita_Light", 0.5f, 0.5f, 18f, (1f, 1f, 1f, alpha));
            }
            else if (total_timer > gameByGroupStart + gameByGroupDuration && total_timer <= teamGroupStart)
            {
                timer = 0f; // Reset for the next group
            }
            else if (total_timer > teamGroupStart && total_timer <= teamGroupStart + teamGroupDuration)
            {
                alpha = CalculateFadeAlpha();
                InternalCalls.RenderTextFont("TEAM", "MedusaGothic_D", 0.5f, 0.5f, 15f, (1f, 1f, 1f, alpha));
            }
            else if (total_timer > teamGroupStart + teamGroupDuration && total_timer <= memberGroupStart)
            {
                timer = 0f; // Reset for the next group
            }

            if (total_timer > memberGroupStart && total_timer <= memberGroupStart + totalMemberDisplayTime)
            {
                float timeSinceGroupStart = total_timer - memberGroupStart;
                int memberIndex = (int)(timeSinceGroupStart / (memberDisplayDuration + durationBetweenGroups));
                float timeInCurrentSegment = timeSinceGroupStart % (memberDisplayDuration + durationBetweenGroups);

                //Check if we are in the display time(not in the gap)
                if (timeInCurrentSegment <= memberDisplayDuration && memberIndex < teamMembers.Count)
                {
                    var member = teamMembers[memberIndex];

                    float alpha = 1f; // Default to fully visible
                    if (timeInCurrentSegment <= memberDisplayDuration / 2)
                    {
                        //fade in
                        alpha = timeInCurrentSegment / (memberDisplayDuration / 2);
                    }
                    else
                    {
                        //fade out
                        alpha = 1 - ((timeInCurrentSegment - (memberDisplayDuration / 2)) / (memberDisplayDuration / 2));
                    }

                    InternalCalls.RenderTextFont(member.name, "MedusaGothic_D", 0.5f, 0.55f, 15f, (1f, 1f, 1f, alpha));
                    InternalCalls.RenderTextFont(member.role, "Semplicita_Light", 0.5f, 0.55f - lineBreak, 18f, (1f, 1f, 1f, alpha));
                }
            }
            else if (total_timer > teamGroupStart + teamGroupDuration && total_timer <= memberGroupStart)
            {
                timer = 0f; // Reset for the next group
            }
            currentTimeAfterMembers = total_timer - nextSectionStartTime;
            if (total_timer > nextSectionStartTime)
            {
                // Calculate the interpolation factor based on the timer after members display
                float duration = 20f; // Total duration to complete the interpolation, adjust as needed
                float t = Math.Clamp(currentTimeAfterMembers / duration, 0f, 1f);
                // Calculate the normalized Y position by interpolating between startY and endY
                float normalizedYPos = Lerp(topY, bottomY, t);


                InternalCalls.RenderTextFont("Instructors", "MedusaGothic_D", 0.5f, normalizedYPos, 15f, (1f, 1f, 1f, 1f));

                float instructorLineBreak = 1.5f;
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
                
                endingGroupStartTime = nextSectionStartTime + duration + postDisplayWaitTime;
                //float fadeEffectDuration = 5f; // Total duration of the fade effect
                float currentTimeInFadeEffect = total_timer - endingGroupStartTime;

                if (currentTimeInFadeEffect >= 0 && currentTimeInFadeEffect <= fadeDuration)
                {
                    // Calculate alpha for fade in and out
                    if (currentTimeInFadeEffect <= fadeDuration / 2)
                    {
                        // Fade in
                        alpha = currentTimeInFadeEffect / (fadeDuration / 2);
                    }
                    else
                    {
                        // Fade out
                        alpha = 1 - ((currentTimeInFadeEffect - (fadeDuration / 2)) / (fadeDuration / 2));
                    }

                    //text
                    InternalCalls.RenderTextFont("HAHAHAHAHAHAHHA", "MedusaGothic_D", 0.5f, 0.5f, 15f, (1f, 1f, 1f, alpha));
                }
                else if (currentTimeInFadeEffect > fadeDuration + 3f)
                {
                    //do smth
                    //InternalCalls.LoadScene("Assets/Scenes/MainMenu.insight");
                  
                    //timer = 0f; 
                }
            }

            timer += dt;
            total_timer += dt;

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

        static private float CalculateFadeAlpha()
        {
            float alpha;
            if (timer <= fadeDuration / 2f)
            {
                alpha = timer / (fadeDuration / 2f); // Fade in
            }
            else
            {
                alpha = 1f - ((timer - fadeDuration / 2f) / (fadeDuration / 2f)); // Fade out
            }

            if (timer >= fadeDuration)
            {
                timer = 0f; // Reset timer for next calculation
            }

            return alpha;
        }
    }
}